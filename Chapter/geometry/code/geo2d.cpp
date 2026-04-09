#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using Real = long double;

constexpr Real EPS = 1e-8;
constexpr Real  PI = 3.1415926535897932384626L;

bool lt(Real x, Real y)                                            { return x < y - EPS; }
bool le(Real x, Real y)                                           { return x <= y + EPS; }
bool gt(Real x, Real y)                                               { return lt(y, x); }
bool ge(Real x, Real y)                                               { return le(y, x); }
bool eq(Real x, Real y)                                   { return le(x, y) && ge(x, y); }
int sgn(Real x)                                            { return gt(x, 0) - lt(x, 0); }

struct Vec { Real x, y; };

Vec  operator-(Vec v)                                             { return {-v.x, -v.y}; }
Vec  operator+(Vec u, Vec v)                            { return {u.x + v.x, u.y + v.y}; }
Vec  operator-(Vec u, Vec v)                            { return {u.x - v.x, u.y - v.y}; }
Vec  operator*(Vec v, Real k)                               { return {v.x * k, v.y * k}; }
Vec  operator*(Real k, Vec v)                               { return {k * v.x, k * v.y}; }
Vec  operator/(Vec v, Real k)                               { return {v.x / k, v.y / k}; }

Real operator*(Vec u, Vec v)                             { return u.x * v.x + u.y * v.y; }
Real operator^(Vec u, Vec v)                             { return u.x * v.y - u.y * v.x; }

std::istream& operator>>(std::istream& is, Vec& v)            { return is >> v.x >> v.y; }
std::ostream& operator<<(std::ostream& os, Vec  v) { return os<<'('<<v.x<<','<<v.y<<')'; }

Real len(Vec v)                                              { return std::sqrtl(v * v); }
Vec  norm(Vec v)                           { Real r = len(v); return {v.x / r, v.y / r}; }

Vec  rotl(Vec v)                                                   { return {-v.y, v.x}; }
Vec  rotr(Vec v)                                                   { return {v.y, -v.x}; }
Vec  rot(Real th)                               { return {std::cosl(th), std::sinl(th)}; }
Vec  rot(Real r, Real th)               { return {r * std::cosl(th), r * std::sinl(th)}; }
Vec  rot(Vec v, Real th)           { return {v.x * std::cosl(th) - v.y * std::sinl(th),
                                             v.x * std::sinl(th) + v.y * std::cosl(th)}; }
Real angle(Vec u, Vec v)                 { return std::fabsl(std::atan2l(u ^ v, u * v)); }
Real angle1(Vec u, Vec v)                            { return std::atan2l(u ^ v, u * v); }
Real angle2(Vec u, Vec v)    { Real th = angle1(u, v); return th < 0 ? th + PI / 2 : th; }

using Dot = Vec;

int  sgn(Dot p)                      { return (p.y > 0 || p.y == 0 && p.x > 0) ? 1 : -1; }
Real dist(Dot a, Dot b)                                             { return len(a - b); }
Real orient(Dot o, Dot a, Dot p)                             { return (a - o) ^ (p - o); }

Dot  rotl(Dot p, Dot o)                                        { return o + rotl(p - o); }
Dot  rotr(Dot p, Dot o)                                        { return o + rotr(p - o); }
Dot  rot(Dot p, Dot o, Real th)                             { return o + rot(p - o, th); }

bool is_same(Dot a, Dot b)                        { return eq(a.x, b.x) && eq(a.y, b.y); }
bool is_collinear(Dot o, Dot a, Dot b)                  { return eq(orient(o, a, b), 0); }
bool in_angle(Dot o, Dot a, Dot b, Dot p)    { if (orient(o, a, b) < 0) std::swap(a, b);
                                return ge(orient(o, a, p), 0) && le(orient(o, b, p), 0); }

