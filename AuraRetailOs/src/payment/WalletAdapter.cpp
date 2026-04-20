#include <iostream>
  #include "payment/WalletAdapter.h"
  #include "payment/UserWallet.h"
   
  WalletAdapter::WalletAdapter(const std::string& uid) : userId(uid), lastAmount(0) {}
   
  bool WalletAdapter::pay(double amount) {
      std::cout << "[WalletAdapter] debitWallet(" << userId << ", Rs." << amount << ")" << std::endl;
      bool ok = UserWallet::getInstance()->debit(userId, amount);
      if (ok) lastAmount = amount;
      return ok;  // returns FALSE if insufficient balance -- payment declined
  }
   
  bool WalletAdapter::refund(const std::string& txnId) {
      std::cout << "[WalletAdapter] Refunding Rs." << lastAmount << " to " << userId << std::endl;
      UserWallet::getInstance()->credit(userId, lastAmount);
      return true;
  }
   
  std::string WalletAdapter::getStatus() const {
      double b = UserWallet::getInstance()->getBalance(userId);
      return "Wallet:" + userId + " Rs." + std::to_string((int)b);
  }
 