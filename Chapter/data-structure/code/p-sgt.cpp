#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <vector>

using i64 = int64_t;

template <class Info, class Merge,
    class Init = decltype([](i64, i64){ return Info{}; })>
struct PstSegTree {

    i64 L, R;
    std::vector<int> lson, rson;
    std::vector<Info> info;
    std::optional<Init> init;
    Merge merge;

    auto node(i64 l, i64 r) -> int {
        lson.push_back(0);
        rson.push_back(0);
        info.push_back((*init)(l, r));
        return info.size() - 1;
    }

    void dive(int p, i64 l, i64 m, i64 r) {
        if (!lson[p]) lson[p] = node(l, m);
        if (!rson[p]) rson[p] = node(m, r);
    }

    auto clone(int p) -> int {
        lson.push_back(lson[p]);
        rson.push_back(rson[p]);
        info.push_back(info[p]);
        return info.size() - 1;
    }

    auto build(i64 L, i64 R, Init init = {}) -> int {
        this->L = L, this->R = R, this->init.emplace(init);
        lson.resize(1), rson.resize(1), info.resize(1);
        return node(L, R);
    }

    int modify(int p, i64 i, auto&& f) { return modify(p, L, R, i, f); }
    int modify(int p, i64 l, i64 r, i64 i, auto&& f) {
        p = clone(p);
        if (r - l == 1) return info[p] = f(info[p]), p;
        i64 m = l + (r - l) / 2;
        dive(p, l, m, r);
        if (i < m) lson[p] = modify(lson[p], l, m, i, f);
        else rson[p] = modify(rson[p], m, r, i, f);
        info[p] = merge(info[lson[p]], info[rson[p]]);
        return p;
    }

    Info query(int p, i64 i) { return query(p, i, i + 1); }
    Info query(int p, i64 a, i64 b) { return query(p, L, R, a, b); }
    Info query(int p, i64 l, i64 r, i64 a, i64 b) {
        if (a <= l && r <= b) return info[p];
        i64 m = l + (r - l) / 2;
        dive(p, l, m, r);
        if (b <= m) return query(lson[p], l, m, a, b);
        if (m <= a) return query(rson[p], m, r, a, b);
        return merge(query(lson[p], l, m, a, b), query(rson[p], m, r, a, b));
    }

};

// discrete -> {0, 1, 2, ...}
template <class T, class Comp = std::less<T>>
struct Disc {
    std::vector<T> pool;
    void clear() { pool.clear(); }
    auto size() { return pool.size(); }
    void insert(T const &x) { pool.push_back(x); }
    void mapping() {
        std::sort(pool.begin(), pool.end());
        pool.erase(std::unique(pool.begin(), pool.end()), pool.end());
    }
    auto dec(int id) -> T { return pool[id]; }
    auto enc(T const &x) -> int {
        auto it = std::lower_bound(pool.begin(), pool.end(), x);
        return it - pool.begin();
    }
};

void solve() {

    int n, q;
    std::cin >> n >> q;
    Disc<i64> disc;
    std::vector<i64> a(n);
    for (auto &x : a) std::cin >> x, disc.insert(x);

    disc.mapping();
    std::vector<int> b(n);
    for (int i = 0; i < b.size(); i++) b[i] = disc.enc(a[i]);

    std::vector<int> ver;
    PstSegTree<int, std::plus<int>> sgt;
    ver.push_back(sgt.build(0, disc.size()));
    for (int v : b) ver.push_back(
        sgt.modify(ver.back(), v, [&](int x){ return x + 1; }));

    // v0 v1 v2 v3 ...
    //    a0 a1 a2 ...
    // [0, 2) -> v2 - v0
    // k = cnt(x < bound)

    while (q--) {
        int l, r, k;
        std::cin >> l >> r >> k;
        int cnt = 0, bound = 0;
        auto dfs = [&] (auto &self, int p, int q, int lo, int hi) -> bool {
            int frq = sgt.info[q] - sgt.info[p];
            if (cnt + frq <= k) { cnt += frq; return false; }
            if (hi - lo == 1) { bound = lo; return true; }
            i64 m = lo + (hi - lo) / 2;
            sgt.dive(p, lo, m, hi), sgt.dive(q, lo, m, hi);
            if (self(self, sgt.lson[p], sgt.lson[q], lo, m)) return true;
            if (self(self, sgt.rson[p], sgt.rson[q], m, hi)) return true;
            return false;
        };
        dfs(dfs, ver[l], ver[r], 0, disc.size());
        std::cout << disc.dec(bound) << '\n';
    }

}
