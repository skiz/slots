#ifndef LEDGER_RECORD_H
#define LEDGER_RECORD_H

#include <iostream>
#include <vector>
#include <chrono>

enum LedgerRecordType {
  UNKNOWN,
  COIN_INSERTED,
  BILL_INSERTED,
  CASHOUT_TICKET,
  CASHOUT_HANDPAY,
};

class LedgerRecord {
  public:
    LedgerRecord() {}
    ~LedgerRecord() {}

    static bool Create(LedgerRecordType type, unsigned int cents);
    static bool Reset();
    static std::vector<LedgerRecord> List(int limit, int offset);

    int id_;
    LedgerRecordType type_;
    unsigned int cents_;
    std::string ticket_id_;
    std::chrono::system_clock::time_point time_;
};


#endif
