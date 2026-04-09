struct DSU {

    std::vector<int> fa, sz;
    std::vector<int> xor_with_root; // f(x, rx)

    DSU(int n) { init(n); }

    void init(int n) {
        fa.resize(n);
        std::iota(fa.begin(), fa.end(), 0);
        sz.assign(n, 1);
        xor_with_root.assign(n, 0); // x ^ x = 0
    }

    int find(int x) {
        if (fa[x] == x) return x;
        int root = find(fa[x]); // fa[fa[x]] == new_rt
        xor_with_root[x] ^= xor_with_root[fa[x]]; // fa[x] == old_rt
        return fa[x] = root; // fa[x] == new_rt
    }

    bool merge(int x, int y, int w) {
        int rx = find(x), ry = find(y);
        // f(rx, ry) = ~f(x, rx) * f(x, y) * f(y, ry)
        //           =  f(x, rx) * f(x, y) * f(y, ry)
        w = xor_with_root[x] ^ w ^ xor_with_root[y];
        if (rx == ry) return w == 0;
        // f(ry, rx) = ~f(rx, ry) =  f(rx, ry)
        if (sz[rx] > sz[ry]) std::swap(rx, ry), w = w;
        fa[x] = y, sz[y] += sz[x];
        xor_with_root[x] = w;
        return true;
    }

};