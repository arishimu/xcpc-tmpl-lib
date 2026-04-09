std::vector<int> fa(n), sz(n), w(n), cent(n);
auto pull = [&](auto &f, int u, int fa_) -> void {
    fa[u] = fa_, sz[u] = 1;
    for (int v : adj[u]) {
        if (v == fa) continue;
        f(f, v, u);
        sz[u] += sz[v];
        w[u] = std::max(w[u], sz[v]);
    }
    cent[u] = u;
    for (int v : adj[u]) {
        if (v == fa) continue;
        for (int p = cent[v]; p != u; p = fa[p]) {
            if (std::max(w[p], sz[u] - sz[p]) <= sz[u] / 2) {
                cent[u] = p;
                break;
            }
        }
    }
}