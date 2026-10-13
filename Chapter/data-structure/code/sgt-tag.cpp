#include <iostream>
#include <vector>

using i64 = int64_t;

template <class Info, class Tag,
    class Merge, class ApplyToInfo, class ApplyToTag,
    class ReadyToApply = decltype([](Tag const &, Info const &){ return true; })>
struct SegTree {

    i64 L, R;
    std::vector<Info> info;
    std::vector<Tag> tag;

    Tag id;
    Merge merge;
    ApplyToInfo info_apply;
    ApplyToTag tag_apply;
    ReadyToApply apply_ready;

    template <class Init = decltype([](i64){ return Info{}; })>
    void build(i64 L, i64 R, Init init = {}, Tag id = {}) {
        auto pull = [&](auto &self, int p, i64 l, i64 r) -> void {
            if (r - l == 1) return info[p] = init(l), void();
            i64 m = l + (r - l) / 2;
            self(self, p * 2, l, m);
            self(self, p * 2 + 1, m, r);
            info[p] = merge(info[p * 2], info[p * 2 + 1]);
        };
        this->L = L, this->R = R, this->id = id;
        info.resize((R - L) * 4);
        tag.resize((R - L) * 4);
        pull(pull, 1, L, R);
    }

    void apply(int p, Tag const &t) {
        info_apply(t, info[p]);
        tag_apply(t, tag[p]);
    }

    void push(int p) {
        apply(p * 2, tag[p]);
        apply(p * 2 + 1, tag[p]);
        tag[p] = id;
    }

    void modify(i64 i, Tag const &t) { modify(i, i + 1, t); }
    void modify(i64 a, i64 b, Tag const &t) { modify(1, L, R, a, b, t); }
    void modify(int p, i64 l, i64 r, i64 a, i64 b, Tag const &t) {
        if (r <= a || b <= l) return;
        if (a <= l && r <= b && apply_ready(t, info[p])) return apply(p, t);
        i64 m = l + (r - l) / 2;
        push(p);
        modify(p * 2, l, m, a, b, t);
        modify(p * 2 + 1, m, r, a, b, t);
        info[p] = merge(info[p * 2], info[p * 2 + 1]);
    }

    Info query(i64 i) { return query(i, i + 1); }
    Info query(i64 a, i64 b) { return query(1, L, R, a, b); }
    Info query(int p, i64 l, i64 r, i64 a, i64 b) {
        if (a <= l && r <= b) return info[p];
        push(p);
        i64 m = l + (r - l) / 2;
        if (b <= m) return query(p * 2, l, m, a, b);
        if (m <= a) return query(p * 2 + 1, m, r, a, b);
        return merge(query(p * 2, l, m, a, b), query(p * 2 + 1, m, r, a, b));
    }

};

void solve() {

    int n, q;
    std::cin >> n >> q;
    std::vector<i64> a(n);
    for (auto &x : a) std::cin >> x;

    struct Tag {
        i64 add = 0;
        i64 chmax = INT64_MIN / 2;
        i64 chmin = INT64_MAX / 2;
    };

    struct Info {
        i64 len, sum;
        i64 max1, cntx, max2;
        i64 min1, cntn, min2;
    };

    SegTree<Info, Tag, decltype([](Info const &x, Info const &y) {
        Info z;
        z.len = x.len + y.len;
        z.sum = x.sum + y.sum;
        if (x.max1 > y.max1) {
            z.max1 = x.max1;
            z.cntx = x.cntx;
            z.max2 = std::max(y.max1, x.max2);
        } else if (x.max1 < y.max1) {
            z.max1 = y.max1;
            z.cntx = y.cntx;
            z.max2 = std::max(x.max1, y.max2);
        } else {
            z.max1 = x.max1;
            z.cntx = x.cntx + y.cntx;
            z.max2 = std::max(x.max2, y.max2);
        }
        if (x.min1 < y.min1) {
            z.min1 = x.min1;
            z.cntn = x.cntn;
            z.min2 = std::min(y.min1, x.min2);
        } else if (x.min1 > y.min1) {
            z.min1 = y.min1;
            z.cntn = y.cntn;
            z.min2 = std::min(x.min1, y.min2);
        } else {
            z.min1 = x.min1;
            z.cntn = x.cntn + y.cntn;
            z.min2 = std::min(x.min2, y.min2);
        }
        return z;
    }), decltype([](Tag const &op, Info &targ) {
        targ.sum += op.add * targ.len;
        targ.max1 += op.add, targ.max2 += op.add;
        targ.min1 += op.add, targ.min2 += op.add;
        if (op.chmax > targ.min1) {
            targ.sum += (op.chmax - targ.min1) * targ.cntn;
            targ.min1 = op.chmax;
            targ.max1 = std::max(targ.max1, op.chmax);
            if (targ.max1 > targ.min1)
                targ.max2 = std::max(targ.max2, op.chmax);
        }
        if (op.chmin < targ.max1) {
            targ.sum -= (targ.max1 - op.chmin) * targ.cntx;
            targ.max1 = op.chmin;
            targ.min1 = std::min(targ.min1, op.chmin);
            if (targ.min1 < targ.max1)
                targ.min2 = std::min(targ.min2, op.chmin);
        }
    }), decltype([](Tag const &op, Tag &targ) {
        targ.add = targ.add + op.add;
        targ.chmax = std::max(targ.chmax + op.add, op.chmax);
        targ.chmin = std::min(std::max(targ.chmin + op.add, op.chmax), op.chmin);
        targ.chmax = std::min(targ.chmax, targ.chmin);
    }), decltype([](Tag const &op, Info const &targ) {
        return op.chmax < targ.min2 + op.add
            && op.chmin > targ.max2 + op.add;
    })> sgt;

    sgt.build(0, n, [&](i64 i) {
        return Info {
            .len = 1, .sum = a[i],
            .max1 = a[i], .cntx = 1, .max2 = INT64_MIN / 2,
            .min1 = a[i], .cntn = 1, .min2 = INT64_MAX / 2,
        };
    });

    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        if (op == 3) {
            std::cout << sgt.query(l, r).sum << '\n';
        } else {
            i64 b;
            std::cin >> b;
            Tag t;
            if (op == 0) t.chmin = b;
            else if (op == 1) t.chmax = b;
            else if (op == 2) t.add = b;
            sgt.modify(l, r, t);
        }
    }

}
