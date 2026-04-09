auto decomp(i64 x) {
    std::map<int, int> ind;
    if (x == 1) return ind;
    for (int p : prime) {
        if (1LL * p * p > x) break;
        int k = 0;
        while (x % p == 0) k++, x /= p;
        if (k > 0) ind[p] = k;
    }
    if (x > 1) ind[x] = 1;
    return ind;
}