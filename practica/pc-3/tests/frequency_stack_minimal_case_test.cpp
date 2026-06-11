#include <cassert>

#include "frequency_stack.h"

int main() {
  FrequencyStack fs;
  fs.push(42);

  assert(fs.size() == 1);
  assert(fs.maximumFrequency() == 1);
  assert(fs.pop() == 42);
  assert(fs.empty());
  assert(fs.checkInvariant());
}
