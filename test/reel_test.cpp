#include "reel.h"
#include <gtest/gtest.h>
#include <map>
#include <iostream>

typedef std::map<Symbol, int>  ResultCounter;

TEST(ReelTest, WeightedSet) {
  Reel r;
  ResultCounter ctr;

  for (int i = 0; i < 10000; ++i) {
    Symbol s = r.GetSymbol(&r.standardReelWeights);
    ctr[s]++;
  }

  for (auto t : ctr) {
    std::cout << t.first << " = " << t.second << std::endl;
  }
}

TEST(ReelTest, GenerateSymbols) {
  Reel r;
  r.GenerateSymbols(5, 3);
  ASSERT_EQ(r.symbols.size(), 15);

  for (auto t : r.symbols) {
    std::cout << t.first << " = " << t.second << std::endl;
  }
}

TEST(ReelTest, PayoutTable) {
  Reel r;
  ASSERT_EQ(r.payoutTable[CHERRY][5], 250);
  ASSERT_EQ(r.payoutTable[JACKPOT][4], 10000);
}

TEST(ReelTest, VerifyAllCherriesWin) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = CHERRY;
  }
  r.GenerateWinningLines(20);
  ASSERT_EQ(250*20, r.GetCreditsWon());
}

TEST(ReelTest, VerifyAllNothing) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.GenerateWinningLines(20);
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, VerifyMissedJackpot) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[0] = JACKPOT;
  r.symbols[1] = JACKPOT;
  r.symbols[2] = JACKPOT;
  r.symbols[3] = JACKPOT;
  r.symbols[4] = JACKPOT;
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, VerifyJackpot) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[0] = JACKPOT;
  r.symbols[1] = JACKPOT;
  r.symbols[2] = JACKPOT;
  r.symbols[3] = JACKPOT;
  r.symbols[4] = JACKPOT;
  r.GenerateWinningLines(2);
  ASSERT_EQ(50000, r.GetCreditsWon());
}
