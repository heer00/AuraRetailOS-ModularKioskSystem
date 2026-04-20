  // src/main.cpp
  #include <iostream>
  #include <string>
  
  // Core
  #include "core/KioskInterface.h"
  #include "core/Kiosk.h"
  #include "core/KioskBuilder.h"
  #include "core/CentralRegistry.h"
  
  // Factories
  #include "factory/KioskFactorySimple.h"
  #include "factory/KioskFactory.h"
  
  // Hardware
  #include "hardware/RefrigerationModule.h"
  #include "hardware/SolarModule.h"
  #include "hardware/NetworkModule.h"
  
  // Inventory
  #include "inventory/RealInventory.h"
  #include "inventory/Bundle.h"
  #include "inventory/Product.h"
  
  // Persistence
  #include "persistence/ConfigStore.h"
  #include "persistence/TransactionLog.h"
  
  // Payment
  #include "payment/PaymentPool.h"
  
  // Commands
  #include "command/PurchaseItemCommand.h"
  #include "command/RefundCommand.h"
  #include "command/RestockCommand.h"
  
  // ANSI colours
  #define RST  "\033[0m"
  #define BOLD "\033[1m"
  #define CYN  "\033[36m"
  #define YLW  "\033[33m"
  #define GRN  "\033[32m"
  #define RED  "\033[31m"
  #define MAG  "\033[35m"
  
  void banner(const std::string& title, const std::string& color = CYN) {
      std::cout << "\n" << color << BOLD
                << "══════════════════════════════════════════════════\n"
                << "  " << title << "\n"
                << "══════════════════════════════════════════════════"
                << RST << "\n\n";
  }
  
  // ── helper: build a kiosk from factory, wrap in decorators, run a purchase ──
  void runKioskDemo(const std::string& type,
                    bool refrigerate, int temp,
                    bool solar, int battery,
                    bool offline,
                    TransactionLog& log) {
  
      banner("KIOSK TYPE: " + type, YLW);
  
      // 1. Abstract Factory creates compatible subsystem family
      KioskFactory* factory = KioskFactorySimple::createFactory(type);
      Kiosk* baseKiosk = KioskBuilder()
          .addDispenser(factory->createDispenser())
          .addPayment(factory->createPayment())
          .addInventory(factory->createInventory())
          .addPricingPolicy(factory->createPricingPolicy())
          .build();
  
      // 2. Decorator pattern: wrap kiosk in modules
      KioskInterface* kiosk = baseKiosk;
      if (refrigerate) kiosk = new RefrigerationModule(kiosk, temp);
      if (solar)       kiosk = new SolarModule(kiosk, battery);
      NetworkModule* netMod = nullptr;
      if (offline) {
          netMod = new NetworkModule(kiosk, true);
          kiosk  = netMod;
      }
  
      // 3. Run diagnostics
      std::cout << GRN << "[Diagnostics]" << RST << "\n";
      kiosk->runDiagnostics();
  
      // 4. Inventory: get first product from factory inventory
      std::string pid = (type == "food")      ? "FOOD001" :
                        (type == "pharmacy")  ? "MED001"  : "EMER001";
  
      // 5. Command pattern: PurchaseItemCommand encapsulates full flow
      PurchaseItemCommand cmd(pid,
                              baseKiosk->getInventory(),
                              baseKiosk->getPayment(),
                              baseKiosk->getDispenser(),
                              baseKiosk->getPricingPolicy());
      cmd.execute();
      log.append(cmd.getLog());
  
      // 6. If network module present, test queue + drain
      if (netMod) {
          std::cout << "\n" << MAG << "[NetworkModule] Testing offline queue..." << RST << "\n";
          kiosk->purchaseItem(pid, "wallet");  // goes to queue
          netMod->setOffline(false);
          netMod->flushQueue();               // drains queue
      }
  
      std::cout << "\n";
      delete factory;
  }
    int main() {
    // startup: load config, init log
    ConfigStore config("data/config.json");
    config.load();
    TransactionLog txLog("data/transactions.csv");

    std::string kioskType;
    std::cout << "Select kiosk type [food / pharmacy / emergency]: ";
    std::cin >> kioskType;

    KioskFactory* factory = KioskFactorySimple::createFactory(kioskType);
    Kiosk* kiosk = KioskBuilder()
        .addDispenser(factory->createDispenser())
        .addPayment(factory->createPayment())
        .addInventory(factory->createInventory())
        .addPricingPolicy(factory->createPricingPolicy())
        .build();

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
                kiosk->getInventory(), kiosk->getPayment(),
                kiosk->getDispenser(), kiosk->getPricingPolicy());
            cmd.execute();
            txLog.append(cmd.getLog());
        }
        else if (choice == 2) {
            std::string pid;
            std::cout << "Product ID: "; std::cin >> pid;
            int s = kiosk->getInventory()->getStock(pid);
            std::cout << "[Stock] " << pid << ": " << s << "\n";
        }
        else if (choice == 3) {
            std::string pid; int qty;
            std::cout << "Product ID: "; std::cin >> pid;
            std::cout << "Quantity: ";   std::cin >> qty;
            RestockCommand cmd(pid, qty, kiosk->getInventory());
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