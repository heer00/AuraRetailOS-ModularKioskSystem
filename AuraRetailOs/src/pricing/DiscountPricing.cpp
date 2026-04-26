#include <iostream>
#include "pricing/DiscountPricing.h"
#include "core/CentralRegistry.h"

DiscountPricing::DiscountPricing(double rate) : discountRate(rate) {}

double DiscountPricing::computePrice(Item* item) {
    // Read live discount rate from CentralRegistry
    // Guard: only use registry value if it is non-empty and not "Not Found"
    std::string rateStr = CentralRegistry::getInstance()->getConfig("discountRate");
    if (!rateStr.empty() && rateStr != "Not Found") {
        try {
            discountRate = std::stod(rateStr);
        } catch (...) {
            // Malformed value in config — keep constructor default
        }
    }

    double original   = item->getPrice();
    double discounted = original * (1.0 - discountRate);

    std::cout << "[DiscountPricing] " << item->getName()
              << ": Rs." << original
              << " -> Rs." << discounted
              << " (" << discountRate * 100 << "% off)" << std::endl;
    return discounted;
}