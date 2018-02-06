#include "highlighter.h"
#include <gtest/gtest.h>
#include <array>
#include <bitset>

TEST(HighlighterTest, TestOne) {
  std::bitset<15> orig = 0b011110000110011;
  std::array<std::bitset<15>, 5> result = Highlighter::Generate(orig, 20);
  ASSERT_EQ(result[0], 0b000010000100001);
  ASSERT_EQ(result[1], 0b000110000100011);
  ASSERT_EQ(result[2], 0b001110000100011);
  ASSERT_EQ(result[3], 0b011110000100011);
  ASSERT_EQ(result[4], 0b011110000110011);
}

TEST(HighlighterTest, TestTwo) {
  std::bitset<15> orig = 0b100110010101001;
  std::array<std::bitset<15>, 5> result = Highlighter::Generate(orig, 20);
  ASSERT_EQ(result[0], 0b000010000100001);
  ASSERT_EQ(result[1], 0b000110000100000);
  ASSERT_EQ(result[2], 0b000110010100000);
  ASSERT_EQ(result[3], 0b000110010101000);
  ASSERT_EQ(result[4], 0b000110010101000);
}

TEST(HighlighterTest, TestLineMiss) {
  std::bitset<15> orig = 0b100110010101001;
  std::array<std::bitset<15>, 5> result = Highlighter::Generate(orig, 2);
  ASSERT_EQ(result[0], 0b000000000100001);
  ASSERT_EQ(result[1], 0b0);
  ASSERT_EQ(result[2], 0b0);
  ASSERT_EQ(result[3], 0b0);
  ASSERT_EQ(result[4], 0b0);
}

TEST(HighlighterTest, TestBetLineLimit) {
  std::bitset<15> orig = 0b111110000011111;
  std::array<std::bitset<15>, 5> result = Highlighter::Generate(orig, 2);
  ASSERT_EQ(result[0], 0b000000000000001);
  ASSERT_EQ(result[1], 0b000000000000011);
  ASSERT_EQ(result[2], 0b000000000000111);
  ASSERT_EQ(result[3], 0b000000000001111);
  ASSERT_EQ(result[4], 0b000000000011111);
}

TEST(HighlighterTest, GenerateFromReelSymbols) {
  Reel r;
  r.SetSymbol(BONUS, 5);
  r.SetSymbol(BONUS, 6);
  r.SetSymbol(BONUS, 7);
  r.SetSymbol(BONUS, 8);
  r.SetSymbol(BONUS, 9);

  std::array<std::bitset<15>, 5> result = 
    Highlighter::GenerateFromReelSymbols(r.GetSymbols(), BONUS, 20);

  ASSERT_EQ(result[0], 0b000000000100000);
  ASSERT_EQ(result[1], 0b000000001100000);
  ASSERT_EQ(result[2], 0b000000011100000);
  ASSERT_EQ(result[3], 0b000000111100000);
  ASSERT_EQ(result[4], 0b000001111100000);
}


