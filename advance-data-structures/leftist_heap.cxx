//
// Leftist Heap
//
// Complexity:
//
//  O(1) top
//  O(log n) push/pop/merge
//

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <functional>

using namespace std;

#define all(c) ((c).begin()), ((c).end())

template<class T>
struct LeftistHeap {
  struct Node {
    T key;
    Node *left = 0;
    Node *right = 0;
    int dist = 0;
  } *root = 0;
  static Node *merge(Node *x, Node *y) {
    if (!x) {
      return y;
    }
    if (!y) {
      return x;
    }
    if (x->key > y->key) {
      swap(x, y);
    }
    x->right = merge(x->right, y);
    if (!x->left || x->left->dist < x->dist) {
      swap(x->left, x->right);
    }
    x->dist = (x->right ? x->right->dist : 0) + 1;
    return x;
  }
  void push(T key) {
    root = merge(root, new Node({key}));
  }
  void pop() {
    root = merge(root->left, root->right);
  }
  T top() {
    return root->key;
  }
};
