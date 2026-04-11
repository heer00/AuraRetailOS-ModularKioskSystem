#include <iostream>
#include "pricing/StandardPricing.h"

double StandardPricing::computePrice(Item* item) {
    double price = item->getPrice();
    std::cout << "[StandardPricing] Price for " << item->getName()
              << ": Rs." << price << std::endl;
    return price;
}