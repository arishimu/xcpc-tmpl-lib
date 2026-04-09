void test() {

    std::vector<int> a{2, -3, 4, 1, -5};

    a.emplace_back();
    std::adjacent_difference(a.begin(), a.end(), a.begin());
    a[2] += 10, a[5] -= 10; // a[2, 5) += 10
    std::partial_sum(a.begin(), a.end(), a.begin());
    a.pop_back();
    assert((a == std::vector{2, -3, 14, 11, 5}));

    std::vector<int> p{0};
    std::partial_sum(a.begin(), a.end(), std::back_inserter(p));
    assert(p[4] - p[1] == 22); // sum a[1, 4)

}