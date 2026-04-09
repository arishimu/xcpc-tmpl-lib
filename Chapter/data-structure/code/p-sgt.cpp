template <class Info>
struct PSGT {

    int n;
    std::vector<int> root, lson, rson;
    std::vector<Info> info;

    int node() {
        lson.emplace_back();
        rson.emplace_back();
        info.emplace_back();
        return info.size() - 1;
    }

    int clone(int p) {
        lson.push_back(lson[p]);
        rson.push_back(rson[p]);
        info.push_back(info[p]);
        return info.size() - 1;
    }

    void build(std::vector<Info> const &init, int m) {
        auto build_ = [&](auto &f, int l, int r) -> int {
            int p = node();
            if (l + 1 == r) return info[p] = init[l], p;
            int m = (l + r) >> 1;
            lson[p] = f(f, l, m);
            rson[p] = f(f, m, r);
            info[p] = info[lson[p]] + info[rson[p]];
            return p;
        };
        node();
        n = init.size();
        root.reserve(m);
        int z = n * 4 + m * (32 - __builtin_clz(n));
        lson.reserve(z), rson.reserve(z), info.reserve(z);
        root.push_back(build_(build_, 0, n));
    }

    int latest() { return root.size() - 1; }
    int latest_() { return root.back(); }
    void save(int ver) { save_(root[ver]); }
    void save_(int p) { root.push_back(p); }

    void modify(int ver, int i, Info const &v) { save_(modified(ver, i, v)); }
    void modify_(int p, int i, Info const &v) { save_(modified_(p, i, v)); }

    int modified(int ver, int i, Info const &v) { return modified_(root[ver], i, v); }
    int modified_(int p, int i, Info const &v) { return modified_(p, 0, n, i, v); }
    int modified_(int p, int l, int r, int i, Info const &v) {
        p = clone(p);
        if (l + 1 == r) return info[p] = v, p;
        int m = (l + r) >> 1;
        if (i < m) lson[p] = modified_(lson[p], l, m, i, v);
        else rson[p] = modified_(rson[p], m, r, i, v);
        info[p] = info[lson[p]] + info[rson[p]];
        return p;
    }

    Info query(int ver, int L, int R) { return query_(root[ver], L, R); }
    Info query_(int p, int L, int R) { return query_(p, 0, n, L, R); }
    Info query_(int p, int l, int r, int L, int R) {
        if (r <= L || R <= l) return {};
        if (L <= l && r <= R) return info[p];
        int m = (l + r) >> 1;
        return query_(lson[p], l, m, L, R) + query_(rson[p], m, r, L, R);
    }

};

void test() {

    int n, m;
    std::cin >> n >> m;

    struct Info {
        i64 S;
        Info() : S(0) {}
        Info(i64 S) : S(S) {}
        Info operator+(Info const &rhs) { return {S + rhs.S}; }
    };

    PSGT<Info> sgt;
    std::vector<Info> arr(n);
    for (auto &[S] : arr) std::cin >> S;
    sgt.build(arr, m);

    while (m--) {
        int v, op, i;
        std::cin >> v >> op >> i; i--;
        if (op == 1) {
            i64 c;
            std::cin >> c;
            sgt.modify(v, i, {c});
        } else if (op == 2) {
            std::cout << sgt.query(v, i, i + 1).S << '\n';
            sgt.save(v);
        }
    }

}