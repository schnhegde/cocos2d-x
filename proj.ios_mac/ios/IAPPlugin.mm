#import "IAPPlugin.h"
#import "GameManager.h"
#import "EventUtils.h"
#import "Events.h"

// TODO: Verify store receipt for security
static IAPPlugin * _instance = nullptr;
#define kBuyGameProductId @"buy_game"

@implementation IAPPlugin

- (void) dealloc {
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];

    self.purchases = nil;
    self.bundleID = nil;

    [super dealloc];
}

- (id) init {
    self = [super init];
    if (!self) {
        return nil;
    }

    self.purchases = [NSMutableDictionary dictionary];

    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];

    self.bundleID = @"unknown.bundle";
    self.productDetails = [NSMutableDictionary dictionary];

    return self;
}

+ (IAPPlugin*) getInstance {
    if (_instance == nil) {
        _instance = [[IAPPlugin alloc] init];
    }
    return _instance;
}

- (void) completeTransaction:(SKPaymentTransaction *)transaction receiptString:(NSString *)receiptString origin: (NSString*) origin {
    NSString *sku = transaction.payment.productIdentifier;
    NSString *token = transaction.transactionIdentifier;

    if ([self.purchases objectForKey:token] != nil) {
        NSLog(@"{billing} WARNING: Strangeness is afoot.  The same purchase token was specified twice");
    }

    // Remember transaction so that it can be consumed later
    // [self.purchases setObject:transaction forKey:token];

    // Strip bundleID prefix
    if ([sku hasPrefix:self.bundleID]) {
        sku = [sku substringFromIndex:([self.bundleID length] + 1)];
    }
    modules::GameManager::getInstance()->setGameOwned(true);
    modules::EventUtils::dispatchEvent(data::Events::GAME_PURCHASED);
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    NSLog(@"{billing} Got products response with %d hits and %d misses", (int)response.products.count, (int)response.invalidProductIdentifiers.count);

    bool success = false;
    NSString *sku = nil;
    NSUInteger len = [self.bundleID length] + 1;
    NSArray *products = response.products;

    //TODO: Do better, right now assuming if product count is > 1 it is for info
    if (products.count > 1) {
        NSMutableDictionary *prices = [[NSMutableDictionary alloc] init];
        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];

        for (SKProduct* product in products) {
            [numberFormatter setLocale:product.priceLocale];
            NSString *formattedPrice = [numberFormatter stringFromNumber:product.price];

            sku = product.productIdentifier;
            if (sku != nil && [sku hasPrefix:self.bundleID]) {
                sku = [sku substringFromIndex:len];
                [prices setObject:formattedPrice forKey:sku];

                NSDictionary *priceDetails = [NSDictionary dictionaryWithObjectsAndKeys:
                                              product.price,@"localPrice",
                                              [numberFormatter currencyCode] ,@"currencyCode", nil];
                [self.productDetails setObject:priceDetails forKey:sku];
            }

        }
        return;
    }
    else if (products.count > 0) {
        SKProduct *product = [products objectAtIndex:0];

        if (product) {
            NSLog(@"{billing} Found product id=%@, title=%@", product.productIdentifier, product.localizedTitle);

            SKPayment *payment =  [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];

            success = true;
        }
    }

    for (NSString *invalidProductId in response.invalidProductIdentifiers) {
        NSLog(@"{billing} Unused product id: %@", invalidProductId);

        sku = invalidProductId;
    }

    if (!success) {
        // Strip bundleID prefix
        if (sku != nil && [sku hasPrefix:self.bundleID]) {
            sku = [sku substringFromIndex:len];
        }
    }
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {

    for (SKPaymentTransaction *transaction in transactions) {
        NSString *sku = transaction.payment.productIdentifier;
        NSString *token = transaction.transactionIdentifier;

        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased: {
                NSLog(@"{billing} Transaction completed purchase for sku=%@ and token=%@", sku, token);
                // The following receipt validation is done according to the following article
                // https://developer.apple.com/documentation/storekit/in-app_purchase/validating_receipts_with_the_app_store
                NSString *receiptString = @"";
                NSString *origin = @"";

                if ([NSBundle respondsToSelector:@selector(appStoreReceiptURL)]) {
                    //For iOS 7 and up
                    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
                    NSData *receiptFile = [NSData dataWithContentsOfURL:receiptURL];
                    receiptString = [receiptFile base64EncodedStringWithOptions:NSUTF8StringEncoding];
                }

                if (self.storeOrigin != nil && [self.storeOrigin isEqual:transaction.payment]) {
                    self.storeOrigin = nil;
                    origin = @"store";
                }
                [self completeTransaction:transaction receiptString:receiptString origin:origin];
                break;
            }
            case SKPaymentTransactionStateRestored: {
                NSLog(@"{billing} Ignoring restored transaction for sku=%@ and token=%@", sku, token);
                [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
                break;
            }
            case SKPaymentTransactionStatePurchasing: {
                NSLog(@"{billing} Transaction purchasing for sku=%@ and token=%@", sku, token);
                break;
            }
            case SKPaymentTransactionStateFailed: {
                NSLog(@"{billing} Transaction failed with error code %d(%@) for sku=%@ and token=%@", (int)transaction.error.code, transaction.error.localizedDescription, sku, token);
                [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
                break;
            }
            default: {
                NSLog(@"{billing} Ignoring unknown transaction state %ld: error=%d for sku=%@ and token=%@", transaction.transactionState, (int)transaction.error.code, sku, token);
                break;
            }
        }
    }
}

- (BOOL)paymentQueue:(SKPaymentQueue *)queue shouldAddStorePayment:(SKPayment *)payment forProduct:(SKProduct *)product {
    self.storeOrigin = payment;
    return YES;
}

- (void) requestPurchase:(NSString *)productIdentifier {
    // This is done exclusively to set up an SKPayment object with the result (it will initiate a purchase)
    NSString *bundledProductId = [self.bundleID stringByAppendingFormat:@".%@", productIdentifier];

    // Create a set with the given identifier
    NSSet *productIdentifiers = [NSSet setWithObjects:productIdentifier,bundledProductId,nil];

    // Create a products request
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = [IAPPlugin getInstance];

    // Kick it off!
    [productsRequest start];
}

- (void) initializePlugin {
    @try {
        NSString *bundleID = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleIdentifier"];

        [IAPPlugin getInstance].bundleID = bundleID;
        NSLog(@"{billing} Initialized with manifest bundleID: '%@'", bundleID);

    }
    @catch (NSException *exception) {
        NSLog(@"{billing} Failure to get ios:bundleID from manifest file: %@", exception);
    }
}

-(void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue{

    if([[[SKPaymentQueue defaultQueue] transactions] count]==0) {
        // not bought
        modules::GameManager::getInstance()->setGameOwned(false);
        NSLog(@"{billing} not bought");
    } else
        for (SKPaymentTransaction *transaction in [[SKPaymentQueue defaultQueue] transactions]){

            if (![transaction.payment.productIdentifier isEqualToString:kBuyGameProductId]){
                modules::GameManager::getInstance()->setGameOwned(true);
                break;
            }
        }
}


+ (void) initialize {
    BOOL isMarketAvailable = [SKPaymentQueue canMakePayments];
    modules::GameManager::getInstance()->setBillingConnected(isMarketAvailable);
    if (isMarketAvailable) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    }
}

+ (void) purchase:(NSString *)data {
    NSString *sku = data;

    @try {
        [[IAPPlugin getInstance] requestPurchase:sku];
    }
    @catch (NSException *exception) {
        NSLog(@"{billing} WARNING: Unable to purchase item: %@", exception);
    }
}

@end
