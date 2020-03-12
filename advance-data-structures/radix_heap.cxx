//
// Radix Heap
//
// Time Complexity:
//
//  O(|bits|) for all operations.
//  O(1) pop.
//

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <functional>

using namespace std;

#define all(c) ((c).begin()), ((c).end())

template<class T>
struct RadixHeap {
  static int bsr(uint32_t i) {
    return i ? 31 - __builtin_clz(i) : -1;
  }
  uint32_t size;
  uint32_t last;
  vector<pair<uint32_t, T>> v[33];
  RadixHeap() : size(0), last(0) {}

  [[nodiscard]] bool empty() const {
    return size == 0;
  }
  void aux(pair<uint32_t, T> p) {
    v[bsr(p.first ^ last) + 1].push_back(p);
  }
  pair<uint32_t, T> top() {
    if (v[0].empty()) {
      int i = 1;
      while (v[i].empty()) {
        i++;
      }
      last = min_element(all(v[i]))->first;
      for (auto p: v[i]) {
        aux(p);
      }
      v[i].clear();
    }
    return v[0].back();
  }
  void push(uint32_t key, T value) {
    size++;
    aux({key, value});
  }
  void pop() {
    size--;
    top();
    v[0].pop_back();
  }
};
