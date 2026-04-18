  #ifndef WALLETADAPTER_H
  #define WALLETADAPTER_H
  #include <string>
  #include "payment/Payment.h"
   
  // Adapter pattern: converts Payment interface to UserWallet's debit/credit API
  class WalletAdapter : public Payment {
  private:
      std::string userId;
      double      lastAmount;
  public:
      WalletAdapter(const std::string& userId = "guest");
      bool pay(double amount) override;                          // debits UserWallet
      bool refund(const std::string& transactionId) override;   // credits UserWallet
      std::string getStatus() const override;
  };
  #endif
