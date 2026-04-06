#ifndef CENTRALREGISTRY_H
#define CENTRALREGISTRY_H

#include <string>
#include <map>

class CentralRegistry {
private:
    static CentralRegistry* instance;
    std::map<std::string, std::string> config;
    bool emergencyMode;

    CentralRegistry(); // private constructor

public:
    static CentralRegistry* getInstance();

    void setConfig(const std::string& key, const std::string& value);
    std::string getConfig(const std::string& key);

    void setEmergencyMode(bool mode);
    bool isEmergencyMode() const;
};

#endif