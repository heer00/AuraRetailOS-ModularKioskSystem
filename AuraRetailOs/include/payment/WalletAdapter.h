#ifndef WALLET_ADAPTER_H
#define WALLET_ADAPTER_H

#include <string>
#include "payment/Payment.h"

class WalletAdapter : public Payment {
private:
    std::string userId;
    double lastAmount;

public:
    WalletAdapter(const std::string& uid);

    bool pay(double amount) override;
    bool refund(const std::string& txnId) override;

std::string getStatus() const override;};

#endif