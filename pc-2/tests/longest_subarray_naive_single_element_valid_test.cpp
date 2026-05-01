#include <cassert>
#include <vector>

#include "solution.h"

int main() {
  Solution sol;
  std::vector<int> nums = {1, 100, 1};
  assert(sol.longestSubarrayNaive(nums, 0) == 1);
}