struct lex_order {
    bool y_prior = false;
    bool operator()(Dot a, Dot b) {
        if (y_prior) return a.y == b.y ? a.x < b.x : a.y < b.y;
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    }
};

struct direc_order {
    Vec v = {1, 0};
    bool operator()(Dot a, Dot b) {
        return a * v < b * v;
    }
};

struct polar_order {
    Dot o = Dot{0, 0};
    bool operator()(Dot a, Dot b) {
        a = a - o, b = b - o;
        return std::tuple{sgn(a),     0, a * a} <
               std::tuple{sgn(b), a ^ b, b * b};
    }
};

struct Line { Vec v; Real c; };
struct Segm { Dot a, b; };

Line line(Dot a, Dot b)                              { Vec v = b - a; return {v, v ^ a}; }
Line line_pv(Dot p, Vec v)                                          { return {v, v ^ p}; }
Line line_th(Dot p, Real th)                               { return line_pv(p, rot(th)); }
Line line_abc(Real a, Real b, Real c)                     { return {rotr(Vec{a, b}), c}; }
Line line_seg(Segm s)                                           { return line(s.a, s.b); }

Line shift(Line l, Vec v)                               { return {l.v, l.c + (l.v ^ v)}; }
Line para(Dot p, Line l)                                       { return line_pv(p, l.v); }
Line perp(Dot p, Line l)                                 { return line_pv(p, rotl(l.v)); }

Real side(Dot p, Line l)                                       { return (l.v ^ p) - l.c; }
Real dist(Dot p, Line l)                     { return std::fabsl(side(p, l)) / len(l.v); }
Real dist1(Dot p, Line l)                                { return side(p, l) / len(l.v); }

Dot  proj(Dot p, Line l)              { return p - rotl(l.v) * side(p, l) / (l.v * l.v); }
Dot  refl(Dot p, Line l)          { return p - rotl(l.v) * 2 * side(p, l) / (l.v * l.v); }
Dot  itsc(Line m, Line n)                { return (m.c * n.v - n.c * m.v) / (m.v ^ n.v); }

std::ostream& operator<<(std::ostream &os, Line l)
                    { return os << '[' << proj(Dot{0, 0}, l) << ',' << norm(l.v) << ']'; }
std::ostream& operator<<(std::ostream &os, Segm s) { return os<<'['<<s.a<<','<<s.b<<']'; }

bool on_line(Dot p, Line l)                                  { return eq(side(p, l), 0); }
bool is_same(Line m, Line n)                  { return eq(m.v ^ n.v, 0) && eq(m.c, n.c); }
bool is_para(Line m, Line n)                 { return eq(m.v ^ n.v, 0) && !eq(m.c, n.c); }
bool is_perp(Line m, Line n)                                  { return eq(m.v * n.v, 0); }
bool is_itsc(Line m, Line n)                                 { return !eq(m.v ^ n.v, 0); }

bool in_disk(Dot p, Segm s)                       { return le((s.a - p) * (s.b - p), 0); }
bool on_segm(Dot p, Segm s)         { return is_collinear(p, s.a, s.b) && in_disk(p, s); }

auto itsc(Segm m, Segm n) {
    std::vector<Dot> ans;
    Real oa = orient(n.a, n.b, m.a), ob = orient(n.a, n.b, m.b);
    Real oc = orient(m.a, m.b, n.a), od = orient(m.a, m.b, n.b);
    if (oa * ob < 0 && oc * od < 0) {
        ans.push_back((m.a * ob - m.b * oa) / (ob - oa));
        return ans;
    }
    if (on_segm(m.a, n)) ans.push_back(m.a);
    if (on_segm(m.b, n)) ans.push_back(m.b);
    if (on_segm(n.a, m)) ans.push_back(n.a);
    if (on_segm(n.b, m)) ans.push_back(n.b);
    return ans;
}

