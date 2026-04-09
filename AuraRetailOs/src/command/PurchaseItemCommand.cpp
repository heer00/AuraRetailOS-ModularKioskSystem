#include <iostream>
#include "command/PurchaseItemCommand.h"
#include "inventory/Item.h"

PurchaseItemCommand::PurchaseItemCommand(const std::string& productId,
                                         Inventory* inventory,
                                         Payment* payment,
                                         Dispenser* dispenser,
                                         PricingPolicy* pricingPolicy)
    : productId(productId),
      inventory(inventory),
      payment(payment),
      dispenser(dispenser),
      pricingPolicy(pricingPolicy),
      logMessage("Not executed") {}

void PurchaseItemCommand::execute() {
    std::cout << "\n--- PurchaseItemCommand: " << productId << " ---" << std::endl;

    int stock = inventory->getStock(productId);
    if (stock <= 0) {
        logMessage = "FAILED: Out of stock [" + productId + "]";
        std::cout << "[PurchaseItemCommand] " << logMessage << std::endl;
        return;
    }

    Item* item = inventory->getItem(productId);
    if (!item) {
        logMessage = "FAILED: Product not found [" + productId + "]";
        std::cout << "[PurchaseItemCommand] " << logMessage << std::endl;
        return;
    }

    double finalPrice = pricingPolicy->computePrice(item);

    if (!payment->pay(finalPrice)) {
        logMessage = "FAILED: Payment unsuccessful [" + productId + "]";
        std::cout << "[PurchaseItemCommand] " << logMessage << std::endl;
        return;
    }

    if (!dispenser->dispense(productId)) {
        logMessage = "FAILED: Dispensing failed [" + productId + "]";
        std::cout << "[PurchaseItemCommand] " << logMessage << std::endl;
        return;
    }

    inventory->updateStock(productId, stock - 1);

    logMessage = "SUCCESS: purchased [" + productId + "] for Rs." + std::to_string(finalPrice);
    std::cout << "[PurchaseItemCommand] " << logMessage << std::endl;
}

std::string PurchaseItemCommand::getLog() const {
    return logMessage;
}
