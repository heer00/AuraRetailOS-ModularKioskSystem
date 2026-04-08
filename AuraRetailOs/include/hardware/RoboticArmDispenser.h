#ifndef ROBOTICARMDISPENSER_H
#define ROBOTICARMDISPENSER_H

#include "hardware/Dispenser.h"

class RoboticArmDispenser : public Dispenser {
private:
    bool ready;

public:
    RoboticArmDispenser();

    void calibrate() override;
    bool isReady() const override;
    bool dispense(const std::string& productId) override;
};

#endif
