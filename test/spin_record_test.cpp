#include "spin_record.h"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

class SpinRecordTest : public ::testing::Test {
  protected:
    SQLite::Database* db_;

    virtual void SetUp() {
      db_ = new SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
      SpinRecord::Reset(db_);
    }

    virtual void TearDown() {
      delete db_;
    }
};

TEST_F(SpinRecordTest, TestSetup) {
  ASSERT_NE(nullptr, db_);
}

TEST_F(SpinRecordTest, InsertRecord) {
  std::map<int, Symbol> symbols;
  for (int i = 0; i < 15; i++) {
    symbols[i] = CHERRY;
  }
  
  bool r = SpinRecord::Create(db_, SPIN_RECORD_MAIN, symbols, 2, 3, 6, 10, 50, 30);
  ASSERT_TRUE(r);
}

TEST_F(SpinRecordTest, ListRecords) {
  std::map<int, Symbol> symbols;
  for (int i = 0; i < 15; i++) {
    symbols[i] = (Symbol)i;
  }

  for (int i=0; i<20; ++i) {
    ASSERT_TRUE(SpinRecord::Create(db_, SPIN_RECORD_FREE, symbols, 1,2,3,4,5,6));
  }
  
  std::vector<SpinRecord> results = SpinRecord::List(db_, 20, 0);
  ASSERT_EQ(20, results.size());
  SpinRecord r = results[5];
  ASSERT_EQ(6, r.id_);
  ASSERT_EQ(SPIN_RECORD_FREE, r.type_);
  ASSERT_EQ(1, r.bet_credits_);
  ASSERT_EQ(2, r.bet_lines_);
  ASSERT_EQ(3, r.total_bet_credits_);
  ASSERT_EQ(4, r.won_credits_);
  ASSERT_EQ(5, r.won_cents_);
  ASSERT_EQ(6, r.bet_cents_);
  ASSERT_EQ(15, r.symbols_.size());
  for (int i = 0; i < 15; i++) {
    ASSERT_EQ(r.symbols_[i], i);
  }
  
  results = SpinRecord::List(db_, 5, 2);
  ASSERT_EQ(5, results.size());
  ASSERT_EQ(3, results[0].id_);

  results = SpinRecord::List(db_, 20, 20);
  ASSERT_EQ(0, results.size());
}
