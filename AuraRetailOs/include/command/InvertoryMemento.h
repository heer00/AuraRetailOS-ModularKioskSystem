  #ifndef INVENTORYMEMENTO_H
  #define INVENTORYMEMENTO_H
  #include <string>
   
  // Memento pattern: snapshot of product stock before a transaction starts
  // If anything fails after payment, we restore using this snapshot
  struct InventoryMemento {
      std::string productId;
      int         stockBefore;
      bool        valid;   // false = memento not yet taken
      InventoryMemento() : stockBefore(0), valid(false) {}
  };
  #endif
