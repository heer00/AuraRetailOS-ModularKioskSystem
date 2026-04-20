// NetworkModule.cpp
#include <iostream>
#include "hardware/NetworkModule.h"

NetworkModule::NetworkModule(KioskInterface* kiosk, bool offline)
    : KioskModule(kiosk),     // must be 'kiosk', never 'this'
      offline(offline) {}

void NetworkModule::setOffline(bool state) {
    offline = state;
}

bool NetworkModule::isOffline() const {
    return offline;
}

void NetworkModule::runDiagnostics() {
    std::cout << "[NetworkModule] Status: "
              << (offline ? "OFFLINE" : "ONLINE") << std::endl;
    if (!queue.empty()) {
        std::cout << "[NetworkModule] Queued transactions: "
                  << queue.size() << "\n";
    }
    wrappedKiosk->runDiagnostics();   // forward down the chain
}

void NetworkModule::purchaseItem(const std::string& productId,
                                 const std::string& paymentMethod) {
    if (offline) {
        queue.push_back({productId, paymentMethod});
        std::cout << "[NetworkModule] OFFLINE -- queued: "
                  << productId << "\n";
        return;
    }
    wrappedKiosk->purchaseItem(productId, paymentMethod);
}

void NetworkModule::flushQueue() {
    std::cout << "[NetworkModule] Flushing " << queue.size()
              << " queued transaction(s)...\n";
    for (auto& q : queue) {
        wrappedKiosk->purchaseItem(q.first, q.second);
    }
    queue.clear();
}