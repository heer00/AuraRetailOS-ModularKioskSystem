#include <iostream>
  #include "core/CentralRegistry.h"
   
  CentralRegistry* CentralRegistry::instance = nullptr;
   
  CentralRegistry::CentralRegistry() { emergencyMode = false; }
   
  CentralRegistry* CentralRegistry::getInstance() {
      if (instance == nullptr) instance = new CentralRegistry();
      return instance;
  }
   
  void CentralRegistry::setConfig(const std::string& key, const std::string& value) {
      config[key] = value;
  }
   
  std::string CentralRegistry::getConfig(const std::string& key) {
      if (config.find(key) != config.end()) return config[key];
      return "Not Found";
  }
   
  void CentralRegistry::setEmergencyMode(bool mode) { emergencyMode = mode; }
  bool CentralRegistry::isEmergencyMode() const { return emergencyMode; }
   
  // Feature C: Singleton + Strategy integration
  // Because DiscountPricing reads discountRate from this registry at compute-time,
  // calling this ONE method instantly changes prices across every kiosk in the system.
  void CentralRegistry::activateEmergencyPricing() {
      emergencyMode = true;
      config["discountRate"] = "0.00";
      config["maxCap"] = "50";
      std::cout << "[CentralRegistry] *** EMERGENCY PRICING ACTIVATED city-wide ***\n";
      std::cout << "  All kiosks: prices capped Rs.50, discounts OFF.\n";
  }
   
  void CentralRegistry::deactivateEmergencyPricing() {
      emergencyMode = false;
      config["discountRate"] = "0.15";
      config["maxCap"] = "9999";
      std::cout << "[CentralRegistry] Emergency pricing OFF. Normal rates restored.\n";
  }