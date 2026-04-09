#include <chrono>
#include <functional>
#include <ext/pb_ds/assoc_container.hpp>

struct A {
    int x; double y;
    bool operator==(A const &o) const { return x == o.x && y == o.y; };
};

struct B {
    A a; bool z;
    bool operator==(B const &o) const { return a == o.a && z == o.z; };
};

struct Hasher {

    static u64 magic(u64 x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    // default
    template <class T>
    u64 operator()(T const &x) const {
        static const auto tp = std::chrono::steady_clock::now();
        static const u64 eps = tp.time_since_epoch().count();
        return magic(std::hash<T>()(x) + eps);
    }

    template <class ...T>
    u64 mix(T ...x) const { return ((*this)(x) ^ ...); }

    // pair
    template <class T, class U>
    u64 operator()(std::pair<T, U> const &p) const { return mix(p.first, p.second); }

    // tuple
    template <class ...T>
    u64 operator()(std::tuple<T...> const &t) const
    { return std::apply([this](T const &...x) { return mix(x...); }, t); }

    // vector
    template <class T>
    u64 operator()(std::vector<T> const &v) const {
        u64 result = 0;
        for (auto const &x : v) result ^= (*this)(x);
        return result;
    }

    // user
    u64 operator()(A const &a) const { return mix(a.x, a.y); }
    u64 operator()(B const &b) const { return mix(b.a, b.z); }

};

template<class S, class T>
using umap = __gnu_pbds::gp_hash_table<S, T, Hasher>;

int main() {
    std::unordered_set<std::tuple<int, double, std::string>, Hasher> S0;
    std::unordered_set<std::pair<A, B>, Hasher> S1;
    umap<B, int> S2;

    S1.insert({{8, 1.0}, {}});
    S1.insert({{7, 1.0}, {{}, true}});
    S1.insert({{8, 1.0}, {}});
    S1.insert({{6, 1.0}, {}});
    S1.insert({{7, 1.0}, {{}, true}});
    
    assert(S1.size() == 3);
}