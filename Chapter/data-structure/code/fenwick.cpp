#include <algorithm>
#include <iostream>
#include <vector>

using i64 = int64_t;

template <class Info, class Merge>
struct Fenwick {

    int n;
    Info id;
    Merge merge;
    std::vector<Info> info;

    Fenwick(int n, Info id = {}) : n(n), id(id), info(n + 1) {}
    Fenwick(std::vector<Info> const &v, Info id = {}) : Fenwick(v.size(), id) {
        for (int i = 1; i <= n; i++) {
            info[i] = merge(info[i], v[i - 1]);
            int j = i + (i & -i);
            if (j <= n) info[j] = merge(info[j], info[i]);
        }
    }

    void add(int k, Info v) {
        for (int i = k + 1; i <= n; i += i & -i)
            info[i] = merge(info[i], v); }

    Info ask(int k) {
        Info acc = id;
        for (int i = k; i >= 1; i -= i & -i)
            acc = merge(acc, info[i]);
        return acc;
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
    disc.insert(INT64_MAX);

    disc.mapping();
    std::vector<int> cnt(disc.size());
    for (auto &x : a) cnt[disc.enc(x)] += 1;
    Fenwick<int, std::plus<int>> fwk(cnt);

    while (q--) {
        int k;
        std::cin >> k; // 0-based k-min
        int acc = 0, cur = 0, w = 31 - __builtin_clz(cnt.size());
        for (int p = 1 << w; p; p >>= 1) {
            if ((cur | p) > n) continue;
            int t = acc + fwk.info[cur | p];
            if (t > k) continue;
            acc = t, cur |= p;
        }
        std::cout << disc.dec(cur) << '\n';
    }

}
