//
// Persistent Heap
//
// Complexity:
//
//  O(log n) merge/push/pop
//  O(1) copy
//

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <map>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define all(c) ((c).begin()), ((c).end())

template<class T>
struct persistent_heap {
  struct node {
    T x;
    node *l, *r;
  } *root;
  persistent_heap() : root(0) {}
  node *merge(node *a, node *b) {
    if (!a || !b) {
      return a ? a : b;
    }
    if (a->x > b->x) {
      swap(a, b);
    }
    if (rand() % 2) {
      return new node({a->x, merge(a->l, b), a->r});
    } else {
      return new node({a->x, a->l, merge(a->r, b)});
    }
  }
  void merge(persistent_heap b) {
    root = merge(root, b.root);
  }
  void push(T x) {
    root = merge(root, new node({x}));
  }
  void pop() {
    root = merge(root->l, root->r);
  }
  T top() const {
    return root->x;
  }
};
