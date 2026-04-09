void test() {
    int L = -114, R = 514;       // [L, R)
    while (R - L > 1) {
        int M = (L + R) / 2;
        if (judge(M)) L = M;     // [M, R)
        else R = M;              // [L, M)
    }
    if (judge(L)) std::cout << "NO" << '\n';
    else std::cout << L << '\n';
}