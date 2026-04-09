#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

template <class Key, int width, class Comp = std::less<Key>>
struct xor_basis {

    std::array<std::bitset<width>, width> basis;
    std::array<std::vector<Key>, width> keys;

    xor_basis() : basis{}, keys{} {}

    void insert(Key k, std::bitset<width> v) {
        std::vector<Key> S, temp;
        for (int i = width - 1; i >= 0; i--) {
            if (!v.test(i)) continue;
            if (basis[i].test(i)) {
                v ^= basis[i];
                std::set_symmetric_difference(
                    S.begin(), S.end(), keys[i].begin(), keys[i].end(),
                    std::back_inserter(temp), Comp{});
                S = std::move(temp);
            } else {
                basis[i] = std::move(v);
                keys[i] = std::move(S);
                return;
            }
        }
    }

    auto get_basis(std::bitset<width> v) -> std::optional<std::bitset<width>> {
        std::bitset<width> result;
        for (int i = width - 1; i >= 0; i--) {
            if (!v.test(i)) continue;
            if (basis[i].none()) return std::nullopt;
            v ^= basis[i];
            result.set(i, true);
        }
        return result;
    }

    auto get_keys(std::bitset<width> v) -> std::optional<std::vector<Key>> {
        std::vector<Key> result, temp;
        auto coord = get_basis(v);
        if (!coord) return std::nullopt;
        for (int i = width - 1; i >= 0; i--) {
            if (!coord.value().test(i)) continue;
            std::set_symmetric_difference(
                result.begin(), result.end(), keys[i].begin(), keys[i].end(),
                std::back_inserter(temp), Comp{});
            result = std::move(temp);
        }
        return result;
    }

};

void test() {
    int n;
    std::cin >> n;
    xor_basis<int, 50> B;
    for (int i = 0; i < n; i++) {
        u64 x;
        std::cin >> x;
        B.insert(i, x);
    }

    // subset of max-xor
    std::bitset<50> ans = 0;
    for (int i = 49; i >= 0; i--) {
        if (B.basis[i].none()) continue;
        if (B.basis[i][i] ^ ans[i]) ans ^= B.basis[i];
    }
    std::cout << ans.to_ullong() << '\n';
}