#ifndef REEL_H
#define REEL_H

#include "random.h"
#include <map>
#include <vector>

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
  FREE_SPIN,
  NOTHING,
};

class Reel {
  public:
    Reel();
    void GenerateWinningLines(int maxLines);
    Symbol GetSymbol(std::map<Symbol, int> *weightedSet);
    std::map<int, Symbol> GetSymbols();
    std::map<int, std::array<int,5>> GetPaylines();
    std::map<int, std::vector<int>> GetWinningPaylinePositions();
    std::map<Symbol, std::map<int, int>> GetPayoutTable();
    std::vector<int> GetWinningPositionsForPayline(int payline);
    std::vector<int> GetWinningLines();
    void SetSymbol(Symbol sym, int pos);
    void GenerateSymbols(int reels, int spots);
    int GetCreditsWon();
    void DumpLines();
    std::map<Symbol, int> GetReelWeights();
    uint32_t GetColorForPayline(int payline);
  private:
    static std::array<uint32_t, 20> payline_colors_;
    static std::map<Symbol, int> standard_reel_weights_;
    static std::map<Symbol, std::map<int, int>> payout_table_;
    static std::map<Symbol, std::map<Symbol, Symbol>> compatible_symbols_;
    static std::map<int, std::array<int,5>> paylines_; // symbol[count] = win
    std::map<int, Symbol> symbols_; // keyed by reel position (could be a vector...)
    std::map<int, int> winning_lines_; // payline -> payout
    std::map<int, std::vector<int>> winning_symbols_; // line -> [positions]
    int payout_;
    Random* random_;
};

#endif
