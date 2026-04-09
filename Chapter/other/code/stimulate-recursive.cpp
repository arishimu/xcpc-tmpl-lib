int hanoi(int n, char from, char via, char to) {

    struct Frame { int pc; int n; char from, via, to; int t; };
    std::stack<Frame> stk;
    int rax = 0;

    stk.push({0, n, from, via, to, 0});
    while (!stk.empty()) {
        auto &[pc, n, from, via, to, t] = stk.top();
        bool flag = false;

        if (pc == 0) {
            if (n == 1) {
                std::cout << from << "->" << to << '\n';
                rax = 1, stk.pop(), flag = true;
            }
        }

        else if (pc == 1) stk.push({0, n - 1, from, to, via, 0});
        else if (pc == 2) t += rax;
        else if (pc == 3) std::cout << from << "->" << to << '\n', t++;
        else if (pc == 4) stk.push({0, n - 1, via, from, to, 0});
        else if (pc == 5) rax = t + rax, stk.pop(), flag = true;

        if (!flag) pc++;
    }

    return rax;

}

// int hanoi(int n, char from, char via, char to) {
//     if (n == 1) return std::cout << from << "->" << to << '\n', 1;
//     int t = 0;
//     t += hanoi(n - 1, from, to, via);
//     std::cout << from << "->" << to << '\n'; t++;
//     t += hanoi(n - 1, via, from, to);
//     return t;
// }