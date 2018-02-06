#include "reel.h"
#include <iostream>
#include <algorithm>

/*
// NEW FEATURES - STAY TUNED!
enum SpinTypes {
  STANDARD_SPIN,    // Main game spin
  SLOW_STOP_SPIN,   // Slow down slowly - Possible Big win
  FAST_SPIN,        // Overdrive - Possible Bonus (highlight bonus) fast spin & longer wait.
  JACKPOT_SPIN,     // Flashing - Possible Jackpot (highlight jackpot)
  BONUS_SPIN_WAIT,  // 
  LOCKING_WILDS     // Wilds are locked and flashing.
};

enum ForcedResult {
  STANDARD_GAME,      // Standard game
  BONUS_GAME_1,       // Some kind of touch game
  BONUS_GAME_2,       // Another touch game (3 matches)
  FREE_SPINS_GAME,    // Free spins for X turns at current bet
  LOCKING_WILDS_GAME, // Another 
};

std::map< > Reel::spin_type_weights_ = {
  NEAR_MISS_
  SLOW_SPIN_
}
*/

std::map<Symbol, int> Reel::standard_reel_weights_ = {
  {CHERRY,     13000},
  {BAR,        13650},
  {DOUBLE_BAR, 13550},
  {TEN,        13450},
  {JACK,       13350},
  {QUEEN,      13250},
  {KING,       13150},
  {ACE,        10050},
  {WILD,       16600},
  {BONUS,      10000},
  {JACKPOT,      200},
  {ALT1,       13500},
  {ALT2,        8000},
  {ALT3,       13500},
  {ALT4,       13500},
  {ALT5,           0},
  {ALT6,           0},
  {ALT7,           0},
  {ALT8,           0},
  {ALT9,           0},
  {FREE_SPIN,      0},
  {NOTHING,        0},
};

// Standard payout_ table  <count, <num, payout_>>
std::map<Symbol, std::map<int, int>> Reel::payout_table_ = {
  {CHERRY,      {{1,  1}, {2, 2},{3,    3},{4,     4},{5,     5}}},
  {BAR,                         {{3,    5},{4,    20},{5,    50}}},
  {DOUBLE_BAR,                  {{3,   10},{4,    25},{5,   100}}},
  {TEN,			                    {{3,   20},{4,    50},{5,   200}}},
  {JACK,		                    {{3,   30},{4,    75},{5,   300}}},
  {QUEEN,		                    {{3,   40},{4,   100},{5,   400}}},
  {KING,		                    {{3,   60},{4,   125},{5,   500}}},
  {ACE,			                    {{3,   80},{4,   250},{5,  1000}}},
  {WILD,		                    {                     {5,  1000}}},
  {JACKPOT,		                  {                     {5,  5000}}},
  {ALT1,                        {{3,   50},{4,   250},{5,   500}}},
  {ALT2,                        {{3,  100},{4,   500},{5,  1000}}},
  {ALT3,                        {{3,    1},{4,     2},{5,     3}}},
  {ALT4,                        {{3,    1},{4,     2},{5,     3}}},
  {ALT5,                        {{3,    1},{4,     2},{5,     3}}},
  {ALT6,                        {{3,    2},{4,     4},{5,     6}}},
  {ALT7,                        {{3,    2},{4,     4},{5,     6}}},
  {ALT8,                        {{3,    2},{4,     4},{5,     6}}},
  {ALT9,                        {{3,    2},{4,     4},{5,     6}}},
  {FREE_SPIN,                   {{3,    0},{4,     0},{5,     0}}},
  {BONUS,                       {{3,    0},{4,     0},{5,     0}}},
};

// Compatible symbols_ with selected payout_.  Used for matching bars and
// lowering the winning amount to only bar when mixed with double bars.
std::map<Symbol, std::map<Symbol, Symbol>> Reel::compatible_symbols_ = {
  {BAR, {{DOUBLE_BAR, BAR}}},
  {DOUBLE_BAR, {{BAR, BAR}}},
};

// Paylines are mapped on a 5x3 grid starting at the top left
// and continuing to the right and are zero indexed.
//
// 0   1  2  3  4
// 5   6  7  8  9
// 10 11 12 13 14
//
// These lines can be seen in assets/20-paylines.gif
//
std::map<int, std::array<int,5>> Reel::paylines_ = {
  { 0, {{ 5, 6, 7, 8, 9}}},
  { 1, {{ 0, 1, 2, 3, 4}}},
  { 2, {{10,11,12,13,14}}},
  { 3, {{ 0, 6,12, 8, 4}}},
  { 4, {{10, 6, 2, 8,14}}},
  { 5, {{ 5, 1, 2, 3, 9}}},
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

std::array<uint32_t, 20> Reel::payline_colors_ {
  0xFFF8FF00, 0xFF00950A, 0xFF00EC17, 0xFF00BCFA, 0xFFCD00FF,
  0xFF4600FF, 0xFFFF00FB, 0xFFFF9700, 0xFFFF0000, 0xFFB1A129,
  0xFFA800FF, 0xFFFF9EFF, 0xFF00FFC1, 0xFFFFAC4C, 0xFFA80000,
  0xFFFFE000, 0xFFE00000, 0xFF9A6534, 0xFF34961D, 0xFFF7003D
};


Reel::Reel() {
  random_ = &Random::GetInstance();
  for (int i = 0; i < 15; i++) {
    symbols_[i] = NOTHING;
  }
}

uint32_t Reel::GetColorForPayline(int payline) {
  if (payline < 0 || payline >= 20) {
    return 0x0;
  }
  return payline_colors_[payline];
};

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
    symbols_[i] = GetSymbol(&standard_reel_weights_);
  }
}

