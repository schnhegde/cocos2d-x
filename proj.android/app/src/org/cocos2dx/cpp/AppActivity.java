/****************************************************************************
Copyright (c) 2015-2016 Chukong Technologies Inc.
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
package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.os.Build;
import android.util.Log;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

import androidx.annotation.NonNull;

import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdCallback;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;


public class AppActivity extends Cocos2dxActivity {

    static String rewardedId = "ca-app-pub-3940256099942544/5224354917";
    static RewardedAd rewardedAd = null;
    static RewardedAdCallback rewardedAdCallback = null;
    static AppActivity instance;
    static bool isRewarding = false;

    public static native void rewardVideoWatchCompleted();
    public static native void rewardVideoCancelled();
    public static native void rewardVideoFailedToShow();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // Make sure we're running on Pie or higher to change cutout mode
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            // Enable rendering into the cutout area
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }
        // DO OTHER INITIALIZATION BELOW
        instance = this;
        initAds();
    }

    static void loadRewardedAd() {
        RewardedAdLoadCallback adLoadCallback = new RewardedAdLoadCallback() {
            @Override
            public void onRewardedAdLoaded() {
                // Ad successfully loaded.
//                AppActivity.showRewardedAd();
            }

           @Override
            public void onRewardedAdFailedToLoad(LoadAdError adError) {
                // Ad failed to load.
               Log.d("{sachin}", "failed to load ad");
            }
        };
        rewardedAd.loadAd(new AdRequest.Builder().build(), adLoadCallback);
    }

    public static bool isRewardedAdLoaded() {
        return rewardedAd.isLoaded();
    }

    public static void showRewardedAd() {
        if (rewardedAd.isLoaded()) {
            Activity activityContext = instance;
            RewardedAdCallback adCallback = new RewardedAdCallback() {
                @Override
                public void onRewardedAdOpened() {
                    // Ad opened.
                }

                @Override
                public void onRewardedAdClosed() {
                    // Ad closed.
                    if (isRewarding) {
                        rewardVideoWatchCompleted();
                    } else {
                        rewardVideoCancelled();
                    }
                    isRewarding = false;
                    loadRewardedAd();
                }

                @Override
                public void onUserEarnedReward(@NonNull RewardItem reward) {
                    // User earned reward.
                    isRewarding = true;
                }

                @Override
                public void onRewardedAdFailedToShow(AdError adError) {
                    // Ad failed to display.
                    rewardVideoCancelled();
                }
            };
            rewardedAd.show(activityContext, adCallback);
        } else {
            Log.d("TAG", "The rewarded ad wasn't loaded yet.");
        }

    }

    public static void initAds() {
        MobileAds.initialize(instance, new OnInitializationCompleteListener() {
            @Override
            public void onInitializationComplete(InitializationStatus initializationStatus) {
                rewardedAd = new RewardedAd(instance, rewardedId);
                loadRewardedAd();
            }
        });
    }

}
