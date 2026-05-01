#include <cassert>
#include <vector>

#include "solution.h"

int main() {
  Solution sol;
  std::vector<int> nums = {10, 1, 2, 4, 7, 2};
  assert(sol.longestSubarrayNaive(nums, 5) == 4);
}
