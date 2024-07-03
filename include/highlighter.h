#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <array>
#include <bitset>
#include <map>
#include "reel.h"

/* Calculates highlights for upcoming matches.  Used when first reel hits (ie bonus)
 * so that we know what to highlight until the next reel stops. 1 for each reel stop.
 * Wins will remain highlighted throughout the reel results. Possible wins that turned
 * in to loses will no longer be highlighted.  The result of this function provides
 * a highlight map for each of the 5 spin stops of the reels.
 */
class Highlighter
{
public:
  static std::array<std::bitset<15>, 5> Generate(std::bitset<15> bits,
                                                 int maxLines);
  static std::array<std::bitset<15>, 5> GenerateFromReelSymbols(
      std::map<int, Symbol> symbols, Symbol sym, int maxLines);
  static void DumpBits(std::bitset<15> bits);

private:
  static const std::array<std::bitset<15>, 20> paylines_;
  static const std::bitset<15> column_mask_;
  static const std::bitset<15> left3_mask_;
  static const std::bitset<15> left4_mask_;
};
#endif
