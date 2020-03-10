//
// Minimum Arborescence (Chu-Liu / Edmonds -  recursive shrinking)
//
// Time Complexity: O(m*n)
//

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct graph {
  int n;
  graph(int n) : n(n) {}
  struct edge {
    int src;
    int dst;
    int weight;
  };

  vector<edge> edges;
  const double INF = 9999;

  void add_edge(int u, int v, int w) {
    edges.push_back({u, v, w});
  }

  int arborescence(int r) {
    int N = n;
    for (int res = 0;;) {
      vector<edge> in(N, {-1, -1, (int) INF});
      vector<int> C(N, -1);
      for (auto e : edges) {
        if (in[e.dst].weight > e.weight) {
          in[e.dst] = e;
        }
      }
      in[r] = {r, r, 0};
      for (int u = 0; u < N; u++) {
        if (in[u].src < 0) {
          return -1;
        }
        res += in[u].weight;
      }
      vector<int> mark(N, -1);
      int index = 0;
      for (int i = 0; i < N; i++) {
        if (mark[i] != -1) {
          continue;
        }
        int u = i;
        while (mark[u] == -1) {
          mark[u] = i;
          u = in[u].src;
        }
        if (mark[u] != i || u == r) {
          continue;
        }
        for (int v = in[u].src; u != v; v = in[v].src) {
          C[v] = index;
        }
        C[u] = index++;
      }
      if (index == 0) {
        return res;
      }
      for (int i = 0; i < N; ++i) {
        if (C[i] == -1) {
          C[i] = index++;
        }
      }
      vector<edge> next;
      for (auto &e : edges) {
        if (C[e.src] != C[e.dst] && C[e.dst] != C[r]) {
          next.push_back({C[e.src], C[e.dst], e.weight - in[e.dst].weight});
        }
      }
      edges.swap(next);
      N = index;
      r = C[r];
    }
  }
};

int main() {
  for (int q = 0; q <= 10; q++) {
    srand(q);
    int n = 1000;
    graph g(n);

    for (int i = 0; i < n; i++) {
      for (int k = 0; k < 10; k++) {
        int j;
        do {
          j = rand() % n;
        } while (i == j);
        int w = 1 + rand() % 100;
        g.add_edge(i, j, w);
      }
    }

    double b = g.arborescence(0);
    cout << abs(b) << endl;
  }
}
