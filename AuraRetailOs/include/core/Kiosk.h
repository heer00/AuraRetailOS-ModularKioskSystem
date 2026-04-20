#ifndef KIOSK_H
  #define KIOSK_H
  
  #include <string>
  #include "core/KioskInterface.h"
  
  class Dispenser;
  class Payment;
  class Inventory;
  class PricingPolicy;
  
  class Kiosk : public KioskInterface {
  private:
      Dispenser*     dispenser;
      Payment*       payment;
      Inventory*     inventory;
      PricingPolicy* pricing;
  
  public:
      Kiosk(Inventory* i, Payment* p, Dispenser* d, PricingPolicy* pr);
      Dispenser*     getDispenser()     const;
      Payment*       getPayment()       const;
      Inventory*     getInventory()     const;
      PricingPolicy* getPricingPolicy() const;
  
    void purchaseItem(const std::string& productId,
                  const std::string& paymentMethod);
    void refundTransaction(const std::string& transactionId) override;
    void restockInventory(const std::string& productId, int qty) override;
    void runDiagnostics() override;
  };
  
  #endif