#include <cassert>

#include "frequency_stack.h"

int main() {
  FrequencyStack fs;

  fs.push(1);
  fs.push(1);
  fs.push(2);
  fs.push(2);

  assert(fs.pop() == 2);
  assert(fs.pop() == 1);
  assert(fs.pop() == 2);
  assert(fs.pop() == 1);
  assert(fs.empty());
}
