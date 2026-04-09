#include <iostream>
#include "pricing/DiscountPricing.h"

DiscountPricing::DiscountPricing(double rate) : discountRate(rate) {}

double DiscountPricing::computePrice(Item* item) {
    double original = item->getPrice();
    double discounted = original * (1 - discountRate);

    std::cout << "[DiscountPricing] " << item->getName()
              << ": Rs." << original
              << " -> Rs." << discounted
              << " (" << discountRate * 100 << "% off)" << std::endl;

    return discounted;
}
