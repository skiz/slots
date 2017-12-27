#include "reel.h"
#include <iostream>

std::map<Symbol, int> Reel::standardReelWeights = {
  {CHERRY,     10000},
  {BAR,        13650},
  {DOUBLE_BAR, 13550},
  {TEN,        13450},
  {JACK,       13350},
  {QUEEN,      13250},
  {KING,       13150},
  {ACE,        13050},
  {WILD,        3000},
  {BONUS,       5000},
  {JACKPOT,      200},
  {ALT1,        2500},
  {ALT2,        1500},
  {ALT3,       13000},
  {ALT4,       13000},
  {ALT5,           0},
  {ALT6,           0},
  {ALT7,           0},
  {ALT8,           0},
  {ALT9,           0},
  {FREE_SPIN,      0},
  {NOTHING,        0},
};

// Standard payout table  <count, <num, payout>>
std::map<Symbol, std::map<int, int>> Reel::payoutTable = {
  {CHERRY,      {{1,  1}, {2, 1},{3,    2},{4,     4},{5,     5}}},
  {BAR,                         {{3,    5},{4,    20},{5,    50}}},
  {DOUBLE_BAR,                  {{3,   10},{4,    25},{5,   100}}},
  {TEN,			        {{3,   20},{4,    50},{5,   200}}},
  {JACK,		        {{3,   30},{4,    75},{5,   300}}},
  {QUEEN,		        {{3,   40},{4,   100},{5,   400}}},
  {KING,		        {{3,   60},{4,   125},{5,   500}}},
  {ACE,			        {{3,   80},{4,   250},{5,  1000}}},
  {WILD,		        {                     {5,  1000}}},
  {JACKPOT,		        {                     {5,  5000}}},
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

// Compatible symbols with selected payout.  Used for matching bars and
// lowering the winning amount to only bar when mixed with double bars.
std::map<Symbol, std::map<Symbol, Symbol>> Reel::compatibleSymbols = {
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
std::map<int, std::array<int,5>> Reel::payLines = {
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


Reel::Reel() {
  random_ = &Random::GetInstance();
  //GenerateSymbols(5, 3);
  for (int i = 0; i < 15; i++) {
    symbols[i] = NOTHING;
  }
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

void Reel::GenerateWinningLines(int maxLines) {
  /*
   * map<int line, int payout>
   *
   * Loop through each payline up to maxLines
   *   loop through each position, holding first face until no match 
   *   (this includes wild, and if first is wild, account for this)
   *   if there is a matching result in the payout table, apply each
   *   winning line to an internal map of line -> payout.
   *
   *   Also TODO:
   *    wilds do not count for free spins or bonus.
   *	trigger update to accounting system.
   *	Support one offs like a full house or a straight
   */

  winningLines.clear();
  payout = 0;
  int wilds = 0;
  for (auto line : payLines) {                // check every payline until stopped
    if (line.first == maxLines) break;        // stop if we are exceeding maxLines
    int matches = 0;                          // number of matches on this line thus far
    int symbol = -1;                          // the current symbol we are matching against
    for (auto target : line.second) {         // target is the line position we need to check
      int m = matches;                        // for tracking changes to matches
      if (symbol == -1) {                     // this is our first match so its always good
	if (symbols[target] != WILD) {        // Wilds are skipped if first symbol
	  symbol = symbols[target];           // this is our current match
	} else {
	  wilds++;
	}
	matches++;                            // Increment number of matches for first item
      } else if (symbols[target] == symbol) {
	matches++;
      } else if (symbols[target] == WILD) {       // We have another match
	wilds++;
        if (symbol != BONUS && symbol != FREE_SPIN) {
	  matches++;
	}
      } else {				      // check for compatibles
	for (auto s : compatibleSymbols) {
	  if (symbols[target] == s.first) {   // this symbol is compatible
	    for (auto p : s.second) {         // check all related for match
	      if (p.first == symbol) {
		symbol = p.second;            // we are only paying for the compatible value
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
	int line_payout = 0;
	int paid = 0;
	Symbol sym = Symbol(symbol);             // Get the actual symbol reference
	std::map<int, int>::iterator it = payoutTable[sym].find(matches);
	if(it != payoutTable[sym].end()) {
	  paid = 1;
	  line_payout = it->second;              // We have a winner!
	}
	if (paid) {
	  winningLines[line.first] = line_payout; // Add the winning line to the result
	  payout += line_payout;
	}
	break;
      }
    }
  }
}

void Reel::DumpLines() {
  std::cout << "Winning Lines: " << winningLines.size() << std::endl;
  for (auto w : winningLines) {
    std::cout << "Payline " << w.first << ": " << w.second << " Credits"<< std::endl;
  }
  std::cout << "Total Payout: " << GetCreditsWon() << std::endl;;
  std::cout << symbols[0] << "\t" << symbols[1] << "\t" << symbols[2] << "\t" << symbols[3] << "\t" << symbols[4] << std::endl;
  std::cout << symbols[5] << "\t" << symbols[6] << "\t" << symbols[7] << "\t" << symbols[8] << "\t" << symbols[9] << std::endl;
  std::cout << symbols[10] << "\t" << symbols[11] << "\t" << symbols[12] << "\t" << symbols[13] << "\t" << symbols[14] << std::endl;
}

int Reel::GetCreditsWon() {
  return payout;
}
