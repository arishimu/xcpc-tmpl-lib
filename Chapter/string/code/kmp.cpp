void test() {

    // Hello Hello World Hello
    // 00000012345600000012345
    auto prefix_function = [&] (auto const& s) {
        std::vector<int> pi(s.size());
        for (int i = 1, p = 0; i < s.size(); i++) {
            while (p > 0 && s[p] != s[i]) p = pi[p - 1];
            if (s[p] == s[i]) pi[i] = ++p;
        }
        return pi;
    };

    auto kmp_automaton = [&] (auto const& pat, auto const& sigma) {
        auto aut = std::vector(pat.size() + 1, std::vector<int>(sigma.size()));
        auto pi = prefix_function(pat);
        for (int i = 0; i <= pat.size(); i++) {
            for (int c = 0; c < sigma.size(); c++) {
                if (i < pat.size() && pat[i] == sigma[c]) aut[i][c] = i + 1;
                else if (i > 0) aut[i][c] = aut[pi[i - 1]][c];
                else aut[i][c] = 0;
            }
        }
        return aut;
    };

    std::string src = "Hello Hello World Hello";
    std::string pat = "Hello ";

    std::string sigma;
    std::unordered_map<char, int> cid;
    for (char ch : src) if (!cid.count(ch)) cid[ch] = cid.size(), sigma.push_back(ch);
    auto aut = kmp_automaton(pat, sigma);

    int state = 0;
    for (char ch : src) {
        state = aut[state][cid[ch]];
        std::cout << state << '\n';
    }

}