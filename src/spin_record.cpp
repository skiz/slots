#include "spin_record.h"
#include "reel.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <map>

bool SpinRecord::Create(SQLite::Database* db, SpinRecordType type,
    std::map<int, Symbol> symbols, unsigned int bet_credits,
    unsigned int bet_lines, unsigned int total_bet_credits,
    unsigned int won_credits, unsigned int won_cents, unsigned int bet_cents) {
  try {
    SQLite::Statement query(*db,
        "INSERT INTO spins (type, bet_credits, bet_lines, total_bet_credits, "
        "won_credits, won_cents, bet_cents, s0, s1, s2, s3, s4, s5, s6, "
        "s7, s8, s9, s10, s11, s12, s13, s14) VALUES (?, ?, ?, ?, ?, ?, ?, "
        "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bind(1, static_cast<int>(type));
    query.bind(2, bet_credits);
    query.bind(3, bet_lines);
    query.bind(4, total_bet_credits);
    query.bind(5, won_credits);
    query.bind(6, won_cents);
    query.bind(7, bet_cents);
    for (int i=0; i < 15; ++i) {
      query.bind(i+8, symbols[i]);
    }
    query.exec(); 
  } catch (std::exception& e) {
    std::cerr << "SQLite exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool SpinRecord::Reset(SQLite::Database* db) {
  try {
    db->exec("DROP TABLE IF EXISTS spins");
    db->exec("CREATE TABLE spins "
        "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "type INTEGER, "
        "bet_credits UNSIGNED INTEGER, "
        "bet_lines UNSIGNED INTEGER, "
        "total_bet_credits UNSIGNED INTEGER, "
        "won_credits UNSIGNED INTEGER, "
        "won_cents UNSIGNED INTEGER, "
        "bet_cents UNSIGNED INTEGER, "
        "s0 UNSIGNED INTEGER, "
        "s1 UNSIGNED INTEGER, "
        "s2 UNSIGNED INTEGER, "
        "s3 UNSIGNED INTEGER, "
        "s4 UNSIGNED INTEGER, "
        "s5 UNSIGNED INTEGER, "
        "s6 UNSIGNED INTEGER, "
        "s7 UNSIGNED INTEGER, "
        "s8 UNSIGNED INTEGER, "
        "s9 UNSIGNED INTEGER, "
        "s10 UNSIGNED INTEGER, "
        "s11 UNSIGNED INTEGER, "
        "s12 UNSIGNED INTEGER, "
        "s13 UNSIGNED INTEGER, "
        "s14 UNSIGNED INTEGER, "
        "time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
  } catch (std::exception& e) {
    std::cerr << "SQLite exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

std::vector<SpinRecord> SpinRecord::List(SQLite::Database* db, int limit, int offset) {
  std::vector<SpinRecord> res;
  try {
    SQLite::Statement query(*db,
        "SELECT id, type, bet_credits, bet_lines, total_bet_credits, "
        "won_credits, won_cents, bet_cents, s0, s1, s2, s3, s4, s5, s6, "
        "s7, s8, s9, s10, s11, s12, s13, s14 FROM spins "
        "ORDER BY time DESC LIMIT ? OFFSET ?");
    query.bind(1, limit);
    query.bind(2, offset);
    while (query.executeStep()) {
      SpinRecord r;
      r.id_ = query.getColumn(0).getInt();
      r.type_ = (SpinRecordType)query.getColumn(1).getInt();
      r.bet_credits_ = query.getColumn(2).getUInt();
      r.bet_lines_ = query.getColumn(3).getUInt();
      r.total_bet_credits_ = query.getColumn(4).getUInt();
      r.won_credits_ = query.getColumn(5).getUInt();
      r.won_cents_ = query.getColumn(6).getUInt();
      r.bet_cents_ = query.getColumn(7).getUInt();
      for (int i = 0; i < 15; i++) {
        r.symbols_[i] = (Symbol)query.getColumn(i+8).getInt();
      }
      res.push_back(r);
    }
  } catch (std::exception& e) {
    std::cerr << "SQLite exception: " << e.what() << std::endl;
  }
  return res;
}

