  // src/persistence/TransactionLog.cpp
  #include <iostream>
  #include <fstream>
  #include "persistence/TransactionLog.h"
  
  TransactionLog::TransactionLog(const std::string& path) : filePath(path) {}
  
  void TransactionLog::append(const std::string& logLine) {
      std::ofstream file(filePath, std::ios::app);
      if (!file.is_open()) {
          std::cout << "[TransactionLog] ERROR: Cannot open " << filePath << std::endl;
          return;
      }
      file << logLine << "\n";
      file.close();
      std::cout << "[TransactionLog] Logged: " << logLine << std::endl;
  }
  
  std::vector<std::string> TransactionLog::readAll() {
      std::vector<std::string> lines;
      std::ifstream file(filePath);
      if (!file.is_open()) return lines;
      std::string line;
      while (std::getline(file, line)) {
          if (!line.empty()) lines.push_back(line);
      }
      file.close();
      return lines;
  }
 
