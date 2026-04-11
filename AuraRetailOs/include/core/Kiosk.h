#ifndef KIOSK_H
#define KIOSK_H

#include "core/KioskInterface.h"
#include "inventory/Inventory.h"
#include "payment/Payment.h"
#include "hardware/Dispenser.h"
#include "pricing/PricingPolicy.h"

class Kiosk : public KioskInterface {
private:
    Inventory* inventory;
    Payment* payment;
    Dispenser* dispenser;
    PricingPolicy* pricing;

public:
    Kiosk(Inventory* i, Payment* p, Dispenser* d, PricingPolicy* pr);

    void purchaseItem(const std::string& productId) override;
};

#endif