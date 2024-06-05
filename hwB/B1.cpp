//
// Created by duck on 2024/4/15.
//
#include <bits/stdc++.h>
using namespace std;
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using ll = long long;
constexpr int INF = 1e9;

void solve() {
    int N; cin >> N;

    vector<vector<int>> vis(2, vector<int>(N, 0));
    vector<vector<int>> vertex_weight(2, vector<int>(N, 0));
    vector<vector<int>> edge_weight(N, vector<int>(N));

    vector<int> match(N, -1);
    vector<int> slack(N, 1e9);


    vector<int> q;

    for(int i = 0;i < N;i++) {
        for(int j = 0;j < N;j++) {
            cin >> edge_weight[i][j];
            edge_weight[i][j] *= -1;
            vertex_weight[0][i] = max(vertex_weight[0][i], edge_weight[i][j]);
        }
    }

    auto dfs = [&](auto self, int u) -> bool {
        vis[0][u] = 1;
        for(int v = 0;v < N;v++) {
            if(vis[1][v]) continue;

            int delta = vertex_weight[0][u] + vertex_weight[1][v] - edge_weight[u][v];
            if(delta == 0) {
                vis[1][v] = 1;
                if(match[v] == -1 || self(self, match[v])) {
                    match[v] = u;
                    return true;
                }
            } else {
                slack[v] = min(slack[v], delta);
            }
        }
        return false;
    };

    auto update = [&]() {
        int delta = INF;
        for(int i = 0;i < N;i++) if(!vis[1][i]) delta = min(delta, slack[i]);

        for(int i = 0;i < N;i++) {
            if(vis[0][i]) vertex_weight[0][i] -= delta;
            if(vis[1][i]) vertex_weight[1][i] += delta;
        }
    };

    for(int i = 0;i < N;i++) {
        while(true) {
            fill(vis[0].begin(), vis[0].end(), 0);
            fill(vis[1].begin(), vis[1].end(), 0);
            fill(slack.begin(), slack.end(), INF);

            if(dfs(dfs, i)) break;
            update();
        }
    }

    int ans = 0;
    for(int i = 0;i < N;i++) if(match[i] != -1) ans += edge_weight[match[i]][i];
    cout << -ans << '\n';

    for(int i = 0;i < N;i++) if(match[i] != -1) cout << match[i] << " " << i << '\n';

}

int main() {_
    solve();
    return 0;
}

//3
//3 1 2
//6 5 4
//3 7 2
