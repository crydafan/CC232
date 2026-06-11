#include <cassert>
#include <random>

#include "frequency_stack.h"

int main() {
  std::mt19937 rng(0xC0FFEEu);
  std::uniform_int_distribution<int> coin(0, 99);
  std::uniform_int_distribution<int> value(0, 11);

  FrequencyStack fs;
  int vivos = 0;

  for (int op = 0; op < 5000; ++op) {
    bool doPush = (coin(rng) < 60) || vivos == 0;
    if (doPush) {
      fs.push(value(rng));
      ++vivos;
    } else {
      fs.pop();
      --vivos;
    }
    assert(fs.checkInvariant());
  }
}
