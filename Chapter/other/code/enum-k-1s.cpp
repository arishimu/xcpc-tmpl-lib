void test() {
    int k = 5, n = 8;
    // 00011111 00101111 00110111 ... 11111000
    for (int s = (1 << k) - 1, t; s < (1 << n);) {
        std::cout << std::bitset<8>(s) << '\n';
        t = s + (s & -s);
        s = (s & ~t) >> (__builtin_ctz(s & -s) + 1) | t;
    }
}