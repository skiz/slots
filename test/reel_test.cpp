#include "reel.h"
#include <gtest/gtest.h>
#include <map>
#include <iostream>

typedef std::map<Symbol, int>  ResultCounter;

TEST(ReelTest, GenerateSymbols) {
  Reel r;
  r.GenerateSymbols(5, 3);
  ASSERT_EQ(r.GetSymbols().size(), 15);
}

TEST(ReelTest, PayoutTable) {
  Reel r;
  ASSERT_EQ(r.GetPayoutTable()[CHERRY][5], 5);
  ASSERT_EQ(r.GetPayoutTable()[JACKPOT][5], 5000);
}

TEST(ReelTest, VerifyAllCherriesWin) {
  Reel r;
  for (int i = 0; i < 15; i++) {
    r.SetSymbol(CHERRY, i);
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
  r.SetSymbol(JACKPOT, 0);
  r.SetSymbol(JACKPOT, 1);
  r.SetSymbol(JACKPOT, 2);
  r.SetSymbol(JACKPOT, 3);
  r.SetSymbol(JACKPOT, 4);
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, VerifyJackpot) {
  Reel r;
  r.SetSymbol(JACKPOT, 0);
  r.SetSymbol(JACKPOT, 1);
  r.SetSymbol(JACKPOT, 2);
  r.SetSymbol(JACKPOT, 3);
  r.SetSymbol(JACKPOT, 4);
  r.GenerateWinningLines(2);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(5000, r.GetCreditsWon());
}

TEST(ReelTest, WildWildCherry) {
  Reel r;
  r.SetSymbol(WILD, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(CHERRY, 7);
 
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(3, r.GetCreditsWon());
}

TEST(ReelTest, WildCherryCherry) {
  Reel r;
  r.SetSymbol(WILD, 10);
  r.SetSymbol(CHERRY, 11);
  r.SetSymbol(CHERRY, 12);
 
  r.GenerateWinningLines(3);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(3, r.GetCreditsWon());
}

TEST(ReelTest, ThreeTens) {
  Reel r;
  r.SetSymbol(TEN, 5);
  r.SetSymbol(TEN, 6);
  r.SetSymbol(TEN, 7);
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(20, r.GetCreditsWon());

}

TEST(ReelTest, FourTens) {
  Reel r;
  r.SetSymbol(TEN, 5);
  r.SetSymbol(TEN, 6);
  r.SetSymbol(TEN, 7);
  r.SetSymbol(TEN, 8);
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(50, r.GetCreditsWon());
}

TEST(ReelTest, FiveTens) {
  Reel r;
  r.SetSymbol(TEN, 5);
  r.SetSymbol(TEN, 6);
  r.SetSymbol(TEN, 7);
  r.SetSymbol(TEN, 8);
  r.SetSymbol(TEN, 9);
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(200, r.GetCreditsWon());
}


TEST(ReelTest, MixedBars) {
  Reel r;
  r.SetSymbol(BAR, 0);
  r.SetSymbol(DOUBLE_BAR, 1);
  r.SetSymbol(DOUBLE_BAR, 2);
  r.SetSymbol(BAR, 3);
  r.SetSymbol(BAR, 4);
  r.GenerateWinningLines(2);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(50, r.GetCreditsWon());
}

TEST(ReelTest, NotMixedBars) {
  Reel r;
  r.SetSymbol(Symbol(2), 1);
  r.SetSymbol(Symbol(1), 5);

  r.GenerateWinningLines(20);
  ASSERT_EQ(0, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, WildMixedBars) {
  Reel r;
  r.SetSymbol(WILD, 5);
  r.SetSymbol(BAR, 6);
  r.SetSymbol(DOUBLE_BAR, 7);

  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(5, r.GetCreditsWon());
}

TEST(ReelTest, ZeroSumBonus) {
  Reel r;
  r.SetSymbol(BONUS, 5);
  r.SetSymbol(BONUS, 6);
  r.SetSymbol(BONUS, 7);
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, NoWildBonus) {
  Reel r;
  r.SetSymbol(BONUS, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(BONUS, 7);

  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());
}

TEST(ReelTest, NoWildFreeSpins) {
  Reel r;
  r.SetSymbol(FREE_SPIN, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(FREE_SPIN, 7);
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());

  r.SetSymbol(FREE_SPIN, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(WILD, 7);
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());

  r.SetSymbol(WILD, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(FREE_SPIN, 7);
  r.GenerateWinningLines(1);
  ASSERT_EQ(0, r.GetWinningLines().size());
  ASSERT_EQ(0, r.GetCreditsWon());

}

TEST(ReelTest, FiveWildPaysAce) {
  Reel r;
  r.SetSymbol(WILD, 5);
  r.SetSymbol(WILD, 6);
  r.SetSymbol(WILD, 7);
  r.SetSymbol(WILD, 8);
  r.SetSymbol(WILD, 9);
  r.GenerateWinningLines(1);
  ASSERT_EQ(1, r.GetWinningLines().size());
  ASSERT_EQ(1000, r.GetCreditsWon());

}

TEST(ReelTest, BonusMapping) {
  Reel r;
  r.SetSymbol(BONUS, 5);
//  r.SetSymbol(BONUS, 6);
  r.SetSymbol(BONUS, 7);
  r.SetSymbol(BONUS, 8);
  r.SetSymbol(BONUS, 9);
 // r.GenerateWinningLines(1);
 // ASSERT_EQ(1, r.GetWinningLines().size());
 // ASSERT_EQ(0, r.GetCreditsWon());
 
  r.GenerateBonusMapping(1);
}
