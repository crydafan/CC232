#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

class FrequencyStackNaive {
public:
  void push(int val) { items.push_back(val); }

  int pop() {
    assert(!empty() && "FrequencyStackNaive::pop() called on an empty stack");

    int n = static_cast<int>(items.size());

    int bestIdx = -1;
    int bestFreq = -1;
    for (int i = n - 1; i >= 0; --i) {
      int f = 0;
      for (int j = 0; j < n; ++j) {
        if (items[j] == items[i])
          ++f;
      }
      if (f > bestFreq) {
        bestFreq = f;
        bestIdx = i;
      }
    }

    int val = items[bestIdx];
    items.erase(items.begin() + bestIdx);
    return val;
  }

  bool empty() const { return items.empty(); }
  std::size_t size() const { return items.size(); }

private:
  std::vector<int> items;
};
