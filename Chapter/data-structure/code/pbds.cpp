#include <cassert>
#include <cstdint>
#include <functional>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using i64 = int64_t;
using RBT = __gnu_pbds::tree<i64, __gnu_pbds::null_type, std::less<i64>,
                __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;

void test() {

    RBT tr;
    auto [it, ok] = tr.insert(114);
    tr.insert(514), tr.insert(19), tr.insert(19), tr.insert(810);

    assert(tr.empty() == false);
    assert(tr.size() == 5);

    // [19, 19, 114, 514, 810]
    assert(tr.erase(1919) == false);
    assert(*(it = tr.erase(it)) == 514);
    assert(it + 2 == tr.end());
    assert(tr.erase(tr.end()) == tr.end());

    // [19, 19, 514, 810]
    it = tr.find_by_order(2); assert(*it == 514);
    int pos = tr.order_of_key(486); assert(pos == 2);

    // tr[l, r) -> tr[0, 2) -> [19, 19]
    auto l = tr.lower_bound(19); assert(*l == 19);
    auto r = tr.upper_bound(19); assert(*r == 514);

    tr.insert(998), tr.insert(244), tr.insert(353);
    // [19, 19, 244, 353, 514, 810, 998]

    RBT tr2;
    tr.split(353, tr2);
    for (auto i = tr.begin(); i != tr.end(); i++);
    // tr: [19, 19, 244, 353], tr2: [514, 810, 998]
    tr.join(tr2);
    // tr: [19, 19, 244, 353, 514, 810, 998], tr2: []

    tr2.insert(400);
    // tr.join(tr2); // ERROR: range cross!

}