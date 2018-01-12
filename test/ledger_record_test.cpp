#include "ledger_record.h"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>


class LedgerRecordTest : public ::testing::Test {
  protected:
    SQLite::Database* db_;

    virtual void SetUp() {
      db_ = new SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
      LedgerRecord::Reset(db_);
    }

    virtual void TearDown() {
      delete db_;
    }
};

TEST_F(LedgerRecordTest, InsertRecord) {
  bool r = LedgerRecord::Create(db_, COIN_INSERTED, 25);
  ASSERT_TRUE(r);
}

TEST_F(LedgerRecordTest, ListRecords) {
  for (int i=0; i<20; ++i) {
    ASSERT_TRUE(LedgerRecord::Create(db_, BILL_INSERTED, 10000));
  }
  
  std::vector<LedgerRecord> results = LedgerRecord::List(db_, 20, 0);
  ASSERT_EQ(20, results.size());
  LedgerRecord r = results[5];
  ASSERT_EQ(6, r.id_);
  ASSERT_EQ(BILL_INSERTED, r.type_);
  ASSERT_EQ(10000, r.cents_);
  ASSERT_EQ("", r.ticket_id_);

  results = LedgerRecord::List(db_, 5, 2);
  ASSERT_EQ(5, results.size());
  ASSERT_EQ(3, results[0].id_);

  results = LedgerRecord::List(db_, 20, 20);
  ASSERT_EQ(0, results.size());
}
