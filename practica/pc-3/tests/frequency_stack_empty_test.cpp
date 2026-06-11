#include <cassert>

#include "frequency_stack.h"

int main() {
  FrequencyStack fs;

  assert(fs.empty());
  assert(fs.size() == 0);
  assert(fs.maximumFrequency() == 0);
  assert(fs.checkInvariant());

  fs.push(1);
  assert(!fs.empty());
  fs.pop();
  assert(fs.empty());
  assert(fs.maximumFrequency() == 0);
  assert(fs.checkInvariant());
}
