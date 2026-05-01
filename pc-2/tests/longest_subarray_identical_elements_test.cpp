#include <cassert>
#include <vector>

#include "solution.h"

int main() {
  Solution sol;
  std::vector<int> nums = {5, 5, 5, 5};
  assert(sol.longestSubarray(nums, 0) == 4);
}
