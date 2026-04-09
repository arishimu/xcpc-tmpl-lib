#include <cstdint>
#include <iostream>

using i64 = int64_t;

void test() {
    std::cout << "Hello World" << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T = 1;
    std::cin >> T;
    while (T--) test();
}