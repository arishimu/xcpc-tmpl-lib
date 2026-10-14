#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

using i64 = int64_t;

template <class Info, class Tag,
    class Merge, class ApplyToInfo, class ApplyToTag,
    class Init = decltype([](i64, i64){ return Info{}; }),
    class ReadyToApply = decltype([](Tag const &, Info const &){ return true; })>
struct SegTree {

    i64 L, R;
    std::vector<int> lson, rson;
    std::vector<Info> info;
    std::vector<Tag> tag;

    Tag id;
    std::optional<Init> init;
    Merge merge;
    ApplyToInfo info_apply;
    ApplyToTag tag_apply;
    ReadyToApply apply_ready;

    auto node(i64 l, i64 r) -> int {
        lson.push_back(0), rson.push_back(0);
        info.push_back((*init)(l, r)), tag.push_back(id);
        return info.size() - 1;
    }

    void dive(int p, i64 l, i64 m, i64 r) {
        if (!lson[p]) lson[p] = node(l, m);
        if (!rson[p]) rson[p] = node(m, r);
    }

    void apply(int p, Tag const &t) {
        info_apply(t, info[p]);
        tag_apply(t, tag[p]);
    }

    void push(int p, i64 l, i64 m, i64 r) {
        dive(p, l, m, r);
        apply(lson[p], tag[p]);
        apply(rson[p], tag[p]);
        tag[p] = id;
    }

    void build(i64 L, i64 R, Init init = {}, Tag id = {}) {
        this->L = L, this->R = R;
        this->init.emplace(init), this->id = id;
        lson.resize(1), rson.resize(1), info.resize(1), tag.resize(1);
        node(L, R);
    }

    void modify(i64 i, Tag const &t) { modify(i, i + 1, t); }
    void modify(i64 a, i64 b, Tag const &t) { modify(1, L, R, a, b, t); }
    void modify(int p, i64 l, i64 r, i64 a, i64 b, Tag const &t) {
        if (r <= a || b <= l) return;
        if (a <= l && r <= b && apply_ready(t, info[p])) return apply(p, t);
        i64 m = l + (r - l) / 2;
        push(p, l, m, r);
        modify(lson[p], l, m, a, b, t);
        modify(rson[p], m, r, a, b, t);
        info[p] = merge(info[lson[p]], info[rson[p]]);
    }

    Info query(i64 i) { return query(i, i + 1); }
    Info query(i64 a, i64 b) { return query(1, L, R, a, b); }
    Info query(int p, i64 l, i64 r, i64 a, i64 b) {
        if (a <= l && r <= b) return info[p];
        i64 m = l + (r - l) / 2;
        push(p, l, m, r);
        if (b <= m) return query(lson[p], l, m, a, b);
        if (m <= a) return query(rson[p], m, r, a, b);
        return merge(query(lson[p], l, m, a, b), query(rson[p], m, r, a, b));
    }

};

void solve() {

    int n, q;
    std::cin >> n >> q;
    std::vector<i64> a(n), p(n + 1);
    for (auto &x : a) std::cin >> x;
    std::partial_sum(a.begin(), a.end(), p.begin() + 1);

    struct Tag {
        i64 mul = 1, add = 0;
    };

    struct Info {
        i64 sum = 0, len = 0;
    };

    auto init = [&](i64 l, i64 r){ return Info{p[r] - p[l], r - l}; };
    SegTree<Info, Tag, decltype([](Info const &x, Info const &y) {
        return Info{x.sum + y.sum, x.len + y.len};
    }), decltype([](Tag const &op, Info &targ) {
        targ.sum = targ.sum * op.mul + op.add * targ.len;
    }), decltype([](Tag const &op, Tag &targ) {
        targ.mul = op.mul * targ.mul;
        targ.add = op.mul * targ.add + op.add;
    }), decltype(init)> sgt;

    sgt.build(0, n, init);

    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        if (op == 3) {
            std::cout << sgt.query(l, r).sum << '\n';
        } else {
            i64 b;
            std::cin >> b;
            Tag t;
            if (op == 0) t.mul = b;
            else if (op == 1) t.add = b;
            else if (op == 2) t.mul = 0, t.add = b;
            sgt.modify(l, r, t);
        }
    }

}
