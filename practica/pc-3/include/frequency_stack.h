#pragma once

#include <cassert>
#include <cstddef>
#include <stack>
#include <unordered_map>
#include <vector>

class FrequencyStack {
public:
  FrequencyStack() : maximum_frequency(0), count(0) { group.emplace_back(); }

  void push(int val) {
    int count = ++frequency[val];

    while (count >= static_cast<int>(group.size())) {
      group.emplace_back();
    }

    group[count].push(val);

    if (count > maximum_frequency) {
      maximum_frequency = count;
    }
    ++this->count;
  }

  int pop() {
    assert(!empty() && "FrequencyStack::pop() called on an empty stack");

    int val = group[maximum_frequency].top();

    group[maximum_frequency].pop();
    --frequency[val];

    if (group[maximum_frequency].empty()) {
      --maximum_frequency;
    }
    --count;

    return val;
  }

  bool empty() const { return maximum_frequency == 0; }
  std::size_t size() const { return count; }
  int maximumFrequency() const { return maximum_frequency; }

  bool checkInvariant() const {
    if (maximum_frequency < 0)
      return false;
    if (!group[0].empty())
      return false;

    for (int f = 1; f <= maximum_frequency; ++f) {
      if (group[f].empty())
        return false;
    }
    for (std::size_t f = static_cast<std::size_t>(maximum_frequency) + 1;
         f < group.size(); ++f) {
      if (!group[f].empty())
        return false;
    }

    std::unordered_map<int, int> counted;
    std::size_t total = 0;
    int observedMax = 0;
    for (std::size_t f = 1; f < group.size(); ++f) {
      std::stack<int> tmp = group[f];
      while (!tmp.empty()) {
        counted[tmp.top()]++;
        tmp.pop();
        ++total;
        if (static_cast<int>(f) > observedMax)
          observedMax = static_cast<int>(f);
      }
    }

    if (total != count)
      return false;

    if (observedMax != maximum_frequency)
      return false;

    for (const auto &kv : frequency) {
      if (kv.second == 0)
        continue;
      auto it = counted.find(kv.first);
      if (it == counted.end() || it->second != kv.second)
        return false;
    }
    for (const auto &kv : counted) {
      auto it = frequency.find(kv.first);
      if (it == frequency.end() || it->second != kv.second)
        return false;
    }
    return true;
  }

private:
  std::unordered_map<int, int> frequency;
  std::vector<std::stack<int>> group;
  int maximum_frequency;
  std::size_t count;
};
