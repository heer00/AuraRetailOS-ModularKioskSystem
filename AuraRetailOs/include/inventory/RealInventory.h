#ifndef REALINVENTORY_H
#define REALINVENTORY_H

#include <map>
#include <vector>
#include "inventory/Inventory.h"

class RealInventory : public Inventory {
private:
    std::map<std::string, Item*> items;

public:
    void addItem(Item* item) override;
    Item* getItem(const std::string& id) override;
    int getStock(const std::string& id) override;
    void updateStock(const std::string& id, int newStock) override;
    std::vector<std::string> getAllProductIds() override;

    // Returns all items — needed by InventoryStore::save()
    const std::map<std::string, Item*>& getAll() const { return items; }
};
#endif