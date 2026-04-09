void test() {
    int a[] = {0, 1, 2, 3, 4};
    do { for (auto x : a) std::cout << x << ' '; std::cout << '\n'; }
    while (std::next_permutation(a, a + 5));
    assert(std::equal(a, a + 5, (int[]){0, 1, 2, 3, 4}));
}