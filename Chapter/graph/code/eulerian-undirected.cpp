#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using i64 = int64_t;

struct Graph {

    int n;
    std::vector<int> deg;
    std::vector<std::vector<int>> adj;

    struct Edge { int u, v; };
    std::vector<Edge> e;

    Graph(int n) : n(n), deg(n), adj(n) {}
    void add_edge(int u, int v) {
        deg[u] += 1, deg[v] += 1;
        adj[u].push_back(e.size()); e.push_back({u, v});
        adj[v].push_back(e.size()); e.push_back({v, u});
    }

    void lexico() {
        for (auto &v : adj)
            std::sort(v.begin(), v.end(),
                [&](int i, int j) { return e[i].v < e[j].v; }); }

    auto eulerian() -> std::vector<int> {
        std::vector<int> trail;
        for (int u = 0, odd = 0; u < n; u++)
            if ((deg[u] & 1) && ++odd > 2) return trail;

        int s = -1;
        for (int u = 0; u < n; u++) if (deg[u] & 1) { s = u; break; }
        if (s == -1) for (int u = 0; u < n; u++) if (deg[u]) { s = u; break; }
        if (s == -1) return {};

        std::vector<int> cur(n), vis(e.size());
        auto dfs = [&](auto &self, int u) -> void {
            while (cur[u] < adj[u].size()) {
                int eid = adj[u][cur[u]++];
                if (vis[eid ^ 1]) continue;
                vis[eid] = true;
                auto [_, v] = e[eid];
                self(self, v);
                trail.push_back(eid);
            }
        };

        dfs(dfs, s);

        if (trail.size() < e.size() / 2) return {};
        std::reverse(trail.begin(), trail.end());
        return trail;
    }

};

void test() {
    int N, M;
    std::cin >> N >> M;
    Graph g(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        std::cin >> u >> v;
        g.add_edge(u, v);
    }

    g.lexico();
    auto trail = g.eulerian();
    if (trail.empty() && !g.e.empty()) { std::cout << "No" << '\n'; return; }

    std::cout << "Yes" << '\n';
    for (int eid : trail) { auto [u, v] = g.e[eid]; std::cout << u << ' '; }
    if (g.e.empty()) std::cout << 0 << '\n';
    else std::cout << g.e[trail.back()].v << '\n';
    for (int eid : trail) { eid >>= 1; std::cout << eid << ' '; }
    std::cout << '\n';
}