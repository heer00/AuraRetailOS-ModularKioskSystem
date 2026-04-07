#include <iostream>
#include "factory/FoodFactory.h"
#include "hardware/SpiralDispenser.h"
#include "payment/UPIAdapter.h"
#include "inventory/RealInventory.h"
#include "inventory/InventoryProxy.h"
#include "inventory/Product.h"
#include "pricing/DiscountPricing.h"

Dispenser* FoodFactory::createDispenser() {
    std::cout << "[FoodFactory] Creating SpiralDispenser\n";
    return new SpiralDispenser();
}

Payment* FoodFactory::createPayment() {
    std::cout << "[FoodFactory] Creating UPIAdapter\n";
    return new UPIAdapter();
}

Inventory* FoodFactory::createInventory() {
    std::cout << "[FoodFactory] Creating Food Inventory\n";
    RealInventory* real = new RealInventory();
    real->addItem(new Product("FOOD001", "Chips Pack", 20, 100));
    return new InventoryProxy(real, "food-system");
}

PricingPolicy* FoodFactory::createPricingPolicy() {
    std::cout << "[FoodFactory] Creating DiscountPricing\n";
    return new DiscountPricing(10);
}