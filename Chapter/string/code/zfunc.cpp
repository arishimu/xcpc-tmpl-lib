template <class Iter>
auto zfunc(Iter s, Iter e) {
    std::vector<int> z(e - s); z.front() = z.size();
    for (int i = 1, p = 0; i < z.size(); i++) {
        int b = p + z[p];
        z[i] = std::max(std::min(z[i - p], b - i), 0);
        while (i + z[i] < z.size() && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > b) p = i;
    }
    return z;
}

void test() {
    std::string s = "Hello Hello World Hello"; // Hello Hello World Hello
    auto z = zfunc(s.begin(), s.end());        // n0000060000000000050000
    for (auto &x : z) std::cout << x << '\n';
}