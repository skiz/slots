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

TEST(ReelTest, GenerateReels) {
  Reel r;
  r.GenerateSymbols(5, 3);
  ASSERT_EQ(r.symbols.size(), 15);

  for (auto t : r.symbols) {
    std::cout << t.first << " = " << t.second << std::endl;
  }
}
