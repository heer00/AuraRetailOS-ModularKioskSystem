#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "persistence/InventoryStore.h"
#include "inventory/Product.h"

InventoryStore::InventoryStore(const std::string& path) : filePath(path) {}

void InventoryStore::load(RealInventory* inventory) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[InventoryStore] WARNING: Cannot open " << filePath
                  << " — check that data/inventory.json exists in the"
                  << " AuraRetailOs folder." << std::endl;
        return;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        // Strip \r for Windows CRLF safety
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.find("\"id\"") == std::string::npos) continue;

        auto get = [&](const std::string& key) -> std::string {
            auto p = line.find("\"" + key + "\":");
            if (p == std::string::npos) return "";
            p += key.size() + 3;
            while (p < line.size() && line[p] == ' ') p++;
            size_t q = line.size();
            for (char ch : {',', '}'}) {
                auto idx = line.find(ch, p);
                if (idx != std::string::npos && idx < q) q = idx;
            }
            std::string v = line.substr(p, q - p);
            // trim trailing spaces
            while (!v.empty() && v.back() == ' ') v.pop_back();
            if (v.size() >= 2 && v.front() == '"') v = v.substr(1, v.size() - 2);
            return v;
        };

        std::string id    = get("id");
        std::string name  = get("name");
        std::string prStr = get("price");
        std::string stStr = get("stock");

        if (id.empty()) continue;

        double price = prStr.empty() ? 0.0 : std::stod(prStr);
        int    stock = stStr.empty() ? 0   : std::stoi(stStr);

        inventory->addItem(new Product(id, name, price, stock));
        std::cout << "[InventoryStore] Loaded: " << id
                  << " (" << name << ") stock=" << stock << std::endl;
        count++;
    }
    file.close();
    std::cout << "[InventoryStore] Total items loaded: " << count << std::endl;
}

void InventoryStore::save(RealInventory* inventory) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[InventoryStore] ERROR: Cannot write to " << filePath << std::endl;
        return;
    }
    file << "[\n";
    const auto& all = inventory->getAll();
    bool first = true;
    for (const auto& pair : all) {
        Item* item = pair.second;
        if (!first) file << ",\n";
        file << "  {\"id\": \""   << item->getId()   << "\""
             << ", \"name\": \"" << item->getName() << "\""
             << ", \"price\": "  << item->getPrice()
             << ", \"stock\": "  << item->getStock() << "}";
        first = false;
    }
    file << "\n]\n";
    file.close();
    std::cout << "[InventoryStore] Saved to " << filePath << std::endl;
}