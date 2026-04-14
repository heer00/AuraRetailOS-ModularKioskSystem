  // src/payment/WalletAdapter.cpp
  #include <iostream>
  #include "payment/WalletAdapter.h"
  
  bool WalletAdapter::pay(double amount) {
      std::cout << "[WalletAdapter] Translating pay() -> debitWallet()" << std::endl;
      std::cout << "[Wallet SDK] debitWallet(Rs." << amount << ") -> SUCCESS" << std::endl;
      return true;
  }
  
  bool WalletAdapter::refund(const std::string& transactionId) {
      std::cout << "[WalletAdapter] Crediting wallet for txn: " << transactionId << std::endl;
      return true;
  }
  
  std::string WalletAdapter::getStatus() const { return "Wallet:online"; }
 
