#include <iostream>
#include "core/Kiosk.h"
#include "hardware/Dispenser.h"
#include "payment/Payment.h"
#include "inventory/Inventory.h"
#include "inventory/Item.h"
#include "pricing/PricingPolicy.h"

Kiosk::Kiosk(Dispenser* d, Payment* p, Inventory* i, PricingPolicy* pr)
    : dispenser(d), payment(p), inventory(i), pricingPolicy(pr) {}

Dispenser* Kiosk::getDispenser() { return dispenser; }
Payment* Kiosk::getPayment() { return payment; }
Inventory* Kiosk::getInventory() { return inventory; }
PricingPolicy* Kiosk::getPricingPolicy() { return pricingPolicy; }

void Kiosk::purchaseItem(const std::string& productId, const std::string& paymentMethod) {
    std::cout << "[Kiosk] purchaseItem called for product=" << productId
              << " payment=" << paymentMethod << std::endl;

    if (inventory->getStock(productId) <= 0) {
        std::cout << "[Kiosk] Product out of stock.\n";
        return;
    }

    Item* item = inventory->getItem(productId);
    if (!item) {
        std::cout << "[Kiosk] Product not found.\n";
        return;
    }

    double price = pricingPolicy->computePrice(item);
    std::cout << "[Kiosk] Computed price: Rs." << price << std::endl;

    if (payment->pay(price)) {
        dispenser->dispense(productId);
        inventory->updateStock(productId, -1);
        std::cout << "[Kiosk] Purchase SUCCESS for " << productId << std::endl;
    } else {
        std::cout << "[Kiosk] Payment FAILED.\n";
    }
}

void Kiosk::refundTransaction(const std::string& transactionId) {
    std::cout << "[Kiosk] Refund for transactionId=" << transactionId << std::endl;
    if (payment->refund(transactionId)) {
        std::cout << "[Kiosk] Refund SUCCESS.\n";
    } else {
        std::cout << "[Kiosk] Refund FAILED.\n";
    }
}

void Kiosk::restockInventory(const std::string& productId, int qty) {
    inventory->updateStock(productId, qty);
    std::cout << "[Kiosk] Restocked " << productId << " by " << qty << std::endl;
}

void Kiosk::runDiagnostics() {
    std::cout << "[Diagnostics] Dispenser: "
              << (dispenser->isReady() ? "READY" : "FAULT")
              << " | Payment: " << payment->getStatus()
              << " | Inventory: online" << std::endl;
}