#include <cassert>

#include "frequency_stack.h"

int main() {
  FrequencyStack fs;

  for (int i = 0; i < 5; ++i) {
    fs.push(9);
  }
  assert(fs.maximumFrequency() == 5);

  int popped = 0;
  while (!fs.empty()) {
    assert(fs.pop() == 9);
    ++popped;
  }
  assert(popped == 5);
}
