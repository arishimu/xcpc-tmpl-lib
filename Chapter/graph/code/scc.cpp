struct SCC {

    int n;
    std::vector<int> cid;
    std::vector<std::vector<int>> adj;
    SCC(int n) : n(n), adj(n) {}
    void add_edge(int u, int v) { adj[u].push_back(v); }

    auto tarjan() {
        std::vector<std::vector<int>> scc;
        cid.assign(n, -1);

        int timer = 0;
        std::stack<int> stk;
        std::vector<int> dfn(n, -1), low(n, -1);

        auto dfs = [&](auto &f, int u) -> void {
            low[u] = dfn[u] = timer++;
            stk.push(u);
            for (int v : adj[u]) {
                if (dfn[v] == -1) f(f, v), low[u] = std::min(low[u], low[v]);
                else if (cid[v] == -1) low[u] = std::min(low[u], dfn[v]);
            }
            if (low[u] == dfn[u]) {
                int id = scc.size();
                scc.push_back({});
                while (true) {
                    int v = stk.top(); stk.pop();
                    scc.back().push_back(v);
                    cid[v] = id;
                    if (v == u) break;
                }
            }
        };

        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfs(dfs, i);
            }
        }

        return scc;
    }

    struct Graph {
        std::vector<std::vector<int>> dots;
        std::vector<std::pair<int, int>> edges;
    };

    Graph compress() {
        auto scc = tarjan();
        Graph G{std::move(scc), {}};
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (cid[u] == cid[v]) continue;
                G.edges.emplace_back(cid[u], cid[v]);
            }
        }
        return G;
    }

};

void test() {
    int n, m;
    std::cin >> n >> m;
    SCC g(n);
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