  // src/payment/PaymentPool.cpp
  #include <iostream>
  #include "payment/PaymentPool.h"
  #include "payment/UPIAdapter.h"
  
  PaymentPool::PaymentPool() {
      // Pre-seed pool with 2 UPI adapters
      available.push(new UPIAdapter());
      available.push(new UPIAdapter());
      std::cout << "[PaymentPool] Initialised with 2 payment adapters." << std::endl;
  }
  
  Payment* PaymentPool::acquire() {
      if (available.empty()) {
          std::cout << "[PaymentPool] WARNING -- pool exhausted, no adapters available." << std::endl;
          return nullptr;
      }
      Payment* p = available.front();
      available.pop();
      inUse.insert(p);
      std::cout << "[PaymentPool] Adapter acquired. Remaining: "
                << available.size() << std::endl;
      return p;
  }
  
  void PaymentPool::release(Payment* payment) {
      if (inUse.count(payment)) {
          inUse.erase(payment);
          available.push(payment);
          std::cout << "[PaymentPool] Adapter released. Available: "
                    << available.size() << std::endl;
      }
  }
 
