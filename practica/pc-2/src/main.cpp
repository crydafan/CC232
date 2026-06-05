#include <iostream>
#include <vector>

#include "solution.h"

using std::cout;
using std::endl;
using std::vector;

int main() {
  Solution sol;

  vector<int> nums = {10, 1, 2, 4, 7, 2};
  int limit = 10;

  cout << "optima:   " << sol.longestSubarray(nums, limit) << endl;
  cout << "ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
}
