template <class Info, class Tag>
struct HLD {

    ...

    Info directed_path_query(int u, int v) {
        Info r2u, r2v;
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                r2u = sgt.query(in[top[u]], in[u] + 1) + r2u;
                u = fa[top[u]];
            } else {
                r2v = sgt.query(in[top[v]], in[v] + 1) + r2v;
                v = fa[top[v]];
            }
        }
        if (dep[u] > dep[v]) r2u = sgt.query(in[v], in[u] + 1) + r2u;
        else r2v = sgt.query(in[u], in[v] + 1) + r2v;
        return {r2u.backw * r2v.forw, r2v.backw * r2u.forw};
    }

};

struct LinearFunc {
    i64 k, b;
    i64 operator()(i64 x) { return k * x + b; }
};

struct DirectedInfo {
    LinearFunc f;
    DirectedInfo() : f(1, 0) {}
    DirectedInfo operator*(DirectedInfo const &o) {
        auto &[k1, b1] = f;
        auto &[k2, b2] = o.f;
        // f1 * f2 != f2 * f1
        // (f1 * f2)(.) = f2(f1(.)) = k2 (k1 x + b1) + b2
        return {LinearFunc{k1 * k2, b1 * k2 + b2}};
    }
};

struct Tag {...};

struct Info {
    DirectedInfo forw, backw;

    Info() : forw(), backw() {}
    Info(DirectedInfo forw, DirectedInfo backw): forw(forw), backw(backw) {}
    
    void operator+=(Tag const &t) {...}
    Info operator+(Info const &o) { return {l, o.r, forw * o.forw, o.backw * backw}; }
};