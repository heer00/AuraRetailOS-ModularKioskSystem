#include "core/CentralRegistry.h"

CentralRegistry* CentralRegistry::instance = nullptr;

CentralRegistry::CentralRegistry() {
    emergencyMode = false;
}

CentralRegistry* CentralRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new CentralRegistry();
    }
    return instance;
}

void CentralRegistry::setConfig(const std::string& key, const std::string& value) {
    config[key] = value;
}

std::string CentralRegistry::getConfig(const std::string& key) {
    if (config.find(key) != config.end()) {
        return config[key];
    }
    return "Not Found";
}

void CentralRegistry::setEmergencyMode(bool mode) {
    emergencyMode = mode;
}

bool CentralRegistry::isEmergencyMode() const {
    return emergencyMode;
}