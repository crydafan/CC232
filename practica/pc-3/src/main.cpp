#include <iostream>

#include "frequency_stack.h"

using std::cout;

// Minimal reference run (official LeetCode 895 example).
//
//   push(5) push(7) push(5) push(7) push(4) push(5)
//   pop() pop() pop() pop()  ->  5 7 5 4
int main() {
  FrequencyStack fs;

  const int input[] = {5, 7, 5, 7, 4, 5};
  cout << "push:";
  for (int v : input) {
    fs.push(v);
    cout << ' ' << v;
  }
  cout << '\n';
  cout << "size = " << fs.size() << ", maximum frequency = " << fs.maximumFrequency() << '\n';

  cout << "pop :";
  while (!fs.empty()) {
    cout << ' ' << fs.pop();
  }
  cout << '\n';
  cout << "expected: 5 7 5 4 ... (then the rest in frequency order)\n";

  return 0;
}
