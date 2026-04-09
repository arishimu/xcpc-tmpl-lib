template <class T, class F>
struct Fenwick {

    int n; F f;
    std::vector<T> p;
    Fenwick(F f) : f(f) {}

    void build(std::vector<T> const &a) {
        n = a.size();
        p.assign(n + 1, {});
        for (int x = 1; x <= n; x++) {
            p[x] = f(p[x], a[x - 1]);
            int y = x + (x & -x);
            if (y > n) continue;
            p[y] = f(p[y], p[x]);
        }
    }

    // i \in [0, n)
    void modify(int i, T const &v) {
        for (int x = i + 1; x <= n; x += x & -x)
            p[x] = f(p[x], v);
    }

    // return S[0, k), where k \in [1, n]
    T query(int k) {
        T ans = T{};
        for (int x = k; x >= 1; x -= x & -x)
            ans = f(ans, p[x]);
        return ans;
    }

    // return r \in [1, n], where S[0, r) is the largest of <= v
    int find(T const &v) {
        T acc{}; int r = 0, width = 32 - __builtin_clz(n);
        for (int mask = 1 << (width - 1); mask; mask >>= 1) {
            if ((r | mask) > n) continue;
            T tmp = f(acc, p[r | mask]);
            if (tmp > v) continue;
            r |= mask;
            acc = tmp;
        }
    }

};

void test() {
    int n, m;
    std::cin >> n >> m;
    std::vector<i64> arr(n);
    for (auto &x : arr) std::cin >> x;

    using add = std::plus<i64>;
    Fenwick<i64, add> fw(add{});
    fw.build(arr);

    while (m--) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int i; i64 v;
            std::cin >> i >> v; i--;
            fw.modify(i, v);
        } else if (op == 2) {
            int l, r;
            std::cin >> l >> r; l--, r--, r++;
            std::cout << fw.query(r) - fw.query(l) << '\n';
        }
    }
}