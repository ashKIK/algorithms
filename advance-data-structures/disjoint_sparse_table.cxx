//
// Disjoint Sparse Table
//
// Time Complexity:
//   pre-processing O(n log n)
//   query O(1)
//

#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

#define all(c) ((c).begin()), ((c).end())

template<class T, class Operator>
struct DisjointSparseTable {
  vector<vector<T>> ys;
  Operator o_times; // binary associative operator
  DisjointSparseTable(vector<T> xs, Operator o_times_) : o_times(o_times_) {
    int n = 1;
    while (n <= xs.size()) {
      n *= 2;
    }
    xs.resize(n);
    ys.push_back(xs);
    for (int h = 1;; h++) {
      int range = 2 << h;
      int half = range >>= 1;
      if (range > n) {
        break;
      }
      ys.push_back(xs);
      for (int i = half; i < n; i += range) {
        for (int j = i - 2; j >= i - half; j--) {
          ys[h][j] = o_times(ys[h][j], ys[h][j + 1]);
        }
        for (int j = i + 1; j < min(n, i + half); j++) {
          ys[h][j] = o_times(ys[h][j - 1], ys[h][j]);
        }
      }
    }
  }
  T prod(int i, int j) {
    --j;
    int h = sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(i ^ j);
    return o_times(ys[h][i], ys[h][j]);
  }
};

template<class T, class Op>
auto makeDisjointSparseTable(vector<T> xs, Op op) {
  return DisjointSparseTable<T, Op>(xs, op);
}
