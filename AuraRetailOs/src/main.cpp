#include <iostream>
#include <string>

#include "inventory/Product.h"
#include "inventory/Inventory.h"
#include "payment/Payment.h"
#include "payment/CardAdapter.h"
#include "payment/UPIAdapter.h"
#include "pricing/PricingPolicy.h"
#include "pricing/StandardPricing.h"
#include "pricing/DiscountPricing.h"

// Helper: perform a purchase using given pricing strategy and payment method
void performPurchase(Inventory& inv, const std::string& itemId,
                     PricingPolicy* pricing, Payment* payment) {
    std::cout << "\n--- Purchase Request: " << itemId << " ---" << std::endl;

    Item* item = inv.getItem(itemId);
    if (!item) {
        std::cout << "[ERROR] Item not found: " << itemId << std::endl;
        return;
    }
    if (item->getStock() <= 0) {
        std::cout << "[ERROR] Out of stock: " << item->getName() << std::endl;
        return;
    }

    // Strategy Pattern: compute price using selected strategy
    double finalPrice = pricing->computePrice(item);

    // Adapter Pattern: pay using selected payment method
    bool success = payment->pay(finalPrice);

    if (success) {
        inv.reduceStock(itemId);
        std::cout << "[Kiosk] Purchase SUCCESS. Remaining stock: "
                  << item->getStock() << std::endl;
    } else {
        std::cout << "[Kiosk] Purchase FAILED." << std::endl;
    }
}

int main() {
    std::cout << "========== Aura Retail OS - Subtask 2 Simulation ==========" << std::endl;

    // --- Setup Inventory ---
    Inventory inventory;
    inventory.addItem(new Product("P001", "Mineral Water", 20.0, 5));
    inventory.addItem(new Product("P002", "Chocolate Bar", 50.0, 3));
    inventory.addItem(new Product("P003", "Aspirin Pack", 120.0, 2));

    // --- Setup Pricing Strategies (Strategy Pattern) ---
    StandardPricing standard;
    DiscountPricing discount(0.15);   // 15% off

    // --- Setup Payment Adapters (Adapter Pattern) ---
    CardAdapter cardPay;
    UPIAdapter upiPay;

    // Purchase 1: Water at standard price, paid by Card
    performPurchase(inventory, "P001", &standard, &cardPay);

    // Purchase 2: Chocolate with 15% discount, paid by UPI
    performPurchase(inventory, "P002", &discount, &upiPay);

    // Purchase 3: Aspirin at standard price, paid by UPI
    performPurchase(inventory, "P003", &standard, &upiPay);

    // Purchase 4: Try out-of-stock item (Aspirin, only 2 in stock)
    performPurchase(inventory, "P003", &standard, &cardPay);
    performPurchase(inventory, "P003", &standard, &cardPay); // should fail

    std::cout << "\n========== Simulation Complete ==========" << std::endl;
    return 0;
}