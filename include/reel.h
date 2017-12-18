#ifndef REEL_H
#define REEL_H

#include "random.h"
#include <map>

enum Symbol {
  CHERRY,
  BAR,
  DOUBLE_BAR,
  TEN,
  JACK,
  QUEEN,
  KING,
  ACE,
  WILD,
  BONUS,
  JACKPOT
};
 
class Reel {
  public:
    Reel();
    Symbol GetSymbol(std::map<Symbol, int> *weightedSet);
    void GenerateSymbols(int reels, int spots);
    static std::map<Symbol, int> standardReelWeights;
    std::map<int, Symbol> symbols;
  private:
    Random* random_;
};

#endif
