#include <iostream>
#include "core/Kiosk.h"
#include "inventory/Item.h"

Kiosk::Kiosk(Inventory* i, Payment* p, Dispenser* d, PricingPolicy* pr)
    : inventory(i), payment(p), dispenser(d), pricing(pr) {}

void Kiosk::purchaseItem(const std::string& productId) {

    std::cout << "\n[Kiosk] Purchase request: " << productId << std::endl;

    int stock = inventory->getStock(productId);
    if (stock <= 0) {
        std::cout << "[Kiosk] FAILED: Out of stock\n";
        return;
    }

    Item* item = inventory->getItem(productId);
    double price = pricing->computePrice(item);

    if (!payment->pay(price)) {
        std::cout << "[Kiosk] FAILED: Payment error\n";
        return;
    }

    dispenser->dispense(productId);
    inventory->updateStock(productId, stock - 1);

    std::cout << "[Kiosk] SUCCESS\n";
}