void Reel::GenerateWinningLines(int maxLines) {
  /*
   * map<int line, int payout_>
   *
   * Loop through each payline up to maxLines
   *   loop through each position, holding first face until no match 
   *   (this includes wild, and if first is wild, account for this)
   *   if there is a matching result in the payout_ table, apply each
   *   winning line to an internal map of line -> payout_.
   *
   *   Also TODO:
   *   * ensure all winning_lines_ also have a representitive winning_symbols_
   *     entry that includes the symbols that were matched for each payline. 
   *	 * Support one offs like a full house or a straight if enabled
   */
  winning_symbols_.clear();
  winning_lines_.clear();
  payout_ = 0;
  int wilds = 0;
  for (auto line : paylines_) {               // check every payline until stopped
    if (line.first == maxLines) break;        // stop if we are exceeding maxLines
    int matches = 0;                          // number of matches on this line thus far
    std::vector<int> syms;                    // matching symbol positions for this line
    int symbol = -1;                          // the current symbol we are matching against
    for (auto target : line.second) {         // target is the line position we need to check
      int m = matches;                        // for tracking changes to matches
      if (symbol == -1) {                     // this is our first match so its always good
        if (symbols_[target] != WILD) {       // Wilds are skipped if first symbol
          symbol = symbols_[target];          // this is our current match
        } else {
          wilds++;
        }
        syms.push_back(target);               // store matching symbol
        matches++;                            // Increment number of matches for first item
      } else if (symbols_[target] == symbol) {
        syms.push_back(target);               // store matching symbol
        matches++;
      } else if (symbols_[target] == WILD) {       // We have another match
        wilds++;
        if (symbol != BONUS && symbol != FREE_SPIN) {
          matches++;
          syms.push_back(target);               // store matching symbol
        }
      } else {				                          // check for compatibles
        for (auto s : compatible_symbols_) {
          if (symbols_[target] == s.first) {   // this symbol is compatible
            for (auto p : s.second) {         // check all related for match
              if (p.first == symbol) {
                symbol = p.second;            // we are only paying for the compatible value
                syms.push_back(line.first);               // store matching symbol
                matches++;
                break;
              }
            }
          }
          if (m != matches) break;           // break second loop if we found a match
        }
      }

      if (m == matches || matches == 5) {    // no more matches. we are done with this line.

        if (matches == 5 && symbol == -1) {   // all wilds.
          symbol = WILD;
        }

        // dont count wilds for free spins and bonuses
        if (wilds > 0 && (symbol == BONUS || symbol == FREE_SPIN)) {
          continue;
        }

        //check for matching pay table entry
        int line_payout_ = 0;
        int paid = 0;
        Symbol sym = Symbol(symbol);             // Get the actual symbol reference
        std::map<int, int>::iterator it = payout_table_[sym].find(matches);
        if(it != payout_table_[sym].end()) {
          paid = 1;
          line_payout_ = it->second;              // We have a winner!
        }
        if (paid) {
          winning_lines_[line.first] = line_payout_; // Add the winning line to the result
          winning_symbols_[line.first] = syms;
          payout_ += line_payout_;
          //DumpLines();
        }
        break;
      }
    }
  }
}

int Reel::GetCreditsPaidForLine(int index) {
  return winning_lines_[index];
}

void Reel::DumpLines() {
  std::cout << "Winning Lines: " << winning_lines_.size() << std::endl;
  for (auto w : winning_lines_) {
    std::cout << "Payline " << w.first << ": " << w.second << " Credits"<< std::endl;
    std::cout << "Winning Symbol Positions: ";
    for (auto s : winning_symbols_[w.first]) {
      std::cout << s << " ";
    }
    std::cout << std::endl;

  }
  std::cout << "Payout (without bet multiplier): " << GetCreditsWon() << std::endl;;
  std::cout << symbols_[0] << "\t" << symbols_[1] << "\t" << symbols_[2] << "\t" << symbols_[3] << "\t" << symbols_[4] << std::endl;
  std::cout << symbols_[5] << "\t" << symbols_[6] << "\t" << symbols_[7] << "\t" << symbols_[8] << "\t" << symbols_[9] << std::endl;
  std::cout << symbols_[10] << "\t" << symbols_[11] << "\t" << symbols_[12] << "\t" << symbols_[13] << "\t" << symbols_[14] << std::endl;
  std::cout << std::endl << "---------------------" << std::endl;
}

int Reel::GetCreditsWon() {
  return payout_;
}

std::map<int, Symbol> Reel::GetSymbols() {
  return symbols_;
}

std::map<int, std::array<int,5>> Reel::GetPaylines() {
  return paylines_;
}

std::array<int,5> Reel::GetPayline(int index) {
  return paylines_[index];
}

std::map<int, std::vector<int>> Reel::GetWinningPaylinePositions() {
  return winning_symbols_;
}

std::vector<int> Reel::GetWinningPositionsForPayline(int payline) {
  return winning_symbols_.at(payline);
}

std::vector<int> Reel::GetWinningLines() {
  std::vector<int> v;
  for(std::map<int,int>::iterator it = winning_lines_.begin(); it != winning_lines_.end(); ++it) {
    v.push_back(it->first);
  }
  return v;
}

std::map<Symbol, int> Reel::GetReelWeights() {
  return standard_reel_weights_;
}

std::map<Symbol, std::map<int, int>> Reel::GetPayoutTable() {
  return payout_table_;
}

void Reel::SetSymbol(Symbol sym, int pos) {
  symbols_[pos] = sym;
}
