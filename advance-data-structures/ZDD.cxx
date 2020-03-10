//
// Zero-suppressed binary decision diagram with family algebra operations
//
// ZDD maintains a set of sets of integers.
//   - cup(A, B): union of A and B
//   - cap(A, B): intersection of A and B
//   - sub(A, B): difference of A and B
//   - mul(A, B): Kronecker product of A and B
//   - div(A, B): quotient of A and B
//   - mod(A, B): reminder of A and B
//   - count(A) : the number of sets in A
//

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <functional>

using namespace std;
typedef long long LL;

struct ZDD {
  struct Node {
    int v, lo, hi;
    bool operator<(Node y) const {
      if (v != y.v) {
        return v < y.v;
      } else {
        if (lo != y.lo) {
          return lo < y.lo;
        } else {
          return hi < y.hi;
        }
      }
    }
  };
  vector <Node> node;
  static const int zero = 0;
  static const int unit = 1;
  ZDD() : node(2, (Node) {-1, 0, 0}) {}

  int getnode(int v, int lo, int hi) {
    static map<Node, int> H;
    if (!hi) {
      return lo;
    }
    Node t = {v, lo, hi};
    if (H.count(t)) {
      return H[t];
    }
    node.push_back(t);
    return H[t] = node.size() - 1;
  }

  int var(int v) {
    return getnode(v, 0, 1);
  }

  int cup(int x, int y) {
    static map<pair<int, int>, int> H;
    if (x > y) {
      swap(x, y);
    }
    if (!x || x == y) {
      return y;
    }
    if (H.count(make_pair(x, y))) {
      return H[make_pair(x, y)];
    }
    if (node[x].v > node[y].v) {
      return H[make_pair(x, y)] = getnode(node[x].v, cup(node[x].lo, y), node[x].hi);
    } else {
      if (node[x].v < node[y].v) {
        return H[make_pair(x, y)] = getnode(node[y].v, cup(node[y].lo, x), node[y].hi);
      } else {
        return H[make_pair(x, y)] = getnode(node[x].v, cup(node[x].lo, node[y].lo), cup(node[x].hi, node[y].hi));
      }
    }
  }

  int cap(int x, int y) {
    static map<pair<int, int>, int> H;
    if (x > y) {
      swap(x, y);
    }
    if (!x || x == y) {
      return x;
    }
    if (H.count(make_pair(x, y))) {
      return H[make_pair(x, y)];
    }
    if (node[x].v > node[y].v) {
      return H[make_pair(x, y)] = cap(node[x].lo, y);
    } else {
      if (node[x].v < node[y].v) {
        return H[make_pair(x, y)] = cap(node[y].lo, x);
      } else {
        return H[make_pair(x, y)] = getnode(node[x].v, cap(node[x].lo, node[y].lo), cap(node[x].hi, node[y].hi));
      }
    }
  }

  int sub(int x, int y) {
    static map<pair<int, int>, int> H;
    if (!x || !y) {
      return x;
    }
    if (x == y) {
      return 0;
    }
    if (H.count(make_pair(x, y))) {
      return H[make_pair(x, y)];
    }
    if (node[x].v > node[y].v) {
      return H[make_pair(x, y)] = getnode(node[x].v, sub(node[x].lo, y), node[x].hi);
    } else {
      if (node[x].v < node[y].v) {
        return H[make_pair(x, y)] = sub(x, node[y].lo);
      } else {
        return H[make_pair(x, y)] = getnode(node[x].v, sub(node[x].lo, node[y].lo), sub(node[x].hi, node[y].hi));
      }
    }
  }

  int mul(int x, int y) {
    static map<pair<int, int>, int> H;
    if (x > y) {
      swap(x, y);
    }
    if (!x || y == 1) {
      return x;
    }
    if (!y || x == 1) {
      return y;
    }
    if (H.count(make_pair(x, y))) {
      return H[make_pair(x, y)];
    }
    if (node[x].v > node[y].v) {
      return H[make_pair(x, y)] = getnode(node[x].v, mul(node[x].lo, y), mul(node[x].hi, y));
    } else {
      if (node[x].v < node[y].v) {
        return H[make_pair(x, y)] = getnode(node[y].v, mul(node[y].lo, x), mul(node[y].hi, x));
      } else {
        return H[make_pair(x, y)] = getnode(node[x].v, mul(node[x].lo, node[y].lo), cup(cup(
          mul(node[x].hi, node[y].hi), mul(node[x].hi, node[y].lo)), mul(node[x].lo, node[y].hi)));
      }
    }
  }

  int div(int x, int y) {
    static map<pair<int, int>, int> H;
    if (y == 1) {
      return x;
    }
    if (x <= 1) {
      return 0;
    }
    if (x == y) {
      return 1;
    }
    if (node[x].v < node[y].v) {
      return 0;
    }
    if (H.count(make_pair(x, y))) {
      return H[make_pair(x, y)];
    }
    if (node[x].v != node[y].v) {
      return H[make_pair(x, y)] = getnode(node[x].v, div(node[x].lo, y), div(node[x].hi, y));
    }
    int z = div(node[x].hi, node[y].hi);
    return H[make_pair(x, y)] = z && node[y].lo ? cap(z, div(node[x].lo, node[y].lo)) : z;
  }

  int mod(int x, int y) { return sub(x, mul(y, div(x, y))); }

  LL count(int x) {
    static map<int, LL> H;
    if (x <= 1) {
      return x;
    }
    if (H.count(x)) {
      return H[x];
    }
    return H[x] = count(node[x].lo) + count(node[x].hi);
  }

  void display(int x, vector<int> p = vector<int>()) {
    if (x == 1) {
      for (int i : p) {
        cout << i << ".";
      }
      cout << "_ ";
    } else if (x != 0) {
      p.push_back(node[x].v);
      display(node[x].hi, p);
      p.pop_back();
      display(node[x].lo, p);
    }
  }
};
