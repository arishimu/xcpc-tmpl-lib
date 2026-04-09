#include <chrono>
#include <random>

struct SeqHasher {

    struct Proxy {
        std::vector<u64> P;
        std::vector<std::vector<u64>> H, B;
        auto get(int l, int r) {
            std::vector<u64> A(H.size());
            for (int i = 0; i < H.size(); i++) {
                auto &a = A[i], &p = P[i];
                auto &h = H[i], &b = B[i];
                a = h[r] + (p - h[l]) * b[r - l];
                a %= p;
            }
            return A;
        }
    };

    template <class Iter>
    auto hash(Iter start, Iter end) {
        Proxy proxy;
        auto &[P, H, B] = proxy;

        P = mods;
        H.assign(mods.size(), {0});
        B.assign(mods.size(), {1});

        for (int k = 0; k < P.size(); k++) {
            int i = 0;
            auto &p = P[k]; auto &h = H[k], &b = B[k];
            for (auto it = start; it != end; i++) {
                h.push_back(h[i] * base + (*it++));
                b.push_back(b[i] * base);
                h.back() %= p;
                b.back() %= p;
            }
        }

        return proxy;
    }

    u64 base;
    std::vector<u64> mods;

    template <class ...U>
    SeqHasher(U ...mods) : mods{u64(mods)...} { set_base(); }

    void set_base() {
        auto is_prime = [](u64 n) {
            if (n <= 1) return false;
            for (u64 i = 2; i * i <= n; i++) if (n % i == 0) return false;
            return true;
        };
        auto tp = std::chrono::steady_clock::now();
        auto rng = std::mt19937_64(tp.time_since_epoch().count());
        base = rng() % 9982 + 1007;
        while (!is_prime(base)) base++;
    }

};

void test() {
    int n;
    std::cin >> n;
    std::vector<std::vector<u64>> a;
    SeqHasher hasher(998244353, 1e9 + 7, 1e9 + 9);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        auto proxy = hasher.hash(s.begin(), s.end());
        auto hash = proxy.get(0, s.size());
        a.push_back(hash);
    }
    std::sort(a.begin(), a.end());
    std::cout << std::unique(a.begin(), a.end()) - a.begin() << '\n';
}