template <class Info, class Tag, class Merge, class ApplyToInfo, class ApplyToTag,
    class ReadyToApply = decltype([](Tag const &, Info const &){ return true; })>
struct HLD {

    ...

    SegTree<Info, Tag, Merge, ApplyToInfo, ApplyToTag, ReadyToApply> sgt;
    HLD(int n) ... {}
    void add_edge(int u, int v) {...}
    void build(int root) {...}
    void dfs1(int u) {...}
    void dfs2(int u) {...}
    void modify(int u, const Tag &t) { sgt.modify(in[u], t); }
    void subtree_apply(int u, const Tag &t) { sgt.modify(in[u], out[u], t); }
    Info subtree_query(int u) { return sgt.query(in[u], out[u]); }

    void path_apply(int u, int v, const Tag &t) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            sgt.modify(in[top[u]], in[u] + 1, t);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        sgt.modify(in[u], in[v] + 1, t);
    }

    Info undirected_path_query(int u, int v) {
        Info u2v{};
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            u2v = sgt.merge(u2v, sgt.query(in[top[u]], in[u] + 1));
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        return sgt.merge(u2v, sgt.query(in[u], in[v] + 1));
    }

};

void test() {

    int N, M, R;
    std::cin >> N >> M >> R >> Z::p; R--;
    std::vector<i64> a(N);
    for (auto &x : a) std::cin >> x;

    struct Tag { Z z = 0; };
    struct Info { int l, r; Z sum; };

    HLD<Info, Tag, decltype([](Info const &x, Info const &y) {
        return Info{x.l, y.r, x.sum + y.sum};
    }), decltype([](Tag const &t, Info &x) {
        x.sum += (x.r - x.l) * t.z;
    }), decltype([](Tag const &t, Tag &x) {
        x.z += t.z;
    })> hld(N); // step 0: num of nodes
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        std::cin >> u >> v; u--; v--;
        hld.add_edge(u, v); // step 1: add edges
    }
    hld.build(R); // step 2: dfs twice
    hld.sgt.build(0, N, [&](i64 i) { // step 3: init info, build sgt
        int u = hld.seq[i];
        return Info {
            .l = hld.in[u], .r = hld.in[u] + 1,
            .sum = a[u],
        }; // in[] !!!
    });

    while (M--) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int x, y; i64 z;
            std::cin >> x >> y >> z; x--; y--;
            hld.path_apply(x, y, {z});
        } else if (op == 2) {
            int x, y;
            std::cin >> x >> y; x--; y--;
            std::cout << hld.undirected_path_query(x, y).S.val() << '\n';
        } else if (op == 3) {
            int x; i64 z;
            std::cin >> x >> z; x--;
            hld.subtree_apply(x, {z});
        } else if (op == 4) {
            int x;
            std::cin >> x; x--;
            std::cout << hld.subtree_query(x).S.val() << '\n';
        } else if (op == 5) {
            int x; i64 z;
            std::cin >> x >> z; x--;
            hld.modify(x, {z});
        }
    }

}
