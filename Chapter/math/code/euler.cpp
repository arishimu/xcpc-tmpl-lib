std::vector<int> minp, prime;
// std::vector<int> phi;

void euler_sieve(int n) {
    minp.assign(n + 1, 0);
    prime.clear();
    // phi.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            prime.push_back(i);
            // phi[i] = i - 1;
        }
        for (int p : prime) {
            if (i * p > n) break;
            minp[i * p] = p;
            if (p == minp[i]) {
                // phi[i * p] = phi[i] * p;
                break;
            }
            // phi[i * p] = phi[i] * phi[p];
        }
    }
}