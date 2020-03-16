//
// Randomized Binary Search Tree
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>
#include <functional>

using namespace std;

template<class T>
struct randomized_bst {
  struct node {
    T x;
    node *l;
    node *r;
    int s;
  } *root;
  randomized_bst() : root(0) {}

  node *update(node *t) {
    if (!t) {
      return t;
    }
    t->s = 1;
    if (t->l) {
      t->s += t->l->s;
    }
    if (t->r) {
      t->s += t->r->s;
    }
    return t;
  }
  node *make_node(const T &x, node *l = 0, node *r = 0) {
    return update(new node({x, l, r}));
  }
  node *merge(node *a, node *b) {
    if (!a || !b) {
      return a ? a : b;
    }
    if (rand() % (a->s + b->s) < a->s) {
      a->r = merge(a->r, b);
      return update(a);
    } else {
      b->l = merge(a, b->l);
      return update(b);
    }
  }
  pair<node *, node *> split(node *a, const T &x) {
    if (!a) {
      return {0, 0};
    }
    if (a->x < x) {
      auto p = split(a->r, x);
      a->r = p.first;
      return {update(a), p.second};
    } else {
      auto p = split(a->l, x);
      a->l = p.second;
      return {p.first, update(a)};
    }
  }
  void insert(const T &x) {
    auto p = split(root, x);
    root = merge(merge(p.first, make_node(x)), p.second);
  }
  node *remove(node *t, const T &x) {
    if (!t) {
      return t;
    }
    if (t->x == x) {
      return merge(t->l, t->r);
    }
    t->x < x ? (t->r = remove(t->r, x)) : (t->l = remove(t->l, x));
    return update(t);
  }
  void remove(const T &x) {
    root = remove(root, x);
  }
  node *find(const T &x) {
    node *t = root;
    while (t && t->x != x) {
      t = (t->x < x ? t->r : t->l);
    }
    return t;
  }
};
