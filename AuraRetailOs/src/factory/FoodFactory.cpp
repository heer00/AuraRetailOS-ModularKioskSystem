 #include <iostream>
  #include "factory/FoodFactory.h"
  #include "hardware/SpiralDispenser.h"
  #include "payment/UPIAdapter.h"
  #include "inventory/RealInventory.h"
  #include "inventory/InventoryProxy.h"
  #include "inventory/CityMonitor.h"  // NEW
  #include "inventory/Product.h"
  #include "pricing/DiscountPricing.h"
   
  Dispenser* FoodFactory::createDispenser() {
      std::cout << "[FoodFactory] Creating SpiralDispenser\n";
      return new SpiralDispenser();
  }
  Payment* FoodFactory::createPayment() {
      std::cout << "[FoodFactory] Creating UPIAdapter\n";
      return new UPIAdapter();
  }
  Inventory* FoodFactory::createInventory() {
      RealInventory* real = new RealInventory();
      real->addItem(new Product("FOOD001", "Chips Pack",    20.0, 10));
      real->addItem(new Product("FOOD002", "Cold Drink",    35.0,  8));
      real->addItem(new Product("FOOD003", "Chocolate Bar", 50.0,  5));
      // Pass CityMonitor -- alerts fire when stock drops to 3 or below
      return new InventoryProxy(real, "food-system", new CityMonitor(), 3);
  }
  PricingPolicy* FoodFactory::createPricingPolicy() {
      return new DiscountPricing(0.10);
  } 