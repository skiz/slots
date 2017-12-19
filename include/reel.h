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
  JACKPOT,
  NOTHING
};

class Reel {
  public:
    Reel();

    void GenerateWinningLines(int maxLines);
    
    Symbol GetSymbol(std::map<Symbol, int> *weightedSet);
    void SetSymbol(Symbol sym, int pos);
    void GenerateSymbols(int reels, int spots);
    int GetCreditsWon();
    void DumpLines();
    static std::map<Symbol, int> standardReelWeights;
    static std::map<Symbol, std::map<int, int>> payoutTable;
    static std::map<int, std::array<int,5>> payLines;
    std::map<int, Symbol> symbols;
    std::map<int, int> winningLines;
    int payout;
  private:
    Random* random_;
};

#endif
