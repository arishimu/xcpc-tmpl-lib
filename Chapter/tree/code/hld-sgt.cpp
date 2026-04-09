template <class Info, class Tag>
struct HLD {

    ...

    SGT<Info, Tag> sgt;

    HLD(int n) ... {}
    void add_edge(int u, int v) {...}

    void build(int root) {...}
    void build_sgt(std::vector<Info> const &init) {
        std::vector<Info> init_(n);
        for (int i = 0; i < n; i++) init_[in[i]] = init[i];
        sgt.build(init_);
    }

    void dfs1(int u) {...}
    void dfs2(int u) {...}

    void modify(int u, const Info &v) { sgt.modify(in[u], v); }

    void subtree_apply(int u, const Tag &t) { sgt.apply(in[u], out[u], t); }
    Info subtree_query(int u) { return sgt.query(in[u], out[u]); }

    void path_apply(int u, int v, const Tag &t) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            sgt.apply(in[top[u]], in[u] + 1, t);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        sgt.apply(in[u], in[v] + 1, t);
    }

    Info undirected_path_query(int u, int v) {
        Info u2v;
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            u2v = u2v + sgt.query(in[top[u]], in[u] + 1);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        return u2v + sgt.query(in[u], in[v] + 1);
    }

};

void test() {

    int N, M, R;
    std::cin >> N >> M >> R >> Z::p; R--;
    std::vector<i64> a(N);
    for (auto &x : a) std::cin >> x;

    struct Tag {
        Z z;
        Tag() : z(0) {}
        Tag(Z z) : z(z) {}
        void operator+=(Tag const &t) { z += t.z; }
    };

    struct Info {
        int l, r; Z S;
        Info() : l(0), r(0), S(0) {}
        Info(int l, int r, Z S) : l(l), r(r), S(S) {}
        void operator+=(Tag const &t) { S += (r - l) * t.z; }
        Info operator+(Info const &o) { return {l, o.r, S + o.S}; }
    };

    HLD<Info, Tag> hld(N); // step 0: num of nodes
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        std::cin >> u >> v; u--; v--;
        hld.add_edge(u, v); // step 1: add edges
    }
    hld.build(R); // step 2: dfs twice
    std::vector<Info> init(N);
    for (int i = 0; i < N; i++) init[i] = {hld.in[i], hld.in[i] + 1, a[i]}; // in[] !!!
    hld.build_sgt(init); // step 3: init info, build sgt

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
        }
    }

}