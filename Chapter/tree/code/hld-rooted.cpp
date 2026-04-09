struct HLD {

    ...

    int get_rooted_fa(int root, int p) {
        if (p == root) return p;
        if (!is_ancestor(p, root)) return fa[p];
        auto dfn_order = [&] (int u, int v) { return in[u] < in[v]; };
        auto it = std::upper_bound(adj[p].begin(), adj[p].end(), root, dfn_order) - 1;
        return *it;
    }

    int get_rooted_sz(int root, int p) {
        if (p == root) return n;
        if (!is_ancestor(p, root)) return sz[p];
        return n - sz[get_rooted_fa(root, p)];
    }

    int get_rooted_lca(int a, int b, int c) {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }

};