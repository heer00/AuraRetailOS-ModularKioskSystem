#include <iostream>
#include "command/RestockCommand.h"

RestockCommand::RestockCommand(const std::string& productId,
                               int quantity,
                               Inventory* inventory)
    : productId(productId),
      quantity(quantity),
      inventory(inventory),
      logMessage("Not executed") {}

void RestockCommand::execute() {
    std::cout << "\n--- RestockCommand: " << productId
              << " +" << quantity << " ---" << std::endl;

    // Validation 1: quantity must be positive
    if (quantity <= 0) {
        logMessage = "FAILED: Restock quantity must be positive, got "
                     + std::to_string(quantity);
        std::cout << "[RestockCommand] " << logMessage << std::endl;
        return;
    }

    // Validation 2: product must exist
    int currentStock = inventory->getStock(productId);
    if (currentStock < 0) {
        logMessage = "FAILED: Product not found [" + productId + "]";
        std::cout << "[RestockCommand] " << logMessage << std::endl;
        return;
    }

    inventory->updateStock(productId, currentStock + quantity);

    logMessage = "RESTOCK: [" + productId + "] +"
                 + std::to_string(quantity)
                 + " | new stock: "
                 + std::to_string(currentStock + quantity);
    std::cout << "[RestockCommand] " << logMessage << std::endl;
}

std::string RestockCommand::getLog() const {
    return logMessage;
}