#ifndef FOODFACTORY_H
#define FOODFACTORY_H

#include "factory/KioskFactory.h"

class FoodFactory : public KioskFactory {
public:
    Dispenser* createDispenser() override;
    Payment* createPayment() override;
    Inventory* createInventory() override;
    PricingPolicy* createPricingPolicy() override;
};

#endif