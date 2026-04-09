template <class T, class Iter>
auto manacher(Iter start, Iter end, T sep) {
    std::vector<T> t; t.push_back(sep);
    while (start != end) t.push_back(*start++), t.push_back(sep);

    int n = t.size();
    std::vector<int> r(n);
    for (int i = 0, m = 0; i < n; i++) {
        int b = m + r[m];
        if (i < b) r[i] = std::min(r[m * 2 - i], b - i);
        int L = i - r[i], R = i + r[i];
        while (0 <= L && R < n && t[L] == t[R]) r[i]++, L--, R++;
        if (R > b) m = i;
    }

    return r;
}

void test() {
    std::string s;
    std::cin >> s;                              // _m_i_s_s_i_s_s_i_p_p_i_
    auto r = manacher(s.begin(), s.end(), '_'); // 12121252181252121252121
    for (int i = 1; i < r.size() - 1; i++) {
        int L = (i - r[i] + 1) / 2;
        int R = (i + r[i] - 1) / 2;
        std::cout << s.substr(L, R - L) << '\n';
        assert(r[i] - 1 == R - L);
    }
}