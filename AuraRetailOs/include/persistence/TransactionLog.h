// include/persistence/TransactionLog.h
  #ifndef TRANSACTIONLOG_H
  #define TRANSACTIONLOG_H
  
  #include <string>
  #include <vector>
  
  class TransactionLog {
  private:
      std::string filePath;
  
  public:
      TransactionLog(const std::string& path);
      void append(const std::string& logLine);
      std::vector<std::string> readAll();
  };
  
  #endif
