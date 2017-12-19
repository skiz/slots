#include "reel.h"

std::map<Symbol, int> Reel::standardReelWeights = {
  {CHERRY,     13450},
  {BAR,        13450},
  {DOUBLE_BAR, 13450},
  {TEN,        13450},
  {JACK,       13300},
  {QUEEN,      13200},
  {KING,       13100},
  {ACE,        13000},
  {WILD,       11200},
  {BONUS,       5500},
  {JACKPOT,     1500}
};

// Standard payout table  <count, payout>
std::map<Symbol, std::map<int, int>> Reel::payoutTable = {
  {CHERRY,      {{1, 50},{2,100},{3,  150},{4,   200},{5,   250}}},
  {BAR,                         {{3,   10},{4,    20},{5,    30}}},
  {DOUBLE_BAR,                  {{3,   20},{4,    40},{5,    60}}},
  {TEN,			        {{3,   25},{4,    30},{5,    35}}},
  {JACK,		        {{3,   30},{4,    40},{5,    45}}},
  {QUEEN,		        {{3,   40},{4,    70},{5,    90}}},
  {KING,		        {{3,   50},{4,    90},{5,   110}}},
  {ACE,			        {{3,  100},{4,   150},{5,   200}}},
  {JACKPOT,		        {{3, 5000},{4, 10000},{5, 50000}}}
};


// Paylines are mapped on a 5x3 grid starting at the top left
// and continuing to the right and are zero indexed.
//
// 0   1  2  3  4
// 5   6  7  8  9
// 10 11 12 13 14
//
std::map<int, std::array<int,5>> Reel::payLines = {
  { 0, {{ 5, 6, 7, 8, 9}}},
  { 1, {{ 0, 1, 2, 3, 4}}},
  { 2, {{10,11,12,13,14}}},
  { 3, {{ 0, 6,12, 8, 4}}},
  { 4, {{10, 6, 2, 8,14}}},
  { 5, {{ 5, 2, 3, 4, 9}}},
  { 6, {{ 5,11,12,13, 9}}},
  { 7, {{ 0, 1, 7,13,14}}},
  { 8, {{10,11, 7, 3, 4}}},
  { 9, {{ 5,11, 7, 3, 9}}},
  {10, {{ 5, 1, 7,13, 9}}},
  {11, {{ 0, 6, 7, 8, 4}}},
  {12, {{10, 6, 7, 8,14}}},
  {13, {{ 0, 6, 2, 8, 4}}},
  {14, {{10, 6,12, 8,14}}},
  {15, {{ 5, 6, 2, 8, 9}}},
  {16, {{ 5, 6,12, 8, 9}}},
  {17, {{ 0, 1,12, 3, 4}}},
  {18, {{10,11, 2,13,14}}},
  {19, {{ 0,11,12,13, 4}}}
};



// TODO: calculate winning lines with wilds
//

Reel::Reel() {
  random_ = &Random::GetInstance();
}

Symbol Reel::GetSymbol(std::map<Symbol, int> *weightedSet) {
  int sum = 0; 
  for (auto const & pair : *weightedSet) sum += pair.second;
  sum = random_->Generate(sum);
  for (auto const & pair : *weightedSet) {
    if (sum < pair.second) {
      return pair.first;
    }
    sum -= pair.second;
  }
  return CHERRY;
}

void Reel::GenerateSymbols(int reels, int spots) {
  for (int i=0; i < reels*spots; ++i) {
    symbols[i] = GetSymbol(&standardReelWeights);
  }

}

