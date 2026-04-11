#ifndef KIOSK_INTERFACE_H
#define KIOSK_INTERFACE_H

#include <string>

class KioskInterface {
public:
    virtual void purchaseItem(const std::string& productId) = 0;
    virtual ~KioskInterface() {}
};

#endif