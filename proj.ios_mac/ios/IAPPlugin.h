#ifndef IAPPlugin_h
#define IAPPlugin_h

#import <StoreKit/StoreKit.h>

@interface IAPPlugin : NSObject<SKPaymentTransactionObserver, SKProductsRequestDelegate>

@property (nonatomic, retain) NSMutableDictionary *purchases;
@property (nonatomic, retain) NSString *bundleID;
@property (nonatomic, retain) NSMutableDictionary *productDetails;
@property (nonatomic, retain) SKPayment *storeOrigin;

+ (IAPPlugin*) getInstance;
- (void) initializePlugin;
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void) requestPurchase:(NSString *)productIdentifier;
+ (void) initialize;
+ (void) purchase:(NSString *)jsonObject;
+ (void) getPurchases:(NSDictionary *)jsonObject;

@end
#endif /* HCBillingPlugin_h */
