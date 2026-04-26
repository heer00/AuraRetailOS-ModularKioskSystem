#include <iostream>
#include <fstream>
#include <string>
#include "persistence/ConfigStore.h"
#include "core/CentralRegistry.h"

ConfigStore::ConfigStore(const std::string& path) : filePath(path) {}

void ConfigStore::load() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[ConfigStore] WARNING: " << filePath
                  << " not found. Using defaults." << std::endl;
        CentralRegistry::getInstance()->setConfig("discountRate", "0.15");
        CentralRegistry::getInstance()->setConfig("emergencyMode", "false");
        CentralRegistry::getInstance()->setConfig("maxCap",        "50");
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Find key: starts after first quote, ends before second quote
        auto k1 = line.find('"');
        if (k1 == std::string::npos) continue;
        auto k2 = line.find('"', k1 + 1);
        if (k2 == std::string::npos) continue;
        std::string key = line.substr(k1 + 1, k2 - k1 - 1);
        if (key.empty()) continue;

        // Find value: after the colon, between next pair of quotes
        auto colon = line.find(':', k2);
        if (colon == std::string::npos) continue;
        auto v1 = line.find('"', colon);
        if (v1 == std::string::npos) continue;
        auto v2 = line.find('"', v1 + 1);
        if (v2 == std::string::npos) continue;
        std::string val = line.substr(v1 + 1, v2 - v1 - 1);

        CentralRegistry::getInstance()->setConfig(key, val);
        std::cout << "[ConfigStore] Set " << key << " = " << val << std::endl;
    }
    file.close();
}