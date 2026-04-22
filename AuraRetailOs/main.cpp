#include <iostream>
#include <string>
#include <vector>

#include "core/KioskInterface.h"
#include "core/Kiosk.h"
#include "core/KioskBuilder.h"
#include "core/CentralRegistry.h"

#include "factory/KioskFactorySimple.h"
#include "factory/KioskFactory.h"

#include "hardware/SolarModule.h"
#include "hardware/NetworkModule.h"

#include "persistence/ConfigStore.h"
#include "persistence/TransactionLog.h"
#include "persistence/InventoryStore.h"

#include "command/PurchaseItemCommand.h"
#include "command/RestockCommand.h"

#include "inventory/Inventory.h"
#include "inventory/RealInventory.h"
#include "inventory/InventoryProxy.h"
#include "inventory/CityMonitor.h"
#include "inventory/Product.h"

struct ProductInfo { std::string id; std::string name; };

std::vector<ProductInfo> getProductList(const std::string& type) {
    if (type == "food")
        return {{"FOOD001","Chips Pack"},{"FOOD002","Cold Drink"},{"FOOD003","Chocolate Bar"}};
    if (type == "pharmacy")
        return {{"MED001","Aspirin Pack"},{"MED002","Vitamin C"},{"MED003","Bandage Roll"}};
    if (type == "emergency")
        return {{"EMER001","First Aid Kit"},{"EMER002","Water Bottle"},{"EMER003","Energy Bar"}};
    return {};
}

void printStock(const std::string& id, const std::string& name, int stock) {
    std::cout << "  " << id << " | " << name << " | ";
    if      (stock <  0) std::cout << "Product not found in this kiosk\n";
    else if (stock == 0) std::cout << "Out of Stock\n";
    else if (stock <= 3) std::cout << stock << " units  *** LOW STOCK ***\n";
    else                 std::cout << stock << " units\n";
}

int main() {
    ConfigStore config("data/config.json");
    config.load();
    TransactionLog txLog("data/transactions.csv");

    std::string kioskType;
    std::cout << "Select kiosk type [food / pharmacy / emergency]: ";
    std::cin >> kioskType;

    KioskFactory* factory = KioskFactorySimple::createFactory(kioskType);

    // Load inventory from JSON — this IS the stock source of truth
    RealInventory* realInv = new RealInventory();
    InventoryStore invStore("data/inventory.json");
    invStore.load(realInv);

    CityMonitor cityMonitor;
    InventoryProxy* proxy = new InventoryProxy(
        realInv, kioskType + "-system", &cityMonitor, 3);

    Kiosk* baseKiosk = KioskBuilder()
        .addDispenser(factory->createDispenser())
        .addPayment(factory->createPayment())
        .addInventory(proxy)
        .addPricingPolicy(factory->createPricingPolicy())
        .build();

    KioskInterface* kiosk = baseKiosk;
    kiosk = new SolarModule(kiosk, 100);
    kiosk = new NetworkModule(kiosk, false);

    std::vector<ProductInfo> products = getProductList(kioskType);

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n--- Aura Kiosk Menu [" << kioskType << "] ---\n"
                  << "1. Buy item\n"
                  << "2. View stock\n"
                  << "3. Restock item\n"
                  << "4. Run diagnostics\n"
                  << "5. Exit\n"
                  << "Choice: ";
        std::cin >> choice;

        // ── 1. BUY ───────────────────────────────────────────────
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

            // Save to JSON only if purchase succeeded
            if (cmd.getLog().find("SUCCESS") != std::string::npos)
                invStore.save(realInv);
        }

        // ── 2. VIEW STOCK ─────────────────────────────────────────
        else if (choice == 2) {
            std::cout << "\n[Stock Status - " << kioskType << " kiosk]\n";
            std::cout << "  Enter product ID (or 'all'): ";
            std::string input; std::cin >> input;

            if (input == "all") {
                for (auto& p : products) {
                    int s = realInv->getStock(p.id);
                    printStock(p.id, p.name, s);
                }
            } else {
                int s = realInv->getStock(input);
                std::string name = "Unknown";
                for (auto& p : products)
                    if (p.id == input) { name = p.name; break; }
                printStock(input, name, s);
            }
        }

        // ── 3. RESTOCK ───────────────────────────────────────────
        else if (choice == 3) {
            std::string pid; int qty;
            std::cout << "Product ID: "; std::cin >> pid;
            std::cout << "Quantity: "; std::cin >> qty;

            RestockCommand cmd(pid, qty, baseKiosk->getInventory());
            cmd.execute();
            txLog.append(cmd.getLog());

            // Show final stock + save to JSON only if restock succeeded
            if (cmd.getLog().find("RESTOCK") != std::string::npos) {
                int s = realInv->getStock(pid);
                std::string name = "Unknown";
                for (auto& p : products)
                    if (p.id == pid) { name = p.name; break; }
                std::cout << "[Restock] Final stock: ";
                printStock(pid, name, s);
                invStore.save(realInv);
            }
        }

        // ── 4. DIAGNOSTICS ───────────────────────────────────────
        else if (choice == 4) {
            kiosk->runDiagnostics();
        }

        else if (choice != 5) {
            std::cout << "Invalid choice. Enter 1-5.\n";
        }
    }

    std::cout << "[Aura Kiosk] Shutting down.\n";
    delete factory;
    return 0;
}