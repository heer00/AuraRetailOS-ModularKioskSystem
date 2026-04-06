#ifndef KIOSK_H
#define KIOSK_H

#include <string>
#include "KioskInterface.h"

// Forward declarations
class Dispenser;
class Payment;
class Inventory;
class PricingPolicy;

class Kiosk : public KioskInterface {
private:
    Dispenser* dispenser;
    Payment* payment;
    Inventory* inventory;
    PricingPolicy* pricingPolicy;

public:
    Kiosk(Dispenser* d, Payment* p, Inventory* i, PricingPolicy* pr);

    void purchaseItem(const std::string& productId) override;
    void refundTransaction(const std::string& transactionId) override;
    void restockInventory(const std::string& productId, int qty) override;
    void runDiagnostics() override;
};

#endif