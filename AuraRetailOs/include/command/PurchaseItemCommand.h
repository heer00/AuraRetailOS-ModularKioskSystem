  #ifndef PURCHASEITEMCOMMAND_H
  #define PURCHASEITEMCOMMAND_H
  #include <string>
  #include "command/Command.h"
  #include "command/InventoryMemento.h"   // NEW
  #include "inventory/Inventory.h"
  #include "payment/Payment.h"
  #include "hardware/Dispenser.h"
  #include "pricing/PricingPolicy.h"
   
  class PurchaseItemCommand : public Command {
  private:
      std::string    productId;
      Inventory*     inventory;
      Payment*       payment;
      Dispenser*     dispenser;
      PricingPolicy* pricingPolicy;
      std::string    logMessage;
      InventoryMemento memento;   // NEW: saved before any state change
  public:
      PurchaseItemCommand(const std::string& productId, Inventory* inventory,
                          Payment* payment, Dispenser* dispenser, PricingPolicy* policy);
      void execute() override;
      std::string getLog() const override;
  };
  #endif
