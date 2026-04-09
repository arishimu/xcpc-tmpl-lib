int d = 0;
auto dfs = [&] (auto &f, int u, int fa) -> void {
    for (int v : adj[u]) {
        if (v == fa) continue;
        f(f, v, u);
        d = std::max(d, dp[u] + dp[v] + 1); // w(u, v) = 1
        dp[u] = std::max(dp[u], dp[v] + 1); // w(u, v) = 1
    }
};