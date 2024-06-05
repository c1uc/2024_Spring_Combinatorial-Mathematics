//
// Created by duck on 2024/4/17.
//
#include <bits/stdc++.h>
using namespace std;
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using ll = long long;
constexpr ll INF = 1 << 30;
constexpr int MAX_N = 501;

void solve() {
    int N; cin >> N;

    array<ll, MAX_N> S{0};
    array<ll, MAX_N> T{0};

    array<array<ll, MAX_N>, MAX_N> W{0};

    array<ll, MAX_N> lx{0};
    array<ll, MAX_N> ly{0};
    array<ll, MAX_N> l_match{-1};
    array<ll, MAX_N> r_match{-1};
    array<ll, MAX_N> slack{INF};
    array<ll, MAX_N> parent{0};

    fill(l_match.begin(), l_match.end(), -1);
    fill(r_match.begin(), r_match.end(), -1);

    queue<ll> q;

    for(int i = 0;i < N;i++) {
        for(int j = 0;j < N;j++) {
            cin >> W[i][j];
            W[i][j] *= -1;
            lx[i] = max(lx[i], W[i][j]);
        }
    }

    auto augment = [&](ll u) {
        while(u != -1) {
            r_match[u] = parent[u];
            swap(u, l_match[parent[u]]);
        }
    };

    auto check = [&](ll u) {
        T[u] = 1;
        if (r_match[u] != -1) {
            q.emplace(r_match[u]);
            S[r_match[u]] = 1;
            return false;
        }
        augment(u);
        return true;
    };

    auto update = [&]() {
        ll delta = INF;
        for(int i = 0;i < N;i++) if(!T[i]) delta = min(delta, slack[i]);

        for(int i = 0;i < N;i++) {
            if(S[i]) lx[i] -= delta;
            if(T[i]) ly[i] += delta;
            else slack[i] -= delta;
        }
    };

    auto bfs = [&](int root) {
        q.emplace(root);
        S[root] = 1;
        while(true) {
            while (!q.empty()) {
                ll u = q.front(); q.pop();
                for (int v = 0; v < N; v++) {
                    if (T[v]) continue;

                    ll delta = lx[u] + ly[v] - W[u][v];
                    if (slack[v] >= delta) {
                        parent[v] = u;
                        if (delta == 0) {
                            if (check(v)) return;
                        } else {
                            slack[v] = delta;
                        }
                    }
                }
            }

            update();

            for (int i = 0; i < N; i++) {
                if (!T[i] && slack[i] == 0) {
                    if (check(i)) return;
                }
            }
        }
    };

    for(int i = 0;i < N;i++) {
        fill(S.begin(), S.end(), 0);
        fill(T.begin(), T.end(), 0);
        fill(parent.begin(), parent.end(), -1);
        fill(slack.begin(), slack.end(), INF);

        while(!q.empty()) q.pop();

        bfs(i);
    }

    ll ans = 0;
    for(int i = 0;i < N;i++) if(r_match[i] != -1) ans += W[r_match[i]][i];
    cout << -ans << '\n';

    vector<int> V(N);
//    for(int i = 0;i < N;i++) V[r_match[i]] = i;
//    for(auto i: V) cout << i << " ";
//    cout << '\n';
    for(int i = 0;i < N;i++) if(r_match[i] != -1) cout << r_match[i] << " " << i << '\n';

}

int main() {_
    solve();
    return 0;
}

//3
//3 1 2
//6 5 4
//3 7 2