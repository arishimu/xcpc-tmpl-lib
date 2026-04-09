struct PDSU {

    int n;
    std::vector<int> fa, sz;
    std::vector<int> root, lson, rson;

    PDSU(int n, int m) : n(n) {
        int z = n * 4 + m * (32 - __builtin_clz(n));
        root.reserve(m), lson.reserve(z), rson.reserve(z);
        fa.reserve(z), sz.reserve(z);
        root.push_back(build(0, n));
    }

    int node(int lch, int rch, int f, int z) {
        lson.push_back(lch), rson.push_back(rch);
        fa.push_back(f), sz.push_back(z);
        return lson.size() - 1;
    }

    int build(int l, int r) {
        if (l + 1 == r) return node(-1, -1, l, 1);
        int m = (l + r) >> 1;
        return node(build(l, m), build(m, r), -1, 0);
    }

    int latest() { return root.size() - 1; }
    int latest_() { return root.back(); }

    void save(int v) { save_(root[v]); }
    void save_(int rt) { root.push_back(rt); }

    int leaf(int v, int i) { return leaf_(root[v], i); }
    int leaf_(int rt, int i) { return leaf_(rt, 0, n, i); }
    int leaf_(int rt, int l, int r, int i) {
        if (l + 1 == r) return rt;
        int m = (l + r) >> 1;
        if (i < m) return leaf_(lson[rt], l, m, i);
        return leaf_(rson[rt], m, r, i);
    }

    int modify(int v, int i, int f, int z) { return modify_(root[v], i, f, z); }
    int modify_(int rt, int i, int f, int z) { return modify_(rt, 0, n, i, f, z); }
    int modify_(int rt, int l, int r, int i, int f, int z) {
        if (l + 1 == r) return node(-1, -1, f, z);
        int m = (l + r) >> 1;
        if (i < m) return node(modify_(lson[rt], l, m, i, f, z), rson[rt], -1, 0);
        return node(lson[rt], modify_(rson[rt], m, r, i, f, z), -1, 0);
    }

    struct find_result_t { int leaf, index; };
    auto find(int v, int i) -> find_result_t { return find_(root[v], i); }
    auto find_(int rt, int i) -> find_result_t {
        int p = leaf_(rt, i);
        while (fa[p] != i) i = fa[p], p = leaf_(rt, i);
        return {p, i};
    }

    bool same(int v, int i, int j) { return find(v, i).index == find(v, j).index; }
    bool same_(int rt, int i, int j) { return find_(rt, i).index == find_(rt, j).index; }

    void merge(int v, int i, int j) { merge_(root[v], i, j); }
    void merge_(int rt, int i, int j) { save_(merged_(rt, i, j)); }
    int merged(int v, int i, int j) { return merged_(root[v], i, j); }
    int merged_(int rt, int i, int j) {
        auto [p, x] = find_(rt, i);
        auto [q, y] = find_(rt, j);
        if (x == y) return rt;
        if (sz[p] > sz[q]) std::swap(p, q), std::swap(x, y);
        return modify_(modify_(rt, x, y, sz[p]), y, y, sz[p] + sz[q]);
    }

};

void test() {
    PDSU dsu(100, 10);                         // v0
    dsu.merge(dsu.latest(), 1, 2);             // v1
    dsu.merge(dsu.latest(), 2, 3);             // v2
    dsu.merge(0, 1, 3);                        // v3
    int rt = dsu.latest_();                    // v3
    dsu.merge_(rt, 3, 4);                      // v4
    assert(dsu.same(dsu.latest(), 1, 4));
    int v = dsu.latest();
    rt = dsu.merged(v, 4, 5);                  // temp
    rt = dsu.merged_(rt, 5, 6);                // temp
    rt = dsu.merged_(rt, 6, 7);                // temp
    dsu.save_(rt);                             // v5
    dsu.save(dsu.latest());                    // v6 = v5
    assert(dsu.find(dsu.latest(), 1).index
        == dsu.find(dsu.latest(), 7).index);
    dsu.save(3);                               // v7 = v3
    assert(!dsu.same(dsu.latest(), 2, 3));
}