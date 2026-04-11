#ifndef STANDARDPRICING_H
#define STANDARDPRICING_H
#include "pricing/PricingPolicy.h"

// Concrete Strategy: no discount, full price
class StandardPricing : public PricingPolicy {
public:
    double computePrice(Item* item) override;
};
#endif