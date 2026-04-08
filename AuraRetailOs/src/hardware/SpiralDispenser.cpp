#include <iostream>
#include "hardware/SpiralDispenser.h"

SpiralDispenser::SpiralDispenser() : ready(false) {}

void SpiralDispenser::calibrate() {
    ready = true;
    std::cout << "[SpiralDispenser] Coil calibrated." << std::endl;
}

bool SpiralDispenser::isReady() const {
    return ready;
}

bool SpiralDispenser::dispense(const std::string& productId) {
    if (!ready) calibrate();
    std::cout << "[SpiralDispenser] Rotating coil -- dispensing product: " << productId << std::endl;
    return true;
}
