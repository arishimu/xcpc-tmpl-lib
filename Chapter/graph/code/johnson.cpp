template <class T>
struct Graph {

    int n;
    std::vector<T> dist, h;
    std::vector<int> len, inq;
    std::vector<std::vector<int>> adj;
    static constexpr T INF = std::numeric_limits<T>::max() / 2;

    struct Edge { int u, v; T w; };
    std::vector<Edge> e;

    Graph(int n) : n(n), adj(n) {}
    void add_edge(int u, int v, T w) {
        adj[u].push_back(e.size());
        e.push_back({u, v, w});
    }

    bool spfa() {
        std::queue<int> q;
        len.assign(n, 0), inq.assign(n, true), h.assign(n, 0);
        for (int u = 0; u < n; u++) q.push(u);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = false;
            for (int i : adj[u]) {
                auto [_, v, w] = e[i];
                if (h[u] + w < h[v]) {
                    h[v] = h[u] + w;
                    len[v] = len[u] + 1;
                    if (len[v] > n) return false;
                    if (!inq[v]) inq[v] = true, q.push(v);
                }
            }
        }
        return true;
    }

    void dijkstra(int s) {
        using state = std::pair<T, int>;
        std::priority_queue<state, std::vector<state>, std::greater<>> q;
        dist.assign(n, INF), dist[s] = 0, q.emplace(0, s);
        while (!q.empty()) {
            auto [d, u] = q.top(); q.pop();
            if (d > dist[u]) continue;
            for (int i : adj[u]) {
                auto [_, v, w] = e[i];
                if (d + w < dist[v]) {
                    dist[v] = d + w;
                    q.emplace(dist[v], v);
                }
            }
        }
    }

    auto johnson() {
        std::vector<std::vector<T>> ans;
        if (!spfa()) return ans;
        for (auto &[u, v, w] : e) w += h[u] - h[v];
        for (int u = 0; u < n; u++) {
            dijkstra(u);
            ans.push_back(dist);
            for (int v = 0; v < n; v++) {
                if (dist[v] != INF) {
                    ans[u][v] -= h[u] - h[v];
                }
            }
        }
        return ans;
    }

};

void test() {
    int n, m;
    std::cin >> n >> m;
    Graph<i64> g(n);
    for (int i = 0; i < m; i++) {
        int u, v; i64 w;
        std::cin >> u >> v >> w; u--, v--;
        g.add_edge(u, v, w);
    }
    auto dist = g.johnson();
    if (dist.empty()) { std::cout << -1 << '\n'; return; }
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (dist[u][v] == Graph<i64>::INF) {
                std::cout << u << ' ' << v << '\n';
            }
        }
    }
}