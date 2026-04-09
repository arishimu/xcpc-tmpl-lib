// x = x0 + b/d * t
// y = y0 - a/d * t
void exgcd(i64 a, i64 b, i64 &x, i64 &y, i64 &d) {
    i64 M[2][3] = {{1, 0, a}, {0, 1, b}};
    i64 *cur, *pre;
    for (cur = M[0], pre = M[1]; pre[2]; std::swap(cur, pre)) {
        if (std::abs(cur[2]) >= std::abs(pre[2])) {
            i64 q = cur[2] / pre[2];
            cur[0] -= q * pre[0];
            cur[1] -= q * pre[1];
            cur[2] -= q * pre[2];
        }
    }
    x = cur[0], y = cur[1], d = cur[2];
    if (d < 0) x = -x, y = -y, d = -d;
}