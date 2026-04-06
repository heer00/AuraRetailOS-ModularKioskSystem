#ifndef KIOSKFACTORY_H
#define KIOSKFACTORY_H

// Forward declarations
class Dispenser;
class Payment;
class Inventory;
class PricingPolicy;

class KioskFactory {
public:
    virtual Dispenser* createDispenser() = 0;
    virtual Payment* createPayment() = 0;
    virtual Inventory* createInventory() = 0;
    virtual PricingPolicy* createPricingPolicy() = 0;

    virtual ~KioskFactory() {}
};

#endif