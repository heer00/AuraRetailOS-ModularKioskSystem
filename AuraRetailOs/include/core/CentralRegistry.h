#ifndef CENTRAL_REGISTRY_H
#define CENTRAL_REGISTRY_H

#include <iostream>
#include <map>
#include <string>

class CentralRegistry {
private:
    static CentralRegistry* instance;
    std::map<std::string, std::string> config;
    bool emergencyMode;

    CentralRegistry();

public:
    static CentralRegistry* getInstance();

    void setConfig(const std::string& key, const std::string& value);
    std::string getConfig(const std::string& key);

    void setEmergency(bool mode);
    bool isEmergency();
};

#endif