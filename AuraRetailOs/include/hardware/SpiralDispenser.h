#ifndef SPIRALDISPENSER_H
#define SPIRALDISPENSER_H

#include "hardware/Dispenser.h"

class SpiralDispenser : public Dispenser {
private:
    bool ready;

public:
    SpiralDispenser();

    void calibrate() override;
    bool isReady() const override;
    bool dispense(const std::string& productId) override;
};

#endif
