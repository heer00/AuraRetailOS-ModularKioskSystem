#ifndef PHARMACYFACTORY_H
#define PHARMACYFACTORY_H

#include "KioskFactory.h"

class PharmacyFactory : public KioskFactory {
public:
    Dispenser* createDispenser() override;
    Payment* createPayment() override;
    Inventory* createInventory() override;
    PricingPolicy* createPricingPolicy() override;
};

#endif