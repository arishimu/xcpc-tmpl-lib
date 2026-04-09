template <class T>
auto floyd(std::vector<std::vector<T>> const &w) {
    auto d = w; int n = w.size();
    for (int k = 0; k < n; k++) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                d[u][v] = std::min(d[u][v], d[u][k] + d[k][v]);
            }
        }
    }
    return d;
}

void test() {
    int n, m;
    std::cin >> n >> m;

    auto w = std::vector(n, std::vector(n, INT64_MAX / 2));
    for (int u = 0; u < n; u++) w[u][u] = 0;
    for (int i = 0; i < m; i++) {
        int u, v; i64 f;
        std::cin >> u >> v >> f; u--, v--;
        w[u][v] = std::min(w[u][v], f);
        w[v][u] = std::min(w[v][u], f);
    }

    auto d = floyd(w);
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            std::cout << d[u][v] << " \n"[v == n - 1];
        }
    }
}