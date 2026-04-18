#ifndef USERWALLET_H
  #define USERWALLET_H
  #include <map>
  #include <string>
   
  // Singleton pattern: one wallet store for the entire system
  class UserWallet {
  private:
      static UserWallet* instance;
      std::map<std::string, double> balances;
      UserWallet() {}
  public:
      static UserWallet* getInstance();
      void   topUp(const std::string& userId, double amount);   // add balance
      double getBalance(const std::string& userId);
      bool   debit(const std::string& userId, double amount);   // returns false if insufficient
      void   credit(const std::string& userId, double amount);  // used for refunds
  };
  #endif
