struct C {

    static int n;
    static std::vector<Z> I, F, FI;

    static Z inv(int x)  { if (x > n) resize(x << 1); return I[x]; }
    static Z fact(int x) { if (x > n) resize(x << 1); return F[x]; }
    static Z finv(int x) { if (x > n) resize(x << 1); return FI[x]; }
    static Z binom(int N, int M) { return fact(N) * finv(M) * finv(N - M); }

    static void resize(int x) {
        if (x <= n) return;
        I.resize(x + 1), F.resize(x + 1), FI.resize(x + 1);
        for (int i = n + 1; i <= x; i++) F[i] = F[i - 1] * i;
        FI[x] = F[x].inv(), I[x] = FI[x] * F[x - 1];
        for (int i = x - 1; i >= n + 1; i--)
            FI[i] = FI[i + 1] * (i + 1), I[i] = FI[i] * F[i - 1];
        n = x;
    }

};

int C::n;
std::vector<Z> C::I{0}, C::F{1}, C::FI{1};

void test() {
    assert(C::inv(2) == 499122177); // Z::p = 998244353;
    assert(C::fact(6) == 720);
    assert(C::finv(3) == Z{6}.inv());
    assert(C::binom(6, 3) == 20);
}