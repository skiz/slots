#include "highlighter.h"
#include <array>
#include <bitset>
#include <iostream>


const std::array<std::bitset<15>, 20> Highlighter::paylines_ = {
  { 0b000001111100000,
    0b000000000011111,
    0b111110000000000,
    0b001000101010001,
    0b100010101000100,
    0b000001000101110,
    0b011101000100000,
    0b110000010000011,
    0b000110010011000,
    0b000101010101000,
    0b010001010100010,
    0b000000111010001,
    0b100010111000000,
    0b000000101010101,
    0b101010101000000,
    0b000001101100100,
    0b001001101100000,
    0b001000000011011,
    0b110110000000100,
    0b011100000010001,
  }};

const std::bitset<15> Highlighter::column_mask_ {0b000010000100001};
const std::bitset<15> Highlighter::left3_mask_  {0b001110011100111};
const std::bitset<15> Highlighter::left4_mask_  {0b011110111101111};

std::array<std::bitset<15>, 5> Highlighter::Generate(std::bitset<15> bits,
    int maxLines) {

  bool no_more = false;
  std::array<std::bitset<15>, 5> results;
  std::bitset<15> cur, tmp, wins, mask(column_mask_);
  for (int i = 0; i < 5; ++i) {                            
    cur.reset();                                           
    for (int p = 0; p < (int)paylines_.size(); ++p) {
      if (p == maxLines || no_more) {
        break;
      }
      cur |= bits & paylines_[p] & column_mask_ << i;

      if ((bits & paylines_[p] &
            left3_mask_).count() == 3) {               
        tmp = bits & paylines_[p];                     
        if ((bits & paylines_[p] & 
              left4_mask_).count() == 3) {             
          tmp &= left3_mask_;                          
        }
        wins |= tmp;
      }
    }
    if (wins == 0b0) { no_more = true; }
    results[i] = cur | wins & mask;
    mask |= mask << 1;                                 
  }
  results[4] = wins;                                   
  return results;
}

void Highlighter::DumpBits(std::bitset<15> b) {
  for (int i = 0; i < 3; ++i) {
    for (int z = 0; z < 5; ++z) {
      std::cout << b[i*5+z] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

