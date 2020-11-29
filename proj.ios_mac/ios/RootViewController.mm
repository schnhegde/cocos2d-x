/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#include "EventUtils.h"
#include "Events.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "IAPPlugin.h"

@interface RootViewController () <GADRewardedAdDelegate>
@end

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: cocos2d::GLViewImpl::_multisamplingCount > 0 ? YES : NO
                                     numberOfSamples: cocos2d::GLViewImpl::_multisamplingCount ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
    self.rewardedAdLoaded = false;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    [[IAPPlugin getInstance] initializePlugin];
}

-(void) loadRewardedVideo {
    self.rewardedAd = [[GADRewardedAd alloc]
                          initWithAdUnitID:@"ca-app-pub-2853844821432281/2495466249"];
    GADRequest *request = [GADRequest request];
     [self.rewardedAd loadRequest:request completionHandler:^(GADRequestError * _Nullable error) {
       if (error) {
           self.rewardedAdLoaded = false;
       } else {
           self.rewardedAdLoaded = true;
       }
     }];
}

-(void) showRewardedVideo {
    if (self.rewardedAd.isReady) {
        [self.rewardedAd presentFromRootViewController:self delegate:self];
      } else {
        NSLog(@"Ad wasn't ready");
      }
}

/// Tells the delegate that the user earned a reward.
- (void)rewardedAd:(GADRewardedAd *)rewardedAd userDidEarnReward:(GADAdReward *)reward {
  // TODO: Reward the user.
    self.rewardEarned = true;
}

/// Tells the delegate that the rewarded ad was presented.
- (void)rewardedAdDidPresent:(GADRewardedAd *)rewardedAd {
    self.rewardedAdLoaded = false;
}

/// Tells the delegate that the rewarded ad failed to present.
- (void)rewardedAd:(GADRewardedAd *)rewardedAd didFailToPresentWithError:(NSError *)error {
    self.rewardedAdLoaded = false;
    modules::EventUtils::dispatchEvent(data::Events::REWARDED_VIDEO_CANCELLED);
}

/// Tells the delegate that the rewarded ad was dismissed.
- (void)rewardedAdDidDismiss:(GADRewardedAd *)rewardedAd {
    if (self.rewardEarned) {
        modules::EventUtils::dispatchEvent(data::Events::REWARDED_VIDEO_COMPLETED);
        self.rewardEarned = false;
    } else {
        modules::EventUtils::dispatchEvent(data::Events::REWARDED_VIDEO_CANCELLED);
    }
    [self loadRewardedVideo];
}

-(BOOL) isRewardedAdLoaded {
    return self.rewardedAdLoaded;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}

- (NSString*) getDeviceInfo {
    NSString* deviceInfo;
    NSMutableDictionary* deviceObject = [[NSMutableDictionary alloc]init];
    [deviceObject setValue:@"ios" forKey:@"type"];
    [deviceObject setValue:@"ios" forKey:@"store"];
    NSString * version =  [[[NSBundle mainBundle] infoDictionary]
                           objectForKey: @"CFBundleShortVersionString"];
    NSError * error;
    NSURL* urlToDocumentsFolder =
    [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
                                            inDomains:NSUserDomainMask] lastObject];
    NSDate *installDate = [[[NSFileManager defaultManager]
                            attributesOfItemAtPath:urlToDocumentsFolder.path error:&error]
                           objectForKey:NSFileCreationDate];
    NSString *m_platform = 0;
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size + 1);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    machine[size] = '\0';
    m_platform = [NSString stringWithUTF8String:machine];
    [deviceObject setValue:[NSNumber numberWithDouble:[installDate
                                                       timeIntervalSince1970] * 1000] forKey:@"installDate"];
    [deviceObject setValue:version forKey:@"versionNumber"];
    [deviceObject setValue:m_platform forKey:@"device"];
    [deviceObject setValue:[[UIDevice currentDevice] systemVersion] forKey:@"os"];
    deviceInfo = [self getJsonString:deviceObject];

    return deviceInfo;
}

- (NSString*) getJsonString: (NSDictionary*)dict{
    NSError* error = nil;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:dict
                                                       options:(NSJSONWritingOptions)0
                                                         error:&error];
    if (error) {
        return nil;
    }
    NSString* serializedJson = [[NSString alloc] initWithData:jsonData
                                                     encoding:NSUTF8StringEncoding];
    return serializedJson;
}

- (void)emailFeedback {
    NSString* dontRemove = @"   ***** DO NOT REMOVE THIS *****  ";

    NSString *emailBody = [NSString stringWithFormat:@"%@%@%@", dontRemove, [self getDeviceInfo], dontRemove];
    if ([MFMailComposeViewController canSendMail])
    {
        MFMailComposeViewController *mail = [[MFMailComposeViewController alloc] init];
        mail.mailComposeDelegate = self;

        [mail setSubject:@"\"Unblock\" game user feedback"];
        [mail setMessageBody:emailBody isHTML:NO];
        [mail setToRecipients:@[@"support@hashcube.freshdesk.com"]];
        UIViewController *top = [UIApplication sharedApplication].keyWindow.rootViewController;
        [top presentViewController:mail animated:YES completion:nil];
    }
    else
    {
        NSString *recipients = @"mailto:sokoban.feedback@gmail.com?subject=\"Sokoban \" feedback";
        NSString *bodyDefaults= @"&body=";
        NSString *body = [NSString stringWithFormat:@"%@%@", bodyDefaults, emailBody];
        NSString *email = [NSString stringWithFormat:@"%@%@", recipients, body];
        email = [email stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:email] options:@{} completionHandler:^(BOOL success) {
        }];
    }
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger) supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskAllButUpsideDown;
}
#endif

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

// Controls the application's preferred home indicator auto-hiding when this view controller is shown.
// (better use preferredScreenEdgesDeferringSystemGestures for controlling the home indicator)
- (BOOL)prefersHomeIndicatorAutoHidden {
    return NO;
}

// HOME Indicator need to be tapped twice 
-(UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
    return UIRectEdgeBottom; 
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(nullable NSError *)error {

    switch (result) {
        case MFMailComposeResultCancelled:
            NSLog(@"Mail cancelled");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"Mail saved");
            break;
        case MFMailComposeResultSent:
            NSLog(@"Mail sent");
            break;
        case MFMailComposeResultFailed:
            NSLog(@"Mail sent failure: %@",error.description);
            break;
    }
    NSLog(@"Mail dismiss");

    // Dismiss the mail compose view controller.
    [controller dismissViewControllerAnimated:true completion:nil];

}


@end
