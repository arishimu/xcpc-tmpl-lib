void gauss(FMat& A, FMat& B) {

    int n = A.n, m = A.m;

    auto find_pivot = [&](int r, int c) -> std::optional<int> {
        for (int i = r; i < n; i++) if (std::abs(A[i][c]) > eps) return i;
        return std::nullopt;
    };

    auto normalize = [&](int r, int c) {
        for (int j = c + 1; j < m; j++) A[r][j] /= A[r][c];
        for (int j = 0; j < B[0].size(); j++) B[r][j] /= A[r][c];
        A[r][c] = 1;
    };

    auto eliminate = [&](int r, int c) {
        for (int i = 0; i < n; i++) {
            if (i == r) continue;
            for (int j = c + 1; j < m; j++) A[i][j] -= A[i][c] * A[r][j];
            for (int j = 0; j < B[0].size(); j++) B[i][j] -= A[i][c] * B[r][j];
            A[i][c] = 0;
        }
    };

    int r = 0, c = 0;
    while (r < n && c < m) {
        auto p = find_pivot(r, c);
        if (!p) { c++; continue; }
        std::swap(A[p.value()], A[r]);
        std::swap(B[p.value()], B[r]);
        normalize(r, c);
        eliminate(r, c);
        r++, c++;
    }

}

auto solve(FMat A, FMat b) -> std::optional<std::pair<FMat, FMat>> {
    gauss(A, b);
    int n = A.n, m = A.m, r = 0, c = 0;
    std::vector<int> pivot, is_pivot(m);
    while (r < n && c < m) {
        if (A[r][c]) {
            pivot.push_back(c);
            is_pivot[c] = true;
            r++;
        }
        c++;
    }

    for (int i = r; i < n; i++)
        if (std::abs(b[i][0]) > eps)
            return std::nullopt;

    // dim V = rank A + dim null A
    //     m =      r +    (m - r)
    // [ ]   [ ]     [  ]     [  ]             [    ]
    // [x] = [p] + t1[b1] + t2[b2] + ... + tm-r[bm-r]
    // [ ]   [ ]     [  ]     [  ]             [    ]

    FMat particular(m, 1), nullspace_basis(m, m - r);
    for (int i = 0; i < r; i++) particular[pivot[i]][0] = b[i][0];
    for (int j = 0, cnt = 0; j < m; j++) {
        if (is_pivot[j]) continue;
        for (int i = 0; i < r; i++)
            nullspace_basis[pivot[i]][cnt] = -A[i][j];
        nullspace_basis[j][cnt++] = 1;
    }
    return {{particular, nullspace_basis}};
}

void test() {
    int n;
    std::cin >> n;
    FMat A(n, n), b(n, 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) std::cin >> A[i][j];
        std::cin >> b[i][0];
    }
    auto solution = solve(A, b);
    if (!solution) return std::cout << -1 << '\n', void(); // no solution
    auto [p, B] = solution.value();
    if (!B[0].empty()) return std::cout << 0 << '\n', void(); // many solutions
    for (int i = 0; i < p.n; i++)
        std::cout << 'x' << i + 1 << '=' << p[i][0] << '\n'; // unique solution
}