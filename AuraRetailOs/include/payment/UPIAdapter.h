#ifndef UPIADAPTER_H
#define UPIADAPTER_H
#include "payment/Payment.h"

// ADAPTER: wraps external UPI SDK into Payment interface
class UPIAdapter : public Payment {
public:
    bool pay(double amount) override;
    bool refund(const std::string& transactionId) override;
    std::string getStatus() const override;
};
#endif