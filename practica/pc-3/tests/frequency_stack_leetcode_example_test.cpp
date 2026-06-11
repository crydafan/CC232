#include <cassert>

#include "frequency_stack.h"

int main() {
  FrequencyStack fs;

  for (int v : {5, 7, 5, 7, 4, 5}) {
    fs.push(v);
  }

  assert(fs.pop() == 5);
  assert(fs.pop() == 7);
  assert(fs.pop() == 5);
  assert(fs.pop() == 4);
}
