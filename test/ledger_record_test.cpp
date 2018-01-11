#include "ledger_record.h"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>

TEST(LedgerRecordTest, Reset) {
  ASSERT_TRUE(LedgerRecord::Reset());
}

TEST(LedgerRecordTest, InsertRecord) {
  ASSERT_TRUE(LedgerRecord::Reset());
  bool r = LedgerRecord::Create(COIN_INSERTED, 25);
  ASSERT_TRUE(r);
}

TEST(LedgerRecordTest, ListRecords) {
  ASSERT_TRUE(LedgerRecord::Reset());
  for (int i=0; i<20; ++i) {
    ASSERT_TRUE(LedgerRecord::Create(BILL_INSERTED, 10000));
  }
  
  std::vector<LedgerRecord> results = LedgerRecord::List(20, 0);
  ASSERT_EQ(20, results.size());
  LedgerRecord r = results[5];
  ASSERT_EQ(6, r.id_);
  ASSERT_EQ(BILL_INSERTED, r.type_);
  ASSERT_EQ(10000, r.cents_);
  ASSERT_EQ("", r.ticket_id_);

  results = LedgerRecord::List(5, 2);
  ASSERT_EQ(5, results.size());
  ASSERT_EQ(3, results[0].id_);

  results = LedgerRecord::List(20, 20);
  ASSERT_EQ(0, results.size());
}

