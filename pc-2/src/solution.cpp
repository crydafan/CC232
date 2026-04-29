#include <algorithm>
#include <deque>

#include "solution.h"

using std::deque;
using std::max;
using std::min;

int Solution::longestSubarray(vector<int> &nums, int limit) {
  deque<int> maxDq, minDq;
  int l = 0, ans = 0;

  for (size_t r = 0; r < nums.size(); r++) {
    while (!maxDq.empty() && nums[maxDq.back()] <= nums[r])
      maxDq.pop_back();
    maxDq.push_back(r);

    while (!minDq.empty() && nums[minDq.back()] >= nums[r])
      minDq.pop_back();
    minDq.push_back(r);

    while (nums[maxDq.front()] - nums[minDq.front()] > limit) {
      l++;
      if (maxDq.front() < l)
        maxDq.pop_front();
      if (minDq.front() < l)
        minDq.pop_front();
    }

    ans = max(static_cast<size_t>(ans), r - l + 1);
  }

  return ans;
}

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
