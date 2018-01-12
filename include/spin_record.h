#ifndef SPIN_RECORD_H
#define SPIN_RECORD_H

#include "reel.h"
#include <SQLiteCpp/SQLiteCpp.h>

enum SpinRecordType {
  SPIN_RECORD_UNKNOWN,
  SPIN_RECORD_MAIN,
  SPIN_RECORD_FREE,
  SPIN_RECORD_BONUS
};

class SpinRecord {
  public:
    SpinRecord() {}
    ~SpinRecord() {}

    static bool Create(SQLite::Database* db, SpinRecordType type,
        std::map<int, Symbol> symbols, 
        unsigned int bet_credits, unsigned int bet_lines,
        unsigned int total_bet_credits, unsigned int won_credits,
        unsigned int won_cents, unsigned int bet_cents);

    static std::vector<SpinRecord> List(SQLite::Database* db, int limit, int offset);
    
    static bool Reset(SQLite::Database* db);

    int id_;
    SpinRecordType type_;
    unsigned int bet_credits_;
    unsigned int bet_lines_;
    unsigned int total_bet_credits_;
    unsigned int won_credits_;
    unsigned int won_cents_;
    unsigned int bet_cents_;
    std::map<int, Symbol> symbols_; 
};

#endif
