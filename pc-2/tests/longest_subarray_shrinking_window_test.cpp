#include <cassert>
#include <vector>

#include "solution.h"

int main() {
  Solution sol;
  std::vector<int> nums = {8, 2, 4, 7};
  assert(sol.longestSubarray(nums, 4) == 2);
}
