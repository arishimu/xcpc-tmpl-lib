struct Z {

    int x;
    Z() : x(0) {}
    Z(i64 x) : x(norm(x % p)) {}

    int val() const { return x; }
    Z inv() const { return fpow(*this, p - 2); }
    Z operator-() const { return {norm(-x)}; }
    Z& operator+=(Z b) & { x = norm(x + b.x); return *this; }
    Z& operator-=(Z b) & { x = norm(x - b.x); return *this; }
    Z& operator*=(Z b) & { x = 1LL * x * b.x % p; return *this; }
    Z& operator/=(Z b) & { return (*this) *= b.inv(); }

    friend Z operator+(Z a, Z b) { return a += b; }
    friend Z operator-(Z a, Z b) { return a -= b; }
    friend Z operator*(Z a, Z b) { return a *= b; }
    friend Z operator/(Z a, Z b) { return a /= b; }
    friend bool operator==(Z a, Z b) { return a.val() == b.val(); }
    friend bool operator!=(Z a, Z b) { return a.val() != b.val(); }

    static int p;
    static int norm(int x)
    { x += x >> 31 & p; if (x >= p) x -= p; return x; }
    static Z fpow(Z a, int b)
    { Z ans = 1; for (; b; b >>= 1, a *= a) if (b & 1) ans *= a; return ans; }

};

int Z::p = 998244353;

void test() {
    Z a = 114, b = -115;
    assert(a + b == Z::p - 1);
    assert((a *= 2).inv() == (Z{1} / 228));
}