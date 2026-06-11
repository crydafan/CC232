#include <cassert>
#include <random>

#include "frequency_stack.h"
#include "frequency_stack_naive.h"

int main() {
  std::mt19937 rng(123456789u);
  std::uniform_int_distribution<int> coin(0, 99);
  std::uniform_int_distribution<int> value(0, 7);

  const int kSeq = 40;
  const int kOps = 600;

  for (int seq = 0; seq < kSeq; ++seq) {
    FrequencyStack fs;
    FrequencyStackNaive naive;
    int vivos = 0;

    for (int op = 0; op < kOps; ++op) {
      bool doPush = (coin(rng) < 65) || vivos == 0;
      if (doPush) {
        int v = value(rng);
        fs.push(v);
        naive.push(v);
        ++vivos;
      } else {
        assert(fs.pop() == naive.pop());
        --vivos;
      }
      assert(fs.size() == naive.size());
    }
  }
}
