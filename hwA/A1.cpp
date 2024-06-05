//
// Created by duck on 2024/4/1.
//
#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pll;
typedef tuple<ll, ll, ll> tll;
typedef pair<int, int> pii;
typedef tuple<int, int, int> tii;

bool dfs(vector<vector<ll>>& edge, vector<ll>& match, vector<bool>& vis, ll u) {
    for (ll v : edge[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        if (match[v] == -1 || dfs(edge, match, vis, match[v])) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

ll solve() {
    ll n, m;
    cin >> n >> m;
    vector<vector<ll>> edge(n);
    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;

        edge[a].push_back(b);
        edge[b].push_back(a);
    }

    vector<ll> match(n, -1);
    vector<bool> vis(n, false);

    ll ans = 0;
    for (ll i = 0; i < n; i++) {
        fill(vis.begin(), vis.end(), false);
        if (dfs(edge, match, vis, i)) ans++;
    }
    return ans / 2;
}


int main() {_
    // int N; cin >> N;
    // while(N--) {
    cout << solve() << endl;
    // }
    return 0;
}