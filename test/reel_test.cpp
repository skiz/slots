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
   // std::cout << t.first << " = " << t.second << std::endl;
  }
}

TEST(ReelTest, GenerateSymbols) {
  Reel r;
  r.GenerateSymbols(5, 3);
  ASSERT_EQ(r.symbols.size(), 15);
}

TEST(ReelTest, PayoutTable) {
  Reel r;
  ASSERT_EQ(r.payoutTable[CHERRY][5], 5);
  ASSERT_EQ(r.payoutTable[JACKPOT][5], 5000);
}

TEST(ReelTest, VerifyAllCherriesWin) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = CHERRY;
  }
  r.GenerateWinningLines(20);
  ASSERT_EQ(5*20, r.GetCreditsWon());
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
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(5000, r.GetCreditsWon());
}

TEST(ReelTest, WildWildCherry) {
  Reel r;
  r.symbols[0] = BONUS;
  r.symbols[1] = TEN;
  r.symbols[2] = ACE;
  r.symbols[3] = QUEEN;
  r.symbols[4] = ACE;
  r.symbols[5] = WILD;
  r.symbols[6] = WILD;
  r.symbols[7] = CHERRY;
  r.symbols[8] = BAR;
  r.symbols[9] = BONUS;
  r.symbols[10] = JACK;
  r.symbols[11] = JACK;
  r.symbols[12] = ACE;
  r.symbols[13] = WILD;
  r.symbols[14] = CHERRY;
 
  r.GenerateWinningLines(20);
  ASSERT_EQ(3, r.winningLines.size());
  ASSERT_EQ(22, r.GetCreditsWon());
}

TEST(ReelTest, ThreeTens) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.GenerateWinningLines(1);
  //r.DumpLines();
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(5, r.GetCreditsWon());

}

TEST(ReelTest, FourTens) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.symbols[8] = TEN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(10, r.GetCreditsWon());
}

TEST(ReelTest, FiveTens) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.symbols[8] = TEN;
  r.symbols[9] = TEN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(50, r.GetCreditsWon());
}


TEST(ReelTest, MixedBars) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }
  r.symbols[0] = BAR;
  r.symbols[1] = DOUBLE_BAR;
  r.symbols[2] = DOUBLE_BAR;
  r.symbols[3] = BAR;
  r.symbols[4] = BAR;
  r.GenerateWinningLines(2);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(10, r.GetCreditsWon());
}

TEST(ReelTest, NotMixedBars) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.symbols[i] = NOTHING;
  }

  r.symbols[1] = Symbol(2);

  r.symbols[5] = Symbol(1);

  r.GenerateWinningLines(20);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

// WildMixedBars
