#ifndef REEL_H
#define REEL_H

#include "random.h"
#include <map>

enum Symbol {
  CHERRY,             //0
  BAR,                //1
  DOUBLE_BAR,         //2
  TEN,                //3 
  JACK,               //4 
  QUEEN,              //5
  KING,               //6 
  ACE,                //7 
  WILD,               //8
  BONUS,              //9
  JACKPOT,            //10
  ALT1,
  ALT2,
  ALT3,
  ALT4,
  ALT5,
  ALT6,
  ALT7,
  ALT8,
  ALT9,
  NOTHING,
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
