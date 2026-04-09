template <class T>
struct MinCostFlow {

    int n;
    std::vector<T> dist, h;
    std::vector<int> trace;
    std::vector<std::vector<int>> adj;
    static constexpr T INF = std::numeric_limits<T>::max() / 2;

    struct Edge { int to; T cap, cost; };
    std::vector<Edge> e;

    MinCostFlow(int n) : n(n), adj(n) {}
    void add_edge(int u, int v, T cap, T cost) {
        adj[u].push_back(e.size());
        e.push_back({v, cap, cost});
        adj[v].push_back(e.size());
        e.push_back({u, 0, -cost});
    }

    bool johnson(int s, int t) {
        using state = std::pair<T, int>;
        std::priority_queue<state, std::vector<state>, std::greater<>> q;
        dist.assign(n, INF), trace.assign(n, -1);
        dist[s] = 0, q.emplace(0, s);
        while (!q.empty()) {
            auto [d, u] = q.top(); q.pop();
            if (d > dist[u]) continue;
            for (int i : adj[u]) {
                auto [v, cap, cost] = e[i];
                if (cap > 0 && d + cost + h[u] - h[v] < dist[v]) {
                    dist[v] = d + cost + h[u] - h[v];
                    trace[v] = i;
                    q.emplace(dist[v], v);
                }
            }
        }
        return dist[t] != INF;
    }

    struct result { T flow, cost; };
    auto flow(int s, int t) {
        T flow = 0, cost = 0;
        h.assign(n, 0);
        while (johnson(s, t)) {
            T allow = INF;
            for (int i = 0; i < n; i++) h[i] += dist[i];
            for (int v = t; v != s; v = e[trace[v]^1].to)
                allow = std::min(allow, e[trace[v]].cap);
            for (int v = t; v != s; v = e[trace[v]^1].to)
                e[trace[v]].cap -= allow,
                e[trace[v]^1].cap += allow;
            flow += allow;
            cost += allow * h[t];
        }
        return result{flow, cost};
    }

    T get_flow(int eid) { return e[eid << 1 | 1].cap; }

};

void test() {
    int n, m, s, t;
    std::cin >> n >> m >> s >> t; s--, t--;
    MinCostFlow<i64> g(n);
    for (int i = 0; i < m; i++) {
        int u, v; i64 w, c;
        std::cin >> u >> v >> w >> c; u--, v--;
        g.add_edge(u, v, w, c);
    }
    auto [flow, cost] = g.flow(s, t);
    std::cout << flow << ' ' << cost << '\n';
}