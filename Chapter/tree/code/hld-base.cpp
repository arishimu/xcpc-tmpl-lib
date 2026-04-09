struct HLD {

    int n;
    std::vector<int> fa, sz, top, dep, in, out, seq;
    std::vector<std::vector<int>> adj;

    HLD(int n) : n(n), fa(n), sz(n), top(n), dep(n), in(n), out(n), seq(n), adj(n) {}
    void add_edge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }

    void build(int root) {
        top[root] = root, dep[root] = 0, fa[root] = -1;
        dfs1(root), dfs2(root);
    }

    void dfs1(int u) {
        if (fa[u] != -1) {
            auto it = std::find(adj[u].begin(), adj[u].end(), fa[u]);
            adj[u].erase(it);
        }
        sz[u] = 1;
        for (auto& v : adj[u]) {
            fa[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            sz[u] += sz[v];
            auto& h = adj[u].front();
            if (sz[v] > sz[h]) std::swap(v, h);
        }
    }

    void dfs2(int u) {
        static int dfn = 0;
        in[u] = dfn++;
        seq[in[u]] = u;
        for (auto v : adj[u]) {
            if (v == adj[u].front()) top[v] = top[u];
            else top[v] = v;
            dfs2(v);
        }
        out[u] = dfn;
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) u = fa[top[u]];
            else v = fa[top[v]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    int leap(int u, int k) {
        int d = dep[u] - k;
        if (d < 0) return -1;
        while (dep[top[u]] > d) u = fa[top[u]];
        return seq[in[u] + d - dep[u]];
    }

    int dist(int u, int v) { return dep[u] + dep[v] - dep[lca(u, v)] * 2; }
    bool is_ancestor(int anc, int ch) { return in[anc] <= in[ch] && in[ch] < in[anc]; }

};

void test() {
    int n, m, s;
    std::cin >> n >> m >> s; s--;

    HLD tr(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v; u--, v--;
        tr.add_edge(u, v);
    }
    tr.build(s);

    while (m--) {
        int a, b;
        std::cin >> a >> b; a--, b--;
        std::cout << tr.lca(a, b) + 1 << '\n';
    }
}