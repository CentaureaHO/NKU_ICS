#include <benchmark.h>

static int N;
#define MAXN 128
#define MAXM (MAXN * MAXN + MAXN * 2) * 2

const int INF = 0x3f3f3f;

<<<<<<< HEAD
struct Edge
{
    int from, to, cap, flow;
    Edge() {}
    Edge(int from, int to, int cap, int flow)
    {
        this->from = from;
        this->to   = to;
        this->cap  = cap;
        this->flow = flow;
    }
};

template <typename T>
static inline T min(T x, T y)
{
    return x < y ? x : y;
}

struct Dinic
{
    int  n, m, s, t;
    Edge edges[MAXM];
    int  head[MAXN * 2 + 2];
    int  nxt[MAXM];
    bool vis[MAXN * 2 + 2];
    int  d[MAXN * 2 + 2], cur[MAXN * 2 + 2], queue[MAXN * 2 + 2];

    void init(int n)
    {
        this->n = n;
        for (int i = 0; i < n; i++) {
            head[i] = -1;
=======
struct Edge {
  int from, to, cap, flow;
  Edge() {}
  Edge(int from, int to, int cap, int flow) {
    this->from = from;
    this->to = to;
    this->cap = cap;
    this->flow = flow;
  }
};

template <typename T> static inline T min(T x, T y) { return x < y ? x : y; }

struct Dinic {
  int n, m, s, t;
  Edge edges[MAXM];
  int head[MAXN * 2 + 2];
  int nxt[MAXM];
  bool vis[MAXN * 2 + 2];
  int d[MAXN * 2 + 2], cur[MAXN * 2 + 2], queue[MAXN * 2 + 2];

  void init(int n) {
    this->n = n;
    for (int i = 0; i < n; i++) {
      head[i] = -1;
    }
    m = 0;
  }

  void AddEdge(int u, int v, int c) {
    if (c == 0)
      return;
    edges[m] = Edge(u, v, c, 0);
    nxt[m] = head[u];
    head[u] = m++;
    edges[m] = Edge(v, u, 0, 0);
    nxt[m] = head[v];
    head[v] = m++;
  }

  bool BFS() {
    for (int i = 0; i < n; i++)
      vis[i] = 0;
    int qf = 0, qr = 0;
    queue[qr++] = s;
    d[s] = 0;
    vis[s] = 1;
    while (qf != qr) {
      int x = queue[qf++];
      for (int i = head[x]; i != -1; i = nxt[i]) {
        Edge &e = edges[i];
        if (!vis[e.to] && e.cap > e.flow) {
          vis[e.to] = 1;
          d[e.to] = d[x] + 1;
          queue[qr++] = e.to;
>>>>>>> master
        }
        m = 0;
    }

<<<<<<< HEAD
    void AddEdge(int u, int v, int c)
    {
        if (c == 0) return;
        edges[m] = Edge(u, v, c, 0);
        nxt[m]   = head[u];
        head[u]  = m++;
        edges[m] = Edge(v, u, 0, 0);
        nxt[m]   = head[v];
        head[v]  = m++;
=======
  int DFS(int x, int a) {
    if (x == t || a == 0)
      return a;
    int flow = 0, f;
    for (int i = cur[x]; i != -1; i = nxt[i]) {
      Edge &e = edges[i];
      if (d[x] + 1 == d[e.to] && (f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
        e.flow += f;
        edges[i ^ 1].flow -= f;
        flow += f;
        a -= f;
        if (a == 0)
          break;
      }
>>>>>>> master
    }

<<<<<<< HEAD
    bool BFS()
    {
        for (int i = 0; i < n; i++) vis[i] = 0;
        int      qf = 0, qr = 0;
        queue[qr++] = s;
        d[s]        = 0;
        vis[s]      = 1;
        while (qf != qr) {
            int x = queue[qf++];
            for (int i = head[x]; i != -1; i = nxt[i]) {
                Edge& e = edges[i];
                if (!vis[e.to] && e.cap > e.flow) {
                    vis[e.to]   = 1;
                    d[e.to]     = d[x] + 1;
                    queue[qr++] = e.to;
                }
            }
        }
        return vis[t];
    }

    int DFS(int x, int a)
    {
        if (x == t || a == 0) return a;
        int flow = 0, f;
        for (int i = cur[x]; i != -1; i = nxt[i]) {
            Edge& e = edges[i];
            if (d[x] + 1 == d[e.to] && (f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
                e.flow += f;
                edges[i ^ 1].flow -= f;
                flow += f;
                a -= f;
                if (a == 0) break;
            }
        }
        return flow;
    }

    int Maxflow(int s, int t)
    {
        this->s  = s;
        this->t  = t;
        int flow = 0;
        while (BFS()) {
            for (int i = 0; i < n; i++) cur[i] = head[i];
            flow += DFS(s, INF);
        }
        return flow;
=======
  int Maxflow(int s, int t) {
    this->s = s;
    this->t = t;
    int flow = 0;
    while (BFS()) {
      for (int i = 0; i < n; i++)
        cur[i] = head[i];
      flow += DFS(s, INF);
>>>>>>> master
    }
};

extern "C" {

<<<<<<< HEAD
static Dinic* G;
static int    ans;

void bench_dinic_prepare()
{
    N = setting->size;
    bench_srand(1);
    int s = 2 * N, t = 2 * N + 1;
    G = (Dinic*)bench_alloc(sizeof(Dinic));
    G->init(2 * N + 2);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            G->AddEdge(i, N + j, bench_rand() % 10);
        }

    for (int i = 0; i < N; i++) {
        G->AddEdge(s, i, bench_rand() % 1000);
        G->AddEdge(N + i, t, bench_rand() % 1000);
    }
=======
static Dinic *G;
static int ans;

void bench_dinic_prepare() {
  N = setting->size;
  bench_srand(1);
  int s = 2 * N, t = 2 * N + 1;
  G = (Dinic *)bench_alloc(sizeof(Dinic));
  G->init(2 * N + 2);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      G->AddEdge(i, N + j, bench_rand() % 10);
    }

  for (int i = 0; i < N; i++) {
    G->AddEdge(s, i, bench_rand() % 1000);
    G->AddEdge(N + i, t, bench_rand() % 1000);
  }
>>>>>>> master
}

void bench_dinic_run() { ans = G->Maxflow(2 * N, 2 * N + 1); }

int bench_dinic_validate() { return (uint32_t)ans == setting->checksum; }
}
