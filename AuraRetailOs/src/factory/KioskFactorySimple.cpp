#include "factory/KioskFactorySimple.h"
#include "factory/PharmacyFactory.h"
#include "factory/FoodFactory.h"

KioskFactory* KioskFactorySimple::createFactory(const std::string& type) {
    if (type == "pharmacy") return new PharmacyFactory();
    if (type == "food") return new FoodFactory();
    return new PharmacyFactory();
}