struct Graph {

    int n;
    std::vector<int> in, out;
    std::vector<std::vector<int>> adj;

    struct Edge { int u, v; };
    std::vector<Edge> e;

    Graph(int n) : n(n), in(n), out(n), adj(n) {}
    void add_edge(int u, int v) {
        out[u] += 1, in[v] += 1;
        adj[u].push_back(e.size()); e.push_back({u, v});
    }

    void lexico() {
        for (auto &v : adj)
            std::sort(v.begin(), v.end(),
                [&](int i, int j) { return e[i].v < e[j].v; }); }

    auto eulerian() -> std::vector<int> {
        std::vector<int> trail;
        for (int u = 0, odd = 0; u < n; u++)
            if (in[u] != out[u] && (++odd > 2
                || std::abs(in[u] - out[u]) > 1)) return trail;

        int s = -1;
        for (int u = 0; u < n; u++) if (out[u] > in[u]) { s = u; break; }
        if (s == -1) for (int u = 0; u < n; u++) if (out[u]) { s = u; break; }
        if (s == -1) return {};

        std::vector<int> cur(n);
        auto dfs = [&](auto &self, int u) -> void {
            while (cur[u] < adj[u].size()) {
                int eid = adj[u][cur[u]++];
                auto [_, v] = e[eid];
                self(self, v);
                trail.push_back(eid);
            }
        };

        dfs(dfs, s);

        if (trail.size() < e.size()) return {};
        std::reverse(trail.begin(), trail.end());
        return trail;
    }

};

void test() {
    int n, m;
    std::cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v; u--, v--;
        g.add_edge(u, v);
    }

    g.lexico();
    auto trail = g.eulerian();
    if (trail.empty()) { std::cout << "No" << '\n'; return; }
    for (int eid : trail) { auto [u, v] = g.e[eid]; std::cout << u + 1 << ' '; }
    std::cout << g.e[trail.back()].v + 1 << '\n';
}