  // include/command/RefundCommand.h
  #ifndef REFUNDCOMMAND_H
  #define REFUNDCOMMAND_H
  
  #include <string>
  #include "command/Command.h"
  #include "payment/Payment.h"
  #include "inventory/Inventory.h"
  
  class RefundCommand : public Command {
  private:
      std::string  transactionId;
      std::string  productId;
      Payment*     payment;
      Inventory*   inventory;
      std::string  logMessage;
  
  public:
      RefundCommand(const std::string& transactionId,
                    const std::string& productId,
                    Payment*   payment,
                    Inventory* inventory);
  
      void execute()                   override;
      std::string getLog() const       override;
  };
  
  #endif
 
