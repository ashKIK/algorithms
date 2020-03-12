// Persistent Rope
//

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <functional>

using namespace std;

struct rope {
  struct node {
    char v;
    node *l, *r;
    int s;
    node(char v, node *l, node *r) : v(v), l(l), r(r) {
      s = 1 + (l ? l->s : 0) + (r ? r->s : 0);
    }
  } *root;
  static node *join(node *a, node *b) {
    auto R = [](int a, int b) {
      return rand() % (a + b) < a;
    };
    if (!a || !b) {
      return a ? a : b;
    }
    return R(a->s, b->s)
           ? new node(a->v, a->l, join(a->r, b))
           : new node(b->v, join(a, b->l), b->r);
  }
  static pair<node *, node *> split(node *a, int s) {
    if (!a || s <= 0) {
      return {0, a};
    }
    if (a->s <= s) {
      return {a, 0};
    }
    if (a->l && s <= a->l->s) {
      auto p = split(a->l, s);
      return {p.first, new node(a->v, p.second, a->r)};
    } else {
      auto p = split(a->r, s - (a->l ? a->l->s : 0) - 1);
      return {new node(a->v, a->l, p.first), p.second};
    }
  }
  static pair<node *, node *> cut(node *a, int l, int r) {
    if (l >= r) {
      return {0, a};
    }
    auto p = split(a, l);
    auto q = split(p.second, r - l);
    return {q.first, join(p.first, q.second)};
  }
  explicit rope(const char s[]) {
    function<node *(int, int)> build = [&](int l, int r) {
      if (l >= r) {
        return (node *) 0;
      }
      int m = l + (r - l) / 2;
      return new node(s[m], build(l, m), build(m + 1, r));
    };
    root = build(0, strlen(s));
  }

  rope(rope::node *r) : root(r) {}

  [[nodiscard]] int size() const {
    return root ? root->s : 0;
  }
  rope insert(int k, const char s[]) {
    auto p = split(root, k);
    return {join(p.first, join(rope(s).root, p.second))};
  }
  rope substr(int l, int r) {
    return {cut(root, l, r).first};
  }
  rope erase(int l, int r) {
    return {cut(root, l, r).second};
  }
  [[nodiscard]] char at(int k) const {
    function<char(node *)> rf = [&](node *a) {
      int s = a->l ? a->l->s : 0;
      if (k == s) {
        return a->v;
      }
      if (k < s) {
        return rf(a->l);
      }
      k -= s + 1;
      return rf(a->r);
    };
    return rf(root);
  }
  [[nodiscard]] string str() const {
    stringstream ss;
    function<void(node *)> rf = [&](node *a) {
      if (!a) {
        return;
      }
      rf(a->l);
      ss << a->v;
      rf(a->r);
    };
    rf(root);
    return ss.str();
  }
};
