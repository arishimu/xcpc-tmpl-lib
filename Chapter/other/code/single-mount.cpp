template <class Int, class J>
Int find_first_if(Int L, Int R, J judge) {
    while (R - L > 1) {
        Int M = (L + R) / 2;
        if (judge(M - 1)) R = M;
        else L = M;
    }
    return judge(L) ? L : L + 1;
}

template <class Int, class J>
Int find_last_if(Int L, Int R, J judge) {
    while (R - L > 1) {
        Int M = (L + R) / 2;
        if (judge(M)) L = M;
        else R = M;
    }
    return judge(L) ? L : L - 1;
}

int main() {
    // index:  0, 1, 2, 3, 4, 5, 6, 7
    int a[] = {2, 2, 2, 2, 2, 2, 2, 2};
    int b[] = {2, 4, 5, 7, 7, 7, 3, 1};
    int L, R;
    L = find_last_if(1, 8, [&](int i) { return a[i - 1] < a[i]; });
    R = find_first_if(1, 8, [&](int i) { return a[i - 1] > a[i]; });
    assert(L == 0 && R == 8);
    L = find_last_if(1, 8, [&](int i) { return b[i - 1] < b[i]; });
    R = find_first_if(1, 8, [&](int i) { return b[i - 1] > b[i]; });
    assert(L == 3 && R == 6);
    std::cout << "ok" << '\n';
}