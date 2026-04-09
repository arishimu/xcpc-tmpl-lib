struct DSU {

    std::vector<int> fa, sz;

    DSU(int n) { init(n); }

    void init(int n) {
        fa.resize(n);
        std::iota(fa.begin(), fa.end(), 0);
        sz.assign(n, 1);
    }

    int find(int x) {
        int t, r = x;
        while (fa[r] != r) r = fa[r];
        while (fa[x] != x) t = fa[x], fa[x] = r, x = t;
        return r;
    }

    bool same(int x, int y) { return find(x) == find(y); }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] > sz[y]) std::swap(x, y);
        fa[x] = y, sz[y] += sz[x];
        return true;
    }

};