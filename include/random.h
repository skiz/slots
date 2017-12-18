#ifndef RANDOM_H
#define RANDOM_H

#include "randutils.h"

class Random {
  public:
    static Random& GetInstance() {
      static Random instance;
      return instance;
    }
    ~Random() {};

    int Generate(int max);
    
    Random(Random const&) = delete;
    void operator=(Random const&) = delete;

  private:
    Random() {
      randutils::mt19937_rng rng;
    };
    static Random* instance;
    randutils::mt19937_rng rng;
};
#endif
