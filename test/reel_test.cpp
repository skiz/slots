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
  r.GenerateWinningLines(20);
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, VerifyMissedJackpot) {
  Reel r;
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
  r.symbols[5] = WILD;
  r.symbols[6] = WILD;
  r.symbols[7] = CHERRY;
 
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(2, r.GetCreditsWon());
}

TEST(ReelTest, ThreeTens) {
  Reel r;
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(20, r.GetCreditsWon());

}

TEST(ReelTest, FourTens) {
  Reel r;
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.symbols[8] = TEN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(50, r.GetCreditsWon());
}

TEST(ReelTest, FiveTens) {
  Reel r;
  r.symbols[5] = TEN;
  r.symbols[6] = TEN;
  r.symbols[7] = TEN;
  r.symbols[8] = TEN;
  r.symbols[9] = TEN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(200, r.GetCreditsWon());
}


TEST(ReelTest, MixedBars) {
  Reel r;
  r.symbols[0] = BAR;
  r.symbols[1] = DOUBLE_BAR;
  r.symbols[2] = DOUBLE_BAR;
  r.symbols[3] = BAR;
  r.symbols[4] = BAR;
  r.GenerateWinningLines(2);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(50, r.GetCreditsWon());
}

TEST(ReelTest, NotMixedBars) {
  Reel r;
  r.symbols[1] = Symbol(2);
  r.symbols[5] = Symbol(1);

  r.GenerateWinningLines(20);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, WildMixedBars) {
  Reel r;
  r.symbols[5] = WILD;
  r.symbols[6] = BAR;
  r.symbols[7] = DOUBLE_BAR;

  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(5, r.GetCreditsWon());
}

TEST(ReelTest, ZeroSumBonus) {
  Reel r;
  r.symbols[5] = BONUS;
  r.symbols[6] = BONUS;
  r.symbols[7] = BONUS;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, NoWildBonus) {
  Reel r;
  r.symbols[5] = BONUS;
  r.symbols[6] = WILD;
  r.symbols[7] = BONUS;

  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, NoWildFreeSpins) {
  Reel r;
  r.symbols[5] = FREE_SPIN;
  r.symbols[6] = WILD;
  r.symbols[7] = FREE_SPIN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());

  r.symbols[5] = FREE_SPIN;
  r.symbols[6] = WILD;
  r.symbols[7] = WILD;;
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());

  r.symbols[5] = WILD;
  r.symbols[6] = WILD;
  r.symbols[7] = FREE_SPIN;
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.winningLines.size());
  ASSERT_EQ(0, r.GetCreditsWon());

}

TEST(ReelTest, FiveWildPaysAce) {
  Reel r;
  r.symbols[5] = WILD;
  r.symbols[6] = WILD;
  r.symbols[7] = WILD;
  r.symbols[8] = WILD;
  r.symbols[9] = WILD;
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.winningLines.size());
  ASSERT_EQ(1000, r.GetCreditsWon());

}


