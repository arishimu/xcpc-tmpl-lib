template <class Iter>
auto kmp(Iter s, Iter e) {
    std::vector<int> pi(e - s);
    for (int i = 1, k = 0; i < pi.size(); i++) {
        while (k > 0 && s[i] != s[k]) k = pi[k - 1];
        if (s[i] == s[k]) k++;
        pi[i] = k;
    }
    return pi;
}

void test() {
    std::string s = "Hello Hello World Hello"; // Hello Hello World Hello
    auto pi = kmp(s.begin(), s.end());         // 00000012345600000012345
    for (auto &x : pi) std::cout << x << '\n';
}