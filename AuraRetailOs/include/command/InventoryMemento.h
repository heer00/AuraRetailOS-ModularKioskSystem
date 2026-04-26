#ifndef INVENTORY_MEMENTO_H
#define INVENTORY_MEMENTO_H

#include <string>

struct InventoryMemento {
    std::string productId;
    int stockBefore;
    bool valid;

    // Default constructor
    InventoryMemento() : productId(""), stockBefore(0), valid(false) {}

    // Parameterized constructor
    InventoryMemento(const std::string& pid, int stock, bool v)
        : productId(pid), stockBefore(stock), valid(v) {}
};

#endif
