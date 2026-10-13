#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

using i64 = int64_t;

template <class Info, class Merge,
    class Init = decltype([](i64, i64){ return Info{}; })>
struct SegTree {

    i64 L, R;
    std::vector<int> lson, rson;
    std::vector<Info> info;
    std::optional<Init> init;
    Merge merge;

    int node(i64 l, i64 r) {
        lson.push_back(0), rson.push_back(0);
        info.push_back((*init)(l, r));
        return info.size() - 1;
    }

    void dive(int p, i64 l, i64 m, i64 r) {
        if (!lson[p]) lson[p] = node(l, m);
        if (!rson[p]) rson[p] = node(m, r);
    }

    void build(i64 L, i64 R, Init init = {}) {
        this->L = L, this->R = R, this->init.emplace(init);
        lson.resize(1), rson.resize(1), info.resize(1);
        node(L, R);
    }

    void modify(i64 i, auto&& f) { modify(1, L, R, i, f); }
    void modify(int p, i64 l, i64 r, i64 i, auto&& f) {
        if (r - l == 1) return info[p] = f(info[p]), void();
        i64 m = l + (r - l) / 2;
        dive(p, l, m, r);
        if (i < m) modify(lson[p], l, m, i, f);
        else modify(rson[p], m, r, i, f);
        info[p] = merge(info[lson[p]], info[rson[p]]);
    }

    Info query(i64 i) { return query(i, i + 1); }
    Info query(i64 a, i64 b) { return query(1, L, R, a, b); }
    Info query(int p, i64 l, i64 r, i64 a, i64 b) {
        if (a <= l && r <= b) return info[p];
        i64 m = l + (r - l) / 2;
        dive(p, l, m, r);
        if (b <= m) return query(lson[p], l, m, a, b);
        if (m <= a) return query(rson[p], m, r, a, b);
        return merge(query(lson[p], l, m, a, b), query(rson[p], m, r, a, b));
    }

};

void solve() {

    int n, q;
    std::cin >> n >> q;
    std::vector<i64> a(n), p(n + 1);
    for (auto &x : a) std::cin >> x;
    std::partial_sum(a.begin(), a.end(), p.begin() + 1);

    auto init = [&](i64 l, i64 r){ return p[r] - p[l]; };
    SegTree<i64, std::plus<i64>, decltype(init)> sgt;
    sgt.build(0, n, init);

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
