#include <iostream>
#include "pricing/DiscountPricing.h"
#include "core/CentralRegistry.h"

DiscountPricing::DiscountPricing(double rate) : discountRate(rate) {}

double DiscountPricing::computePrice(Item* item) {
    // Read live discount rate from CentralRegistry if available
    std::string rateStr = CentralRegistry::getInstance()->getConfig("discountRate");
    if (rateStr != "Not Found") {
        discountRate = std::stod(rateStr);
    }

    double original   = item->getPrice();
    double discounted = original * (1.0 - discountRate);

    std::cout << "[DiscountPricing] " << item->getName()
              << ": Rs." << original
              << " -> Rs." << discounted
              << " (" << discountRate * 100 << "% off)" << std::endl;
    return discounted;
}