auto devisor(i64 x) {
    std::vector<int> div;
    div.push_back(1);
    auto ind = decomp(x);
    for (auto &[p, k] : ind) {
        int sz = div.size();
        i64 pi = 1;
        for (int i = 0; i < k; i++) {
            pi *= p;
            for (int j = 0; j < sz; j++) {
                div.push_back(div[j] * pi);
            }
        }
    }
    return div;
}