bool is_perfect_itsc(Segm m, Segm n) {
    Real oa = orient(n.a, n.b, m.a), ob = orient(n.a, n.b, m.b);
    Real oc = orient(m.a, m.b, n.a), od = orient(m.a, m.b, n.b);
    return oa * ob < 0 && oc * od < 0;
}

Real dist(Dot p, Segm s) {
    if (is_same(s.a, s.b)) return len(s.a - p);
    Line l = line_seg(s);
    direc_order cmp{l.v};
    if (cmp(s.a, p) && cmp(p, s.b)) return dist(p, l);
    return std::min(len(s.a - p), len(s.b - p));
}

Real dist(Segm m, Segm n) {
    if (is_perfect_itsc(m, n)) return 0;
    return std::min({
        dist(m.a, n), dist(m.b, n),
        dist(n.a, m), dist(n.b, m)
    });
}

struct Circ { Dot o; Real r; };

Circ circ(Dot a, Dot b, Dot c) {
    b = b - a, c = c - a;
    Dot o = a + itsc(line_pv(b / 2, rotl(b)), line_pv(c / 2, rotl(c)));
    return {o, len(a - o)};
}

Dot  circ_at(Dot o, Real r, Real th)                            { return o + rot(r, th); }
Dot  circ_at(Circ O, Real th)                            { return circ_at(O.o, O.r, th); }

auto tangent(Dot o, Real r, Dot p) {
    Vec po = o - p;
    Real d = len(po);
    std::vector<Line> ans;
    if (lt(d, r)) return ans;
    if (eq(d, r)) return ans.push_back(line_pv(p, rotr(po))), ans;
    Real th = std::asinl(r / d);
    ans.push_back(line_pv(p, rot(po, th)));
    ans.push_back(line_pv(p, rot(po, -th)));
    return ans;
}

auto itsc(Dot o, Real r, Line l) {
    std::vector<Dot> ans;
    Real d = dist(o, l);
    Real sqh = r * r - d * d;
    if (lt(sqh, 0)) return ans;
    Dot m = proj(o, l);
    if (eq(sqh, 0)) return ans.push_back(m), ans;
    Vec h = norm(l.v) * std::sqrtl(sqh);
    ans.push_back(m + h), ans.push_back(m - h);
    return ans;
}

auto itsc(Dot o1, Real r1, Dot o2, Real r2) {
    std::vector<Dot> ans;
    Vec o1o2 = o2 - o1;
    Real D = len(o1o2);

    if (gt(D, r1 + r2)) return ans;
    if (lt(D, std::fabsl(r1 - r2))) return ans;
    if (eq(D, r1 + r2) || eq(D, std::fabsl(r1 - r2))) {
        ans.push_back(o1 + norm(o1o2) * r1);
        return ans;
    }

    Real d = (D * D + r1 * r1 - r2 * r2) / (2 * D);
    Real h = std::sqrtl(r1 * r1 - d * d);
    Dot m = o1 + norm(o1o2) * d;
    Vec v = norm(rotl(o1o2)) * h;
    ans.push_back(m + v);
    ans.push_back(m - v);
    return ans;
}

auto tangent(Circ O, Dot p)                               { return tangent(O.o, O.r, p); }
auto itsc(Circ O, Line l)                                    { return itsc(O.o, O.r, l); }
auto itsc(Circ O1, Circ O2)                       { return itsc(O1.o, O1.r, O2.o, O2.r); }

using Polygon = std::vector<Dot>;
using    Poly = Polygon const &;

Real area1(Poly poly) {
    Real ans = 0;
    int n = poly.size();
    for (int i = 0; i < n - 1; i++) ans += poly[i] ^ poly[i + 1];
    ans += poly[n - 1] ^ poly[0];
    return ans / 2.0;
}

Real area(Poly poly) {
    return std::fabsl(area1(poly));
}

bool on_poly(Dot o, Poly poly) {
    for (int i = 0, j = 1; i < poly.size(); i++, j++) {
        if (j >= poly.size()) j -= poly.size();
        if (on_segm(o, Segm{poly[i], poly[j]})) return true;
    }
    return false;
}

