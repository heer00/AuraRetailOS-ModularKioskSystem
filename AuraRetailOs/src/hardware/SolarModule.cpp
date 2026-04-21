// SolarModule.cpp
#include <iostream>
#include "hardware/SolarModule.h"

SolarModule::SolarModule(KioskInterface* kiosk, int battery)
    : KioskModule(kiosk),      // must be 'kiosk', never 'this'
      batteryLevel(battery) {}

void SolarModule::setBatteryLevel(int level) {
    batteryLevel = level;
}

int SolarModule::getBatteryLevel() const {
    return batteryLevel;
}

void SolarModule::runDiagnostics() {
    std::cout << "[SolarModule] Battery: " << batteryLevel << "%" << std::endl;
    if (batteryLevel < 20) {
        std::cout << "[SolarModule] WARNING -- low battery, kiosk restricted.\n";
    }
    wrappedKiosk->runDiagnostics();   // always forward, even when low battery
}

void SolarModule::purchaseItem(const std::string& productId,
                               const std::string& paymentMethod) {
    if (batteryLevel < 20) {
        std::cout << "[SolarModule] BLOCKED -- battery too low ("
                  << batteryLevel << "%).\n";
        return;
    }
    wrappedKiosk->purchaseItem(productId, paymentMethod);
}