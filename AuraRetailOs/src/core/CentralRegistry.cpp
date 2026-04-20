#include "core/CentralRegistry.h"

CentralRegistry* CentralRegistry::instance = nullptr;

CentralRegistry::CentralRegistry() {
    emergencyMode = false;
}

CentralRegistry* CentralRegistry::getInstance() {
    if (instance == nullptr)
        instance = new CentralRegistry();
    return instance;
}

void CentralRegistry::setConfig(const std::string& key, const std::string& value) {
    config[key] = value;
}

std::string CentralRegistry::getConfig(const std::string& key) {
    return config[key];
}

void CentralRegistry::setEmergency(bool mode) {
    emergencyMode = mode;
}

bool CentralRegistry::isEmergency() {
    return emergencyMode;
}