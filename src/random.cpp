#include "random.h"
#include "randutils.h"

int Random::Generate(int max) {
  return rng.uniform(0,max);
}
