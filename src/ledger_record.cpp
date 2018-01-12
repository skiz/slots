#include "ledger_record.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <SQLiteCpp/SQLiteCpp.h>

bool LedgerRecord::Create(SQLite::Database* db, LedgerRecordType type, unsigned int cents) {
  try {
    SQLite::Statement query(*db,
        "INSERT INTO ledger (type, amount, ticket_id) VALUES (?, ?, NULL)");
    query.bind(1, static_cast<int>(type));
    query.bind(2, cents);
    query.exec();
  } catch (std::exception& e) {
    std::cerr << "SQLite exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool LedgerRecord::Reset(SQLite::Database* db) {
  try {
    db->exec("DROP TABLE IF EXISTS ledger");
    db->exec("CREATE TABLE ledger "
        "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "type INTEGER, "
        "amount UNSIGNED INTEGER, "
        "ticket_id STRING, "
        "time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
  } catch (std::exception& e) {
    std::cerr << "SQLite exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

std::vector<LedgerRecord> LedgerRecord::List(SQLite::Database* db, int limit, int offset) {
  std::vector<LedgerRecord> res;
  try {
    SQLite::Statement query(*db,
        "SELECT id, type, amount, ticket_id, time FROM ledger "
        " ORDER BY time DESC LIMIT ? OFFSET ?");
    query.bind(1, limit);
    query.bind(2, offset);
    while (query.executeStep()) {
      LedgerRecord r;
      r.id_ = query.getColumn(0).getInt();
      r.type_ = (LedgerRecordType)query.getColumn(1).getInt();
      r.cents_ = query.getColumn(2).getUInt();
      r.ticket_id_ = query.getColumn(3).getString();
      std::string ts = query.getColumn(4).getString();
      std::stringstream ss;
      ss << ts;
      std::tm tm = {};
      ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
      r.time_ = std::chrono::system_clock::from_time_t(std::mktime(&tm));
      res.push_back(r);
    }
  } catch (std::exception& e) {
      std::cerr << "SQLite exception: " << e.what() << std::endl;
  }
  return res;
}

