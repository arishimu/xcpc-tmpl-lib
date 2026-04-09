template <class Info>
struct SGT {

    int n;
    std::vector<Info> info;

    void build(std::vector<Info> const &init) {
        auto build_ = [&](auto &f, int p, int l, int r) -> void {
            if (l + 1 == r) return info[p] = init[l], void();
            int m = (l + r) >> 1;
            f(f, p << 1, l, m);
            f(f, p << 1 | 1, m, r);
            info[p] = info[p << 1] + info[p << 1 | 1];
        };
        n = init.size();
        info.resize(n << 2);
        build_(build_, 1, 0, n);
    }

    void modify(int i, Info const &v) { modify(1, 0, n, i, v); }
    void modify(int p, int l, int r, int i, Info const &v) {
        if (l + 1 == r) return info[p] = v, void();
        int m = (l + r) >> 1;
        if (i < m) modify(p << 1, l, m, i, v);
        else modify(p << 1 | 1, m, r, i, v);
        info[p] = info[p << 1] + info[p << 1 | 1];
    }

    Info query(int i) { return query(i, i + 1); }
    Info query(int L, int R) { return query(1, 0, n, L, R); }
    Info query(int p, int l, int r, int L, int R) {
        if (R <= l || r <= L) return {};
        if (L <= l && r <= R) return info[p];
        int m = (l + r) >> 1;
        return query(p << 1, l, m, L, R) + query(p << 1 | 1, m, r, L, R);
    }

};

void test() {

    struct Info {
        i64 S;
        Info() : S(0) {}
        Info(i64 S) : S(S) {}
        Info operator+(Info const &rhs) { return {S + rhs.S}; }
    };

    int n, m;
    std::cin >> n >> m;

    std::vector<Info> arr(n);
    for (auto &[S] : arr) std::cin >> S;
    SGT<Info> sgt;
    sgt.build(arr);

    while (m--) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int i; i64 v;
            std::cin >> i >> v; i--;
            sgt.modify(i, sgt.query(i) + Info{v});
        } else if (op == 2) {
            int l, r;
            std::cin >> l >> r; l--, r--, r++;
            std::cout << sgt.query(l, r).S << '\n';
        }
    }

}