#ifndef RESTOCKCOMMAND_H
#define RESTOCKCOMMAND_H

#include <string>
#include "command/Command.h"
#include "inventory/Inventory.h"

class RestockCommand : public Command {
private:
    std::string productId;
    int quantity;
    Inventory* inventory;
    std::string logMessage;

public:
    RestockCommand(const std::string& productId,
                   int quantity,
                   Inventory* inventory);

    void execute() override;
    std::string getLog() const override;
};

#endif
