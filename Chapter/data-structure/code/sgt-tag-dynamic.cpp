template <class Info, class Tag>
struct SGT {

    i64 mini, maxi;
    std::vector<int> lson, rson;
    std::vector<Info> info;
    std::vector<Tag> tag;

    SGT(i64 mini, i64 maxi)
        : mini(mini), maxi(maxi), lson(2), rson(2), info(2), tag(2) {}

    int node() {
        lson.emplace_back(); rson.emplace_back();
        info.emplace_back(); tag.emplace_back();
        return info.size() - 1;
    }

    void apply(int p, Tag const &t) { info[p] += t; tag[p] += t; }
    void pull(int p) { info[p] = info[lson[p]] + info[rson[p]]; }
    void push(int p) {
        if (!lson[p]) lson[p] = node();
        if (!rson[p]) rson[p] = node();
        apply(lson[p], tag[p]);
        apply(rson[p], tag[p]);
        tag[p] = {};
    }

    void modify(i64 i, Info const &v) { modify(1, mini, maxi, i, v); }
    void modify(int p, i64 l, i64 r, i64 i, Info const &v) {
        if (l + 1 == r) return info[p] = v, void();
        i64 m = l + (r - l) / 2;
        push(p);
        if (i < m) modify(lson[p], l, m, i, v);
        else modify(rson[p], m, r, i, v);
        pull(p);
    }

    void apply(i64 L, i64 R, Tag const &t) { apply(1, mini, maxi, L, R, t); }
    void apply(int p, i64 l, i64 r, i64 L, i64 R, Tag const &t) {
        if (r <= L || R <= l) return;
        if (L <= l && r <= R) return apply(p, t);
        i64 m = l + (r - l) / 2;
        push(p);
        apply(lson[p], l, m, L, R, t);
        apply(rson[p], m, r, L, R, t);
        pull(p);
    }

    Info query(i64 i) { return query(i, i + 1); }
    Info query(i64 L, i64 R) { return query(1, mini, maxi, L, R); }
    Info query(int p, i64 l, i64 r, i64 L, i64 R) {
        if (R <= l || r <= L) return {};
        if (L <= l && r <= R) return info[p];
        i64 m = l + (r - l) / 2;
        push(p);
        return query(lson[p], l, m, L, R) + query(rson[p], m, r, L, R);
    }

};

void test() {
    int n, q, m;
    std::cin >> n >> q >> m;
    Z::p = m;

    struct Tag {...};
    struct Info {...};
    SGT<Info, Tag> sgt(0, n);
    for (int i = 0; i < n; i++) {
        i64 x;
        std::cin >> x;
        sgt.modify(i, {x, i, i + 1});
    }

    while (q--) {...}
}