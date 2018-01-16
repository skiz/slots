#include "accounting.h"
#include "engine.h"
#include "reel.h"
#include "ledger_record.h"
#include "spin_record.h"
#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

#define RUN_LONGTESTS false

class PayoutRateTest : public ::testing::Test {
  protected:
    SQLite::Database* db_;
    Accounting a_;

    virtual void SetUp() {
      db_ = new SQLite::Database(":memory:", SQLite::OPEN_READWRITE);
      LedgerRecord::Reset(db_);
      SpinRecord::Reset(db_);

      a_.Init(db_);
    }

    virtual void TearDown() {
      delete db_;
    }
};

# if RUN_LONGTESTS
TEST_F(PayoutRateTest, PayoutPercentage) {
  const unsigned int NUM_SPINS = 10000;

  for (unsigned int i = 0; i < NUM_SPINS; ++i) {
    a_.HandleEvent(BILL_IN);
    a_.HandleEvent(BET_MAX);
    a_.HandleEvent(REELS_STOPPED);
  }

  SQLite::Statement query(*db_, "SELECT sum(bet_credits), "
      "sum(won_credits), sum(bet_cents), sum(won_cents), "
      "sum(won_cents) * 1.0 / sum(bet_cents) * 100.0 FROM spins");
  while (query.executeStep()) {
    std::cout << "\nResults: \n" << std::endl;
    std::cout << "Bet Credits: " << query.getColumn(0) << std::endl;
    std::cout << "Won Credits: " << query.getColumn(1) << std::endl;
    std::cout << "Bet Cents: " << query.getColumn(2) << std::endl;
    std::cout << "Won Cents: " << query.getColumn(3) << std::endl;
    std::cout << "Pay Percentage: " << query.getColumn(4) << std::endl;
  }
}
#endif
