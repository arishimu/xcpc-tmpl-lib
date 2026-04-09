template <class T>
struct MaxFlow {

    int n;
    std::vector<int> dep, cur;
    std::vector<std::vector<int>> adj;

    struct Edge { int to; T cap; };
    std::vector<Edge> e;

    MaxFlow(int n) : n(n), adj(n) {}
    void add_edge(int u, int v, T cap) {
        adj[u].push_back(e.size());
        e.push_back({v, cap});
        adj[v].push_back(e.size());
        e.push_back({u, 0});
    }

    T dfs(int u, int t, T flow) {
        if (u == t) return flow;
        T resi = flow;
        for (int& timer = cur[u]; timer < adj[u].size(); timer++) {
            int eid = adj[u][timer];
            auto [v, c] = e[eid];
            if (dep[v] != dep[u] + 1) continue;
            if (c <= 0) continue;
            T subf = dfs(v, t, std::min(resi, c));
            e[eid].cap -= subf, e[eid^1].cap += subf;
            resi -= subf;
            if (resi <= 0) break;
        }
        return flow - resi;
    }

    bool bfs(int s, int t) {
        std::queue<int> q;
        dep.assign(n, -1);
        dep[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int eid : adj[u]) {
                auto [v, c] = e[eid];
                if (dep[v] != -1) continue;
                if (c <= 0) continue;
                dep[v] = dep[u] + 1;
                q.push(v);
            }
        }
        return dep[t] != -1;
    }

    T flow(int s, int t) {
        T acc = 0;
        while (bfs(s, t)) {
            cur.assign(n, 0);
            acc += dfs(s, t, std::numeric_limits<T>::max());
        }
        return acc;
    }

    T get_flow(int eid) { return e[eid << 1 | 1].cap; }
    bool get_side(int u) { return dep[u] != -1; }

};

void test() {
    int n, m, s, t;
    std::cin >> n >> m >> s >> t; s--, t--;
    MaxFlow<i64> g(n);
    for (int i = 0; i < m; i++) {
        int u, v; i64 w;
        std::cin >> u >> v >> w; u--, v--;
        g.add_edge(u, v, w);
    }
    std::cout << g.flow(s, t) << '\n';
}