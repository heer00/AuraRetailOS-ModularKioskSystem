#include <iostream>
#include <string>

// Core
#include "core/KioskInterface.h"
#include "core/Kiosk.h"
#include "core/KioskBuilder.h"

// Factories
#include "factory/KioskFactorySimple.h"
#include "factory/KioskFactory.h"

// Hardware
#include "hardware/SolarModule.h"
#include "hardware/NetworkModule.h"

// Persistence
#include "persistence/ConfigStore.h"
#include "persistence/TransactionLog.h"

// Commands
#include "command/PurchaseItemCommand.h"
#include "command/RestockCommand.h"

int main() {
    ConfigStore config("data/config.json");
    config.load();
    TransactionLog txLog("data/transactions.csv");

    std::string kioskType;
    std::cout << "Select kiosk type [food / pharmacy / emergency]: ";
    std::cin >> kioskType;

    KioskFactory* factory = KioskFactorySimple::createFactory(kioskType);

    Kiosk* baseKiosk = KioskBuilder()
        .addDispenser(factory->createDispenser())
        .addPayment(factory->createPayment())
        .addInventory(factory->createInventory())
        .addPricingPolicy(factory->createPricingPolicy())
        .build();

    KioskInterface* kiosk = baseKiosk;
    kiosk = new SolarModule(kiosk, 100);
    kiosk = new NetworkModule(kiosk, false);

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n--- Aura Kiosk Menu ---\n"
                  << "1. Buy item\n"
                  << "2. View stock\n"
                  << "3. Restock item\n"
                  << "4. Run diagnostics\n"
                  << "5. Exit\n"
                  << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string pid, method;
            std::cout << "Product ID: "; std::cin >> pid;
            std::cout << "Payment method [card/upi/wallet]: "; std::cin >> method;

            PurchaseItemCommand cmd(pid,
                baseKiosk->getInventory(),
                baseKiosk->getPayment(),
                baseKiosk->getDispenser(),
                baseKiosk->getPricingPolicy());

            cmd.execute();
            txLog.append(cmd.getLog());
        }
        else if (choice == 2) {
            std::string pid;
            std::cout << "Product ID: "; std::cin >> pid;
            int s = baseKiosk->getInventory()->getStock(pid);
            std::cout << "[Stock] " << pid << ": " << s << "\n";
        }
        else if (choice == 3) {
            std::string pid; int qty;
            std::cout << "Product ID: "; std::cin >> pid;
            std::cout << "Quantity: "; std::cin >> qty;

            RestockCommand cmd(pid, qty, baseKiosk->getInventory());
            cmd.execute();
            txLog.append(cmd.getLog());
        }
        else if (choice == 4) {
            kiosk->runDiagnostics();
        }
    }

    delete factory;
    return 0;
}
