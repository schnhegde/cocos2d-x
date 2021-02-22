package org.cocos2dx.cpp;
// Security

import android.app.Activity;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ConsumeParams;
import com.android.billingclient.api.ConsumeResponseListener;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchaseHistoryRecord;
import com.android.billingclient.api.PurchaseHistoryResponseListener;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.android.billingclient.api.SkuDetailsResponseListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class IAPPlugin implements PurchasesUpdatedListener, BillingClientStateListener, SkuDetailsResponseListener, PurchaseHistoryResponseListener {
    public static native void nativePurchaseEvent(boolean purchased);
    public static native void nativeOwnedEvent(boolean owned);
    public static native void nativeConnectedEvent(boolean connected);

    private static BillingClient client;
    private static Activity ctx;
    private static List<String> skuList = new ArrayList<>();
    private static HashMap<String, SkuDetails> skudetails = new HashMap<>();

    void initialize(Activity activity) {
        ctx = activity;
        client = BillingClient.newBuilder(activity)
                .setListener(this)
                .enablePendingPurchases()
                .build();
        client.startConnection(this);

    }

    void handlePurchase(Purchase purchase) {
        nativePurchaseEvent(true);
    }

    int purchase(String sku) {
        BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                .setSkuDetails(skudetails.get(sku))
                .build();
        return client.launchBillingFlow(ctx, billingFlowParams).getResponseCode();
    }

    void refreshProducts() {
        skuList.add("buy_game");
        SkuDetailsParams.Builder params = SkuDetailsParams.newBuilder();
        params.setSkusList(skuList).setType(BillingClient.SkuType.INAPP);
        client.querySkuDetailsAsync(params.build(), this);
        client.queryPurchaseHistoryAsync(BillingClient.SkuType.INAPP, this);
    }

    @Override
    public void onPurchasesUpdated(@NonNull BillingResult billingResult, @Nullable List<Purchase> purchases) {
        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK
                && purchases != null) {
            for (Purchase purchase : purchases) {
                handlePurchase(purchase);
            }
        } else if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.USER_CANCELED) {
            // Handle an error caused by a user cancelling the purchase flow.
        } else {
            // Handle any other error codes.
        }
    }

    @Override
    public void onBillingSetupFinished(@NonNull BillingResult billingResult) {
        if (billingResult.getResponseCode() ==  BillingClient.BillingResponseCode.OK) {
            refreshProducts();
            nativeConnectedEvent(true);
        } else {
            nativeConnectedEvent(false);
        }
    }

    @Override
    public void onBillingServiceDisconnected() {
        nativeConnectedEvent(false);
        client.startConnection(this);
    }

    @Override
    public void onSkuDetailsResponse(BillingResult billingResult,
                                     List<SkuDetails> skuDetailsList) {
        if (skuDetailsList != null) {
            for(int i = 0; i < skuDetailsList.size(); i++) {
                SkuDetails detail = skuDetailsList.get(i);
                String sku = detail.getSku();
                skudetails.put(sku, detail);
            }
        }
    }

    @Override
    public void onPurchaseHistoryResponse(@NonNull BillingResult billingResult, @Nullable List<PurchaseHistoryRecord> list) {
        if (list != null && list.size() > 0) {
            String h = list.get(0).getSku();
            int g = h.compareTo("buy_game");
            if (g == 0) {
                nativeOwnedEvent(true);
            }
        }
    }
}
