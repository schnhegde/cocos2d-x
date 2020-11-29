#include "PlatformIncludes.h"
#include <GoogleMobileAds/GADMobileAds.h>
#import "../proj.ios_mac/ios/RootViewController.h"


bool isRewardedAdAvailable() {
    RootViewController *controller = [UIApplication sharedApplication].keyWindow.rootViewController;
    return [controller isRewardedAdLoaded]; //[Appodeal isReadyForShowWithStyle: AppodealShowStyleRewardedVideo];
}

void initAds() {
    RootViewController *controller = [UIApplication sharedApplication].keyWindow.rootViewController;
    [[GADMobileAds sharedInstance] startWithCompletionHandler:nil];
    
    [controller loadRewardedVideo];
}

void showRewardedAd() {
    RootViewController *controller = [UIApplication sharedApplication].keyWindow.rootViewController;
//    [Appodeal showAd:AppodealShowStyleRewardedVideo rootViewController:controller];
    [controller showRewardedVideo];
}

void openFeedbackEmail() {
    
    RootViewController *controller = [UIApplication sharedApplication].keyWindow.rootViewController;
    [controller emailFeedback];
    
}

void purchase() {
    
}

bool isTablet() {
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
    {
        return true; /* Device is iPad */
    } else {
        return false;
    }
}

std::string getVersionNumber() {
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    
    return [[NSString stringWithFormat:@"%@", [infoDictionary objectForKey:@"CFBundleShortVersionString"]] cStringUsingEncoding:NSUTF8StringEncoding];
}
