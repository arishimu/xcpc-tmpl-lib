struct EDCC {

    int n;
    std::vector<int> cid;
    std::vector<std::vector<std::array<int, 2>>> adj;
    EDCC(int n) : n(n), adj(n) {}

    void add_edge(int u, int v) {
        static int eid = 0;
        adj[u].push_back({eid, v});
        adj[v].push_back({eid, u});
        eid += 1;
    }

    auto tarjan() {
        std::vector<std::vector<int>> edcc;
        cid.assign(n, -1);

        int timer = 0;
        std::stack<int> stk;
        std::vector<int> dfn(n, -1), low(n, -1);

        auto dfs = [&](auto &f, int u, int in) -> void {
            low[u] = dfn[u] = timer++;
            stk.push(u);
            for (auto [eid, v] : adj[u]) {
                if (eid == in) continue;
                if (dfn[v] == -1) {
                    f(f, v, eid);
                    low[u] = std::min(low[u], low[v]);
                } else if (cid[v] == -1 && dfn[v] < dfn[u]) {
                    low[u] = std::min(low[u], dfn[v]);
                }
            }
            if (low[u] == dfn[u]) {
                int id = edcc.size();
                edcc.push_back({});
                while (true) {
                    int v = stk.top(); stk.pop();
                    edcc.back().push_back(v);
                    cid[v] = id;
                    if (v == u) break;
                }
            }
        };

        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfs(dfs, i, -1);
            }
        }

        return edcc;
    }

    struct Graph {
        std::vector<std::vector<int>> dots;
        std::vector<std::pair<int, int>> edges;
    };

    Graph compress() {
        auto edcc = tarjan();
        Graph G{std::move(edcc), {}};
        for (int u = 0; u < n; u++) {
            for (auto [eid, v] : adj[u]) {
                if (cid[u] < cid[v]) {
                    G.edges.emplace_back(cid[u], cid[v]);
                }
            }
        }
        return G;
    }

};

void test() {
    int n, m;
    std::cin >> n >> m;
    EDCC g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v; u--, v--;
        g.add_edge(u, v);
    }
    auto [dots, edges] = g.compress();
    std::cout << dots.size() << '\n';
    for (auto &cc : dots) {
        std::cout << cc.size() << ' ';
        for (int u : cc) std::cout << u + 1 << ' ';
        std::cout << '\n';
    }
}