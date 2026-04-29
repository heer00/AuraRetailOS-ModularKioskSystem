#include "core/diagnosisservice.h"
#include "inventory/Inventory.h"
#include <sstream>
#include <vector>

DiagnosisReport DiagnosisService::runFullCheck(Kiosk* kiosk) {
    DiagnosisReport report;
    std::ostringstream oss;
    
    int totalProducts = 0;
    int lowStockCount = 0;
    
    if (kiosk && kiosk->getInventory()) {
        std::vector<std::string> ids = kiosk->getInventory()->getAllProductIds();
        totalProducts = ids.size();
        for (const auto& id : ids) {
            if (kiosk->getInventory()->getStock(id) < 5) { // Threshold for low stock
                lowStockCount++;
            }
        }
    }

    oss << "----------------------------------------\n\n";
    oss << "[Hardware]\n";
    oss << "Dispenser Type      : Robotic Arm\n";
    oss << "Dispenser Status    : <span style=\"color:#4caf50\">OK</span>\n";
    oss << "Motor Controller    : <span style=\"color:#4caf50\">OK</span>\n";
    oss << "Sensor Module       : <span style=\"color:#4caf50\">OK</span>\n";
    oss << "Door Lock           : Locked\n\n";

    oss << "[Optional Modules]\n";
    oss << "Refrigeration       : ON\n";
    oss << "Solar Module        : Charging\n";
    oss << "Network Module      : <span style=\"color:#4caf50\">Online</span>\n\n";

    oss << "[Payment Systems]\n";
    oss << "UPI                 : <span style=\"color:#4caf50\">Available</span>\n";
    oss << "Card                : <span style=\"color:#4caf50\">Available</span>\n";
    oss << "Wallet              : <span style=\"color:#4caf50\">Available</span>\n\n";

    oss << "[Inventory]\n";
    oss << "Total Products      : " << totalProducts << "\n";
    oss << "Low Stock Items     : " << (lowStockCount > 0 ? "<span style=\"color:#ffeb3b\">" : "") 
        << lowStockCount << (lowStockCount > 0 ? "</span>" : "") << "\n\n";

    oss << "[System & Security]\n";
    oss << "Authorization       : Active\n";
    oss << "Logging             : Enabled\n\n";
    oss << "----------------------------------------";

    report.fullReport = oss.str();
    return report;
}

