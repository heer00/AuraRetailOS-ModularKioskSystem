#ifndef INVENTORY_H
#define INVENTORY_H
#include "inventory/Item.h"
#include <map>
#include <string>
#include <iostream>

class Inventory {
private:
    std::map<std::string, Item*> items;
public:
    void addItem(Item* item) {
        items[item->getId()] = item;
        std::cout << "[Inventory] Added: " << item->getName()
                  << " (Stock: " << item->getStock() << ")" << std::endl;
    }
    Item* getItem(const std::string& id) {
        if (items.count(id)) return items[id];
        return nullptr;
    }
    bool reduceStock(const std::string& id) {
        Item* item = getItem(id);
        if (item && item->getStock() > 0) {
            item->setStock(item->getStock() - 1);
            return true;
        }
        return false;
    }
    ~Inventory() {
        for (auto& pair : items) delete pair.second;
    }
};
#endif