#include <iostream>
#include <vector>

#include "solution.h"

using std::cout;
using std::endl;
using std::vector;

int main() {
  Solution sol;

  {
    vector<int> nums = {8, 2, 4, 7};
    int limit = 4;

    cout << "nums={8,2,4,7}, limit=4" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }

  {
    vector<int> nums = {10, 1, 2, 4, 7, 2};
    int limit = 5;

    cout << "nums={10,1,2,4,7,2}, limit=5" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }

  {
    vector<int> nums = {4, 2, 2, 2, 4, 4, 2};
    int limit = 0;

    cout << "nums={4,2,2,2,4,4,2}, limit=0" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }

  {
    vector<int> nums = {7};
    int limit = 0;

    cout << "nums={7}, limit=0" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }

  {
    vector<int> nums = {3, 3, 3, 3};
    int limit = 0;

    cout << "nums={3,3,3,3}, limit=0" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }

  {
    vector<int> nums = {1, 100, 1};
    int limit = 0;

    cout << "nums={1,100,1}, limit=0" << endl;
    cout << "  optima:   " << sol.longestSubarray(nums, limit) << endl;
    cout << "  ingenua:  " << sol.longestSubarrayNaive(nums, limit) << endl;
  }
}
