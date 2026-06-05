#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "solution.h"

using std::cout;
using std::endl;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

vector<int> make_input(int n, int lo, int hi, unsigned seed = 42) {
  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> dist(lo, hi);

  vector<int> v(n);

  for (auto &x : v)
    x = dist(rng);

  return v;
}

int main() {
  Solution sol;

  {
    vector<int> nums = make_input(100000, 0, 1000);
    int limit = 100;

    auto t0 = steady_clock::now();
    int r1 = sol.longestSubarray(nums, limit);
    auto t1 = steady_clock::now();
    int r2 = sol.longestSubarrayNaive(nums, limit);
    auto t2 = steady_clock::now();

    long long us_opt = duration_cast<microseconds>(t1 - t0).count();
    long long us_naive = duration_cast<microseconds>(t2 - t1).count();

    cout << "n=100000, rango=[0 1000], limit=100" << endl;
    cout << "  optima   resultado=" << r1 << "  tiempo=" << us_opt << " us"
         << endl;
    cout << "  ingenua  resultado=" << r2 << "  tiempo=" << us_naive << " us"
         << endl;
  }

  {
    vector<int> nums = make_input(5000, 0, 50);
    int limit = 200;

    auto t0 = steady_clock::now();
    int r1 = sol.longestSubarray(nums, limit);
    auto t1 = steady_clock::now();
    int r2 = sol.longestSubarrayNaive(nums, limit);
    auto t2 = steady_clock::now();

    long long us_opt = duration_cast<microseconds>(t1 - t0).count();
    long long us_naive = duration_cast<microseconds>(t2 - t1).count();

    cout << "n=5000, rango=[0 50], limit=200 (peor caso para naive)" << endl;
    cout << "  optima   resultado=" << r1 << "  tiempo=" << us_opt << " us"
         << endl;
    cout << "  ingenua  resultado=" << r2 << "  tiempo=" << us_naive << " us"
         << endl;
  }
}
