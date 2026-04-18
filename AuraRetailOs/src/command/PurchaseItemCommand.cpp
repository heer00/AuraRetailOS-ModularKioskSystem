#include <iostream>
  #include "command/PurchaseItemCommand.h"
  #include "inventory/Item.h"
   
  PurchaseItemCommand::PurchaseItemCommand(const std::string& pid,
      Inventory* inv, Payment* pay, Dispenser* dis, PricingPolicy* pr)
      : productId(pid), inventory(inv), payment(pay),
        dispenser(dis), pricingPolicy(pr), logMessage("Not executed") {}
   
  void PurchaseItemCommand::execute() {
      std::cout << "\n--- PurchaseItemCommand: " << productId << " ---" << std::endl;
   
      int stock = inventory->getStock(productId);
      if (stock <= 0) {
          logMessage = "FAILED: Out of stock [" + productId + "]";
          std::cout << "[Cmd] " << logMessage << std::endl; return;
      }
   
      Item* item = inventory->getItem(productId);
      if (!item) {
          logMessage = "FAILED: Product not found [" + productId + "]";
          std::cout << "[Cmd] " << logMessage << std::endl; return;
      }
   
      // --- MEMENTO: save state BEFORE any changes ---
      memento = {productId, stock, true};
      std::cout << "[Memento] Snapshot: " << productId << " stock=" << stock << std::endl;
   
      double finalPrice = pricingPolicy->computePrice(item);
   
      // Step 1: Payment
      if (!payment->pay(finalPrice)) {
          logMessage = "FAILED: Payment declined [" + productId + "]";
          // No stock changed yet -- nothing to rollback
          std::cout << "[Cmd] " << logMessage << std::endl; return;
      }
   
      // Step 2: Dispense
      if (!dispenser->dispense(productId)) {
          // --- ATOMIC ROLLBACK: payment succeeded but dispense failed ---
          std::cout << "[Memento] ROLLBACK: dispense failed! Reversing payment." << std::endl;
          payment->refund("ROLLBACK-" + productId);
          std::cout << "[Memento] Stock stays at: " << memento.stockBefore << std::endl;
          logMessage = "ROLLED BACK: dispense error, payment reversed [" + productId + "]";
          std::cout << "[Cmd] " << logMessage << std::endl; return;
      }
   
      // Step 3: Both succeeded -- commit inventory change
      inventory->updateStock(productId, stock - 1);
      logMessage = "SUCCESS: [" + productId + "] Rs." + std::to_string(finalPrice);
      std::cout << "[Cmd] " << logMessage << std::endl;
  }
   
  std::string PurchaseItemCommand::getLog() const { return logMessage; }
