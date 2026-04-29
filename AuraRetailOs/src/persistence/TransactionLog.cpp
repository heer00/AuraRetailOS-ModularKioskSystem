//   // src/persistence/TransactionLog.cpp
//   #include <iostream>
//   #include <fstream>
//   #include "persistence/TransactionLog.h"
  
//   TransactionLog::TransactionLog(const std::string& path) : filePath(path) {}
  
//   void TransactionLog::append(const std::string& logLine) {
//       std::ofstream file(filePath, std::ios::app);
//       if (!file.is_open()) {
//           std::cout << "[TransactionLog] ERROR: Cannot open " << filePath << std::endl;
//           return;
//       }
//       file << logLine << "\n";
//       file.close();
//       std::cout << "[TransactionLog] Logged: " << logLine << std::endl;
//   }
  
//   std::vector<std::string> TransactionLog::readAll() {
//       std::vector<std::string> lines;
//       std::ifstream file(filePath);
//       if (!file.is_open()) return lines;
//       std::string line;
//       while (std::getline(file, line)) {
//           if (!line.empty()) lines.push_back(line);
//       }
//       file.close();
//       return lines;
//   }
 
#include <iostream>
#include <fstream>
#include <ctime>
#include "persistence/TransactionLog.h"

TransactionLog::TransactionLog(const std::string& path) : filePath(path) {
    // Write CSV header if file is new/empty
    std::ifstream check(filePath);
    bool isEmpty = !check.is_open() || check.peek() == std::ifstream::traits_type::eof();
    check.close();
    if (isEmpty) {
        std::ofstream hdr(filePath);
        if (hdr.is_open()) {
            hdr << "timestamp,type,details\n";
            hdr.close();
        }
    }
}

static std::string getTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

// Detect type from log message prefix
static std::string detectType(const std::string& logLine) {
    if (logLine.find("SUCCESS")   != std::string::npos) return "PURCHASE";
    if (logLine.find("RESTOCK")   != std::string::npos) return "RESTOCK";
    if (logLine.find("REFUNDED")  != std::string::npos) return "REFUND";
    if (logLine.find("ROLLED")    != std::string::npos) return "ROLLBACK";
    if (logLine.find("FAILED")    != std::string::npos) return "FAILED";
    return "INFO";
}

void TransactionLog::append(const std::string& logLine) {
    if (logLine.empty() || logLine == "Not executed") return;

    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        std::cout << "[TransactionLog] ERROR: Cannot open " << filePath << std::endl;
        return;
    }
    std::string ts   = getTimestamp();
    std::string type = detectType(logLine);
    // CSV: timestamp,type,"details"
    file << ts << "," << type << ",\"" << logLine << "\"\n";
    file.close();
    std::cout << "[TransactionLog] Logged: " << logLine << std::endl;
}

std::vector<std::string> TransactionLog::readAll() {
    std::vector<std::string> lines;
    std::ifstream file(filePath);
    if (!file.is_open()) return lines;
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        if (first) { first = false; continue; } // skip header
        if (!line.empty()) lines.push_back(line);
    }
    file.close();
    return lines;
}