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
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;

import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.RequestConfiguration;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdCallback;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;
import com.vinaykumar.sokoban.BuildConfig;
import com.vinaykumar.sokoban.R;

import java.util.Arrays;
import java.util.Date;
import java.util.List;


public class AppActivity extends Cocos2dxActivity {

    static String rewardedId = "ca-app-pub-2853844821432281/8229489719";
    static RewardedAd rewardedAd = null;
    static RewardedAdCallback rewardedAdCallback = null;
    static AppActivity instance;
    static boolean isRewarding = false;
    static boolean isAdLoaded = false;

    public static native void rewardVideoWatchCompleted();
    public static native void rewardVideoCancelled();
    public static native void rewardVideoFailedToShow();
    static IAPPlugin iapPlugin;

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
        iapPlugin = new IAPPlugin();
        iapPlugin.initialize(this);
        initAds();
    }

    static void loadRewardedAd() {
        instance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                rewardedAd = new RewardedAd(instance, rewardedId);
                RewardedAdLoadCallback adLoadCallback = new RewardedAdLoadCallback() {
                    @Override
                    public void onRewardedAdLoaded() {
                        // Ad successfully loaded.
                        isAdLoaded = true;
                    }

                    @Override
                    public void onRewardedAdFailedToLoad(LoadAdError adError) {
                        // Ad failed to load.
                        isAdLoaded = false;
                    }
                };
                rewardedAd.loadAd(new AdRequest.Builder().build(), adLoadCallback);
            }
        });
    }

    public static void purchase() {
        iapPlugin.purchase("buy_game");
    }

    public static String getDeviceInfo() {
        String device_info = null;
        JSONObject device_info_data = new JSONObject();
        Context appContext = Cocos2dxActivity.getContext();
        String install_time = "";
        try {
            PackageInfo packageInfo = appContext.getPackageManager().getPackageInfo(appContext.getPackageName(), 0);
            install_time = "" + new Date(packageInfo.firstInstallTime).toGMTString();
        } catch (PackageManager.NameNotFoundException e) {
            Log.d("{sokoban_deviceInfo}",e.getLocalizedMessage());
        }
        try {
            device_info_data.put("install_time", install_time);
            device_info_data.put("platform", "android");
            device_info_data.put("device", Build.MANUFACTURER + " " + Build.MODEL);
            device_info_data.put("os_version", Build.VERSION.RELEASE);

            device_info = device_info_data.toString();

        } catch (Exception e) {
            Log.d("{sokoban_deviceInfo}",e.getLocalizedMessage());
            return device_info_data.toString();
        }
        return device_info;
    }

    public static void openFeedbackEmail() {
        instance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Intent intent = new Intent(Intent.ACTION_SEND);
                intent.setType("message/rfc822");
                intent.putExtra(android.content.Intent.EXTRA_EMAIL, new String[]{instance.getResources().getString(R.string.mail_to)});
                intent.putExtra(android.content.Intent.EXTRA_SUBJECT, instance.getResources().getString(R.string.feedback_email_dialog_header));
                intent.putExtra(android.content.Intent.EXTRA_TEXT, "*DO NOT REMOVE THIS INFORMATION* " + getDeviceInfo() + " *DO NOT REMOVE THIS INFORMATION*");
                instance.startActivity(Intent.createChooser(intent, instance.getResources().getString(R.string.feedback_email_dialog_header)));
            }
        });
    }

    public static boolean isRewardedAdAvailable() {
        return isAdLoaded;
    }

    public static void showRewardedAd() {
        instance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
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
                            if (AppActivity.isRewarding) {
                                rewardVideoWatchCompleted();
                            } else {
                                rewardVideoCancelled();
                            }
                            AppActivity.isRewarding = false;
                            AppActivity.isAdLoaded = false;
                            loadRewardedAd();
                        }

                        @Override
                        public void onUserEarnedReward(@NonNull RewardItem reward) {
                            // User earned reward.
                            AppActivity.isRewarding = true;
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
        });
    }

    public static boolean isTablet() {
        DisplayMetrics metrics = new DisplayMetrics();
        instance.getWindowManager().getDefaultDisplay().getMetrics(metrics);

        float yInches= metrics.heightPixels/metrics.ydpi;
        float xInches= metrics.widthPixels/metrics.xdpi;
        double diagonalInches = Math.sqrt(xInches*xInches + yInches*yInches);
        if (diagonalInches>=6.5){
            return true;
        }else{
            return false;
        }
    }

    public static String getVersionNumber() {
        return "v" + BuildConfig.VERSION_NAME;
    }

    public static void initAds() {
        instance.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                MobileAds.initialize(instance, new OnInitializationCompleteListener() {
                    @Override
                    public void onInitializationComplete(InitializationStatus initializationStatus) {
                        List<String> testDeviceIds = Arrays.asList("EAE7CE0472F02F7A2DE96560259168BB");
                        RequestConfiguration configuration =
                                new RequestConfiguration.Builder().setTestDeviceIds(testDeviceIds).build();
                        MobileAds.setRequestConfiguration(configuration);
                        rewardedAd = new RewardedAd(instance, rewardedId);
                        loadRewardedAd();
                    }
                });
            }
        });

    }

}
