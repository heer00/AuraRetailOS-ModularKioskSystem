#include <iostream>
#include "hardware/RoboticArmDispenser.h"

RoboticArmDispenser::RoboticArmDispenser() : ready(false) {}

void RoboticArmDispenser::calibrate() {
    ready = true;
    std::cout << "[RoboticArmDispenser] Arm calibrated and zeroed." << std::endl;
}

bool RoboticArmDispenser::isReady() const {
    return ready;
}

bool RoboticArmDispenser::dispense(const std::string& productId) {
    if (!ready) calibrate();
    std::cout << "[RoboticArmDispenser] Arm picking and placing product: " << productId << std::endl;
    return true;
}
