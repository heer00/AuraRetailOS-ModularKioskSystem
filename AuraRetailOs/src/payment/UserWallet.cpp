  #include <iostream>
  #include "payment/UserWallet.h"
   
  UserWallet* UserWallet::instance = nullptr;
   
  UserWallet* UserWallet::getInstance() {
      if (!instance) instance = new UserWallet();
      return instance;
  }
   
  void UserWallet::topUp(const std::string& uid, double amount) {
      balances[uid] += amount;
      std::cout << "[UserWallet] Topped up " << uid << " +Rs." << amount
                << " | Balance: Rs." << balances[uid] << std::endl;
  }
   
  double UserWallet::getBalance(const std::string& uid) {
      return balances.count(uid) ? balances[uid] : 0.0;
  }
   
  bool UserWallet::debit(const std::string& uid, double amount) {
      double bal = getBalance(uid);
      if (bal < amount) {
          std::cout << "[UserWallet] INSUFFICIENT BALANCE for " << uid
                    << " | Has: Rs." << bal << " | Needs: Rs." << amount << std::endl;
          return false;
      }
      balances[uid] -= amount;
      std::cout << "[UserWallet] Debited Rs." << amount << " from " << uid
                << " | Remaining: Rs." << balances[uid] << std::endl;
      return true;
  }
   
  void UserWallet::credit(const std::string& uid, double amount) {
      balances[uid] += amount;
      std::cout << "[UserWallet] Refunded Rs." << amount << " to " << uid
                << " | Balance: Rs." << balances[uid] << std::endl;
  }