bool in_poly(Dot o, Poly poly, bool strict) {
    if (on_poly(o, poly)) return !strict;
    auto cross_test = [o] (Dot a, Dot b) {
        if (orient(o, a, b) > 0) return a.x <= o.x && o.x < b.x;
        return b.x <= o.x && o.x < a.x;
    };
    bool flag = false;
    for (int i = 0, j = 1; i < poly.size(); i++, j++) {
        if (j >= poly.size()) j -= poly.size();
        flag ^= cross_test(poly[i], poly[j]);
    }
    return flag;
}

bool in_convex(Dot o, Poly poly, bool strict) {
    int n = poly.size();
    if (lt(orient(poly[0], poly[1], o), 0)) return false;
    if (lt(orient(poly[n-1], poly[0], o), 0)) return false;
    if (on_segm(o, Segm{poly[0], poly[1]})) return !strict;
    if (on_segm(o, Segm{poly[n-1], poly[0]})) return !strict;
    int l = 1, r = n - 2;
    while (l < r) {
        int m = (l + r + 1) / 2;
        if (gt(orient(poly[0], poly[m], o), 0)) l = m;
        else r = m - 1;
    }
    if (on_segm(o, Segm{poly[l], poly[l+1]})) return !strict;
    return gt(orient(poly[l], poly[l+1], o), 0);
}

auto hull(std::vector<Dot> dots) {
    if (dots.size() <= 1) return dots;
    std::vector<Dot> lo, hi;
    std::sort(dots.begin(), dots.end(), lex_order{});
    for (Dot p : dots) {
        while (lo.size() > 1 && ge((p - lo.end()[-1]) ^ (p - lo.end()[-2]), 0))
            lo.pop_back();
        while (hi.size() > 1 && le((p - hi.end()[-1]) ^ (p - hi.end()[-2]), 0))
            hi.pop_back();
        lo.push_back(p);
        hi.push_back(p);
    }
    std::reverse(hi.begin(), hi.end());
    lo.pop_back(), hi.pop_back();
    lo.insert(lo.end(), hi.begin(), hi.end());
    return lo;
}

struct closest_pair_answer { Real sqd; Dot u, v; };
auto closest_pair(std::vector<Dot> dots) {

    constexpr Real INF = INT64_MAX;
    using answer = closest_pair_answer;

    auto solve = [&](auto &f, int l, int r) -> answer {

        if (l + 1 == r) return {INF, {INF, INF}, {INF, INF}};

        int m = (l + r) / 2;
        Real midx = dots[m].x;
        auto ansl = f(f, l, m);
        auto ansr = f(f, m, r);

        Real sqd; Dot u, v;
        if (ansl.sqd <= ansr.sqd) sqd = ansl.sqd, u = ansl.u, v = ansl.v;
        else sqd = ansr.sqd, u = ansr.u, v = ansr.v;
        std::inplace_merge(dots.begin() + l, dots.begin() + m,
                           dots.begin() + r, lex_order{true});

        std::vector<int> cand;
        cand.reserve(r - l);
        for (int i = l; i < r; i++) {
            Real dx = dots[i].x - midx;
            if (dx * dx < sqd) cand.push_back(i);
        }

        for (int i = 0; i < cand.size(); i++) {
            for (int j = i + 1; j < cand.size(); j++) {
                Real dy = dots[cand[j]].y - dots[cand[i]].y;
                if (dy * dy >= sqd) break;
                Real dx = dots[cand[j]].x - dots[cand[i]].x;
                Real ds = dx * dx + dy * dy;
                if (ds < sqd) sqd = ds, u = dots[cand[i]], v = dots[cand[j]];
            }
        }

        return {sqd, u, v};
    };

    std::sort(dots.begin(), dots.end(), lex_order{});
    return solve(solve, 0, dots.size());

}