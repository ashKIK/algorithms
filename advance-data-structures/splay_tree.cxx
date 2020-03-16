// 
// Splay Tree
//
// Complexity:
//   O(log n), amortized.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T>
struct splay_tree {
  struct node {
    T x;
    node *ch[2];
    int size;
  } *root;
  splay_tree() : root(0) {}
  int size(node *t) {
    return t ? t->size : 0;
  }
  node *update(node *t) {
    if (t) {
      t->size = size(t->ch[0]) + size(t->ch[1]) + 1;
    }
    return t;
  }
  node *rot(node *t, int d) {
    node *s = t->ch[d];
    t->ch[d] = s->ch[!d];
    update(t);
    s->ch[!d] = t;
    update(s);
    return s;
  }
  node *splay(node *t, int k) {
    if (!t) {
      return t;
    }
    if (size(t->ch[0]) == k) {
      return t;
    }
    int d = size(t->ch[0]) < k;
    if (d) {
      k = k - size(t->ch[0]) - 1;
    }
    if (size(t->ch[d]->ch[0]) != k) {
      int c = size(t->ch[d]->ch[0]) < k;
      if (c) {
        k = k - size(t->ch[d]->ch[0]) - 1;
      }
      t->ch[d]->ch[c] = splay(t->ch[d]->ch[c], k);
      c == d ? (t = rot(t, d)) : (t->ch[d] = rot(t->ch[d], !d));
    }
    return rot(t, d);
  }
  T &operator[](int k) {
    root = splay(root, k);
    return root->x;
  }
  void insert(int k, T x) {
    if (size(root) == k) {
      root = update(new node({x, {root, 0}}));
    } else {
      node *t = splay(root, k);
      root = new node({x, {t->ch[0], t}});
      root->ch[1]->ch[0] = 0;
      update(root->ch[1]);
      update(root);
    }
  }
  void erase(int k) {
    root = splay(root, k);
    root->ch[1] = splay(root->ch[1], 0);
    if (root->ch[1]) {
      root->ch[1]->ch[0] = root->ch[0];
      root = update(root->ch[1]);
    } else {
      root = root->ch[0];
    }
  }
};
