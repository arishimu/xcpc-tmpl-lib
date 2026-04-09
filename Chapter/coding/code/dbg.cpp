namespace dbg {

    // template <class ...T>
    // std::ostream& operator<<(std::ostream& os, std::tuple<T...> const &ts) {
    //     os << '(';
    //     std::apply([&](T const &...t) { ((os << t << ','), ...); }, ts);
    //     return os << ')';
    // }

    // template <class T>
    // std::ostream& operator<<(std::ostream& os, std::vector<T> const &v) {
    //     os << '['; for (T const &x : v) os << x << ','; return os << ']';
    // }

    template <class T>
    void out(const char *s, T const &t) { std::cerr << s << '=' << t << '\n'; }

    template <class T, class ...U>
    void out(const char *s, T const &t, U const &...u) {
        while (*s != ',') std::cerr << *s++;
        std::cerr << '=' << t << ',';
        out(s + 1, u...);
    }

    #define dbg(...) dbg::out(#__VA_ARGS__, __VA_ARGS__)

}

void test() {
    int x = 114; double y = 5.14;
    auto p = std::pair{1, 2}; auto t = std::tuple{4., 5., 6.};
    std::vector<int> b{1, 2, 3, 4};
    std::vector<std::string> a{"deadbeef", "/bin/sh", "btw", "arch"};
    dbg(x, y, p, t, b); dbg(114514); dbg(a);
}