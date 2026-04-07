#include "core/KioskBuilder.h"
#include "core/Kiosk.h"

KioskBuilder::KioskBuilder() {
    dispenser = nullptr;
    payment = nullptr;
    inventory = nullptr;
    pricingPolicy = nullptr;
}

KioskBuilder& KioskBuilder::addDispenser(Dispenser* d) {
    dispenser = d;
    return *this;
}

KioskBuilder& KioskBuilder::addPayment(Payment* p) {
    payment = p;
    return *this;
}

KioskBuilder& KioskBuilder::addInventory(Inventory* i) {
    inventory = i;
    return *this;
}

KioskBuilder& KioskBuilder::addPricingPolicy(PricingPolicy* pr) {
    pricingPolicy = pr;
    return *this;
}

Kiosk* KioskBuilder::build() {
    return new Kiosk(dispenser, payment, inventory, pricingPolicy);
}