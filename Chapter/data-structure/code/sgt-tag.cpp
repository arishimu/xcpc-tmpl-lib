template <class Info, class Tag>
struct SGT {

    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;

    void apply(int p, Tag const &t) { info[p] += t; tag[p] += t; }
    void pull(int p) { info[p] = info[p << 1] + info[p << 1 | 1]; }
    void push(int p) { apply(p << 1, tag[p]), apply(p << 1 | 1, tag[p]); tag[p] = {}; }

    void build(std::vector<Info> const &init) {
        auto build_ = [&](auto &f, int p, int l, int r) -> void {
            if (l + 1 == r) return info[p] = init[l], void();
            int m = (l + r) >> 1;
            f(f, p << 1, l, m);
            f(f, p << 1 | 1, m, r);
            pull(p);
        };
        n = init.size();
        info.assign(n << 2, {});
        tag.assign(n << 2, {});
        build_(build_, 1, 0, n);
    }

    void modify(int i, Info const &v) { modify(1, 0, n, i, v); }
    void modify(int p, int l, int r, int i, Info const &v) {
        if (l + 1 == r) return info[p] = v, void();
        int m = (l + r) >> 1;
        push(p);
        if (i < m) modify(p << 1, l, m, i, v);
        else modify(p << 1 | 1, m, r, i, v);
        pull(p);
    }

    void apply(int L, int R, Tag const &t) { apply(1, 0, n, L, R, t); }
    void apply(int p, int l, int r, int L, int R, Tag const &t) {
        if (R <= l || r <= L) return;
        if (L <= l && r <= R) return apply(p, t);
        int m = (l + r) >> 1;
        push(p);
        apply(p << 1, l, m, L, R, t);
        apply(p << 1 | 1, m, r, L, R, t);
        pull(p);
    }

    Info query(int i) { return query(i, i + 1); }
    Info query(int L, int R) { return query(1, 0, n, L, R); }
    Info query(int p, int l, int r, int L, int R) {
        if (R <= l || r <= L) return {};
        if (L <= l && r <= R) return info[p];
        int m = (l + r) >> 1;
        push(p);
        return query(p << 1, l, m, L, R) + query(p << 1 | 1, m, r, L, R);
    }

};

void test() {

    int n, q, m;
    std::cin >> n >> q >> m;
    Z::p = m;

    // tag: ax + b
    // tag -> tag: A(ax+b)+B = (Aa)x+(Ab+B)
    // tag -> info: sum(ax+b) = (a)sum(x)+(nb)

    struct Tag {
        Z a, b;
        Tag() : a(1), b(0) {}
        Tag(Z a, Z b) : a(a), b(b) {}
        void operator+=(Tag const &t) { a *= t.a; b *= t.a; b += t.b; }
    };

    struct Info {
        Z S;
        int l, r;
        Info() : S(0), l(0), r(0) {}
        Info(Z S, int l, int r) : S(S), l(l), r(r) {}
        Info operator+(Info const &b) { return {S + b.S, l, b.r}; }
        void operator+=(Tag const &t) { S *= t.a; S += (r - l) * t.b; }
    };

    std::vector<Info> arr(n);
    for (int i = 0; i < n; i++) {
        i64 x;
        std::cin >> x;
        arr[i] = {x, i, i + 1};
    }

    SGT<Info, Tag> sgt;
    sgt.build(arr);
    while (q--) {
        int op, x, y;
        std::cin >> op >> x >> y; x--; y--; y++;
        if (op == 1) {
            i64 k;
            std::cin >> k;
            sgt.apply(x, y, {k, 0});
        } else if (op == 2) {
            i64 k;
            std::cin >> k;
            sgt.apply(x, y, {1, k});
        } else if (op == 3) {
            std::cout << sgt.query(x, y).S.val() << '\n';
        }
    }

}