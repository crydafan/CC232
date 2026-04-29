#include <algorithm>

#include "solution.h"

using std::max;
using std::min;

int Solution::longestSubarray(vector<int> &nums, int limit) { return 0; }

int Solution::longestSubarrayNaive(vector<int> &nums, int limit) {
  int ans = 0;

  for (size_t i = 0; i < nums.size(); i++) {
    int mn = nums[i], mx = nums[i];

    for (size_t j = i; j < nums.size(); j++) {
      mn = min(mn, nums[j]);
      mx = max(mx, nums[j]);

      if (mx - mn > limit)
        break;

      ans = max(static_cast<size_t>(ans), j - i + 1);
    }
  }
  return ans;
}
