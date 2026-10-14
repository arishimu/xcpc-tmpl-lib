template <class Info, class Tag, class Merge, class ApplyToInfo, class ApplyToTag,
    class ReadyToApply = decltype([](Tag const &, Info const &){ return true; })>
struct HLD {

    ...

    Info directed_path_query(int u, int v) {
        Info r2u, r2v;
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                r2u = sgt.merge(sgt.query(in[top[u]], in[u] + 1), r2u);
                u = fa[top[u]];
            } else {
                r2v = sgt.merge(sgt.query(in[top[v]], in[v] + 1), r2v);
                v = fa[top[v]];
            }
        }
        if (dep[u] > dep[v]) r2u = sgt.merge(sgt.query(in[v], in[u] + 1), r2u);
        else r2v = sgt.merge(sgt.query(in[u], in[v] + 1), r2v);
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
    DirectedInfo(LinearFunc f) : f(f) {}
    DirectedInfo operator*(DirectedInfo const &o) const {
        auto &[k1, b1] = f;
        auto &[k2, b2] = o.f;
        // f1 * f2 != f2 * f1
        // (f1 * f2)(.) = f2(f1(.)) = k2 (k1 x + b1) + b2
        return {LinearFunc{k1 * k2, b1 * k2 + b2}};
    }
};

struct Tag {...};
struct Info { DirectedInfo forw, backw; };

using Merge = decltype([](Info const &x, Info const &y) {
    // 正向信息正向拼接，反向信息反向拼接
    return Info{x.forw * y.forw, y.backw * x.backw};
});
