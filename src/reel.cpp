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

