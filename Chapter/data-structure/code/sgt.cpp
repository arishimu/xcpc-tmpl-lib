#include <functional>
#include <iostream>
#include <vector>

using i64 = int64_t;

template <class Info, class Merge>
struct SegTree {

    i64 L, R;
    std::vector<Info> info;
    Merge merge;

    template <class Init = decltype([](i64){ return Info{}; })>
    void build(i64 L, i64 R, Init init = {}) {
        auto pull = [&](auto &self, int p, i64 l, i64 r) -> void {
            if (r - l == 1) return info[p] = init(l), void();
            i64 m = l + (r - l) / 2;
            self(self, p * 2, l, m);
            self(self, p * 2 + 1, m, r);
            info[p] = merge(info[p * 2], info[p * 2 + 1]);
        };
        this->L = L, this->R = R;
        info.resize((R - L) * 4);
        pull(pull, 1, L, R);
    }

    void modify(i64 i, auto&& f) { modify(1, L, R, i, f); }
    void modify(int p, i64 l, i64 r, i64 i, auto&& f) {
        if (r - l == 1) return info[p] = f(info[p]), void();
        i64 m = l + (r - l) / 2;
        if (i < m) modify(p * 2, l, m, i, f);
        else modify(p * 2 + 1, m, r, i, f);
        info[p] = merge(info[p * 2], info[p * 2 + 1]);
    }

    Info query(i64 i) { return query(i, i + 1); }
    Info query(i64 a, i64 b) { return query(1, L, R, a, b); }
    Info query(int p, i64 l, i64 r, i64 a, i64 b) {
        if (a <= l && r <= b) return info[p];
        i64 m = l + (r - l) / 2;
        if (b <= m) return query(p * 2, l, m, a, b);
        if (m <= a) return query(p * 2 + 1, m, r, a, b);
        return merge(query(p * 2, l, m, a, b), query(p * 2 + 1, m, r, a, b));
    }

};

void solve() {

    int n, q;
    std::cin >> n >> q;
    std::vector<i64> a(n);
    for (auto &x : a) std::cin >> x;

    SegTree<i64, std::plus<i64>> sgt;
    sgt.build(0, n, [&](i64 i){ return a[i]; });

    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int i; i64 v;
            std::cin >> i >> v;
            sgt.modify(i, [&](i64 x){ return x + v; });
        } else if (op == 1) {
            int l, r;
            std::cin >> l >> r;
            std::cout << sgt.query(l, r) << '\n';
        }
    }

}
