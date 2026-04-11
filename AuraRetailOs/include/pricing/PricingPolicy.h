#ifndef PRICINGPOLICY_H
#define PRICINGPOLICY_H
#include "inventory/Item.h"

// STRATEGY interface for pricing
class PricingPolicy {
public:
    virtual double computePrice(Item* item) = 0;
    virtual ~PricingPolicy() {}
};
#endif