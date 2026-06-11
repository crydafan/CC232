#include <iostream>

#include "frequency_stack.h"

using std::cout;

int main() {
  cout << "\n=== LeetCode 895 example ===\n";
  {
    FrequencyStack fs;
    for (int v : {5, 7, 5, 7, 4, 5}) {
      fs.push(v);
      cout << "push " << v << "  -> maxFreq=" << fs.maximumFrequency()
           << " size=" << fs.size() << '\n';
    }
    cout << "pops:";
    while (!fs.empty()) {
      cout << ' ' << fs.pop();
    }
    cout << "   (expected: 5 7 5 4 1...)\n";
  }

  cout << "\n=== Frequency tie: the most recent wins ===\n";
  {
    FrequencyStack fs;
    fs.push(1);
    fs.push(1);
    fs.push(2);
    fs.push(2);
    cout << "pop = " << fs.pop()
         << "  (expected 2: ties at freq 2 but is more recent)\n";
    cout << "pop = " << fs.pop() << "  (expected 1)\n";
  }

  cout << "\n=== Single repeated value ===\n";
  {
    FrequencyStack fs;
    for (int i = 0; i < 4; ++i) {
      fs.push(9);
    }
    cout << "pops:";
    while (!fs.empty()) {
      cout << ' ' << fs.pop();
    }
    cout << "   (expected: 9 9 9 9)\n";
  }
}
