//
// Created by duck on 2024/4/22.
//
#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
constexpr ll INF = 1 << 30;
constexpr int MAX_N = 101;

struct edge {
    ll u, v, c, f;
    edge(ll u, ll v, ll c, ll f) : u(u), v(v), c(c), f(f) {}
};

class EdmondsKarp {
private:
    vector<edge> edges;
    vector<vector<ll>> G;
    vector<ll> parent;
    ll N, M;
    ll s, t;
public:
    EdmondsKarp(const ll N_, const ll s, const ll t) : N(N_), M(0), s(s), t(t) {
        G.resize(N);
        parent.resize(N);
    }

    void add_edge(ll u, ll v, ll c) {
        edges.emplace_back(u, v, c, 0);
        edges.emplace_back(v, u, c, 0);
        G[u].emplace_back(M++);
        G[v].emplace_back(M++);
    }

    bool bfs() {
        fill(parent.begin(), parent.end(), -1);
        queue<ll> q;
        q.emplace(s);
        while(!q.empty()) {
            ll u = q.front(); q.pop();
            for(auto &i : G[u]) {
                edge &e = edges[i];
                if(e.c - e.f > 0 && parent[e.v] == -1 && e.v != s) {
                    parent[e.v] = i;
                    if(e.v == t) return true;
                    q.emplace(e.v);
                }
            }
        }
        return false;
    }

    ll max_flow() {
        ll flow = 0;
        while(bfs()) {
            ll f = INF;
            for(ll i = parent[t];i != -1;i = parent[edges[i].u]) {
                f = min(f, edges[i].c - edges[i].f);
            }
            for(ll i = parent[t];i != -1;i = parent[edges[i].u]) {
                edges[i].f += f;
                edges[i ^ 1].f -= f;
            }
            flow += f;
        }
        return flow;
    }

    vector<edge> min_cut() {
        vector<bool> vis(N, false);
        queue<ll> q;
        q.emplace(s);
        vis[s] = true;
        while(!q.empty()) {
            ll u = q.front(); q.pop();
            for(auto &i : G[u]) {
                edge &e = edges[i];
                if(e.c - e.f > 0 && !vis[e.v]) {
                    vis[e.v] = true;
                    q.emplace(e.v);
                }
            }
        }
        vector<edge> res;
        for(auto &e: edges) {
            if(vis[e.u] && !vis[e.v] && e.c > 0) {
                res.emplace_back(e);
            }
        }
        return res;
    }
};

void solve() {
    ll N, M; cin >> N >> M;

    ll s, t; cin >> s >> t;
    EdmondsKarp ek(N, s, t);

    for(int i = 0;i < M;i++) {
        ll u, v, c; cin >> u >> v >> c;
        ek.add_edge(u, v, c);
    }
    cout << ek.max_flow() << '\n';

    auto cut = ek.min_cut();
    for(auto &e: cut) {
        cout << e.u << ' ' << e.v << '\n';
    }
}

int main() {_
    solve();
    return 0;
}

//4 4
//0 3
//0 1 2
//0 2 1
//1 3 2
//2 3 2