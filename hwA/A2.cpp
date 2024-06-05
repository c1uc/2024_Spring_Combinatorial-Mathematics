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
    vis[u] = true;
    for (ll v : edge[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        if (match[v] == -1 || dfs(edge, match, vis, match[v])) {
            match[v] = u;
            match[u] = v;
            return true;
        }
    }
    return false;
}

void solve() {
    ll n, m;
    cin >> n >> m;
    vector<vector<ll>> edge(n);
    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;

        edge[a].push_back(b);
        edge[b].push_back(a);
    }

    vector<ll> color(n, -1);
    queue<ll> q;
    set<ll> s[2];

    for(int i = 0;i < n;i++) {
        if(color[i] != -1) continue;
        q.push(i);
        color[i] = 0;
        s[0].insert(i);
        while(!q.empty()) {
            ll u = q.front();
            q.pop();
            for(auto v: edge[u]) {
                if(color[v] == -1) {
                    color[v] = 1 - color[u];
                    s[color[v]].insert(v);
                    q.push(v);
                }
            }
        }
    }

    vector<ll> match(n, -1);
    vector<bool> vis(n, false);
    ll ans = 0;

    for(auto i : s[0]) {
        fill(vis.begin(), vis.end(), false);
        if(dfs(edge, match, vis, i)) ans++;
    }

//    for(auto i: s[0]) cout << i << " ";
//    cout << endl;
//    for(auto i: s[1]) cout << i << " ";
//    cout << endl;
//    for(auto i: match) cout << i << " ";
//    cout << endl;

    cout << ans << endl;
    for(auto i: s[0]) {
        if(match[i] != -1) cout << i << " " << match[i] << endl;
    }

    fill(vis.begin(), vis.end(), false);
    for(auto i: s[0]) {
        if(match[i] == -1) dfs(edge, match, vis, i);
    }

//    for(auto i: vis) cout << i << " ";
//    cout << endl;

    set<ll> cover;
    for(auto i: s[0]) {
        if(!vis[i]) cover.insert(i);
        else if(match[i] != -1) cover.insert(match[i]);
    }
    cout << cover.size() << endl;
    for(auto i: cover) cout << i << " ";
    cout << endl;
}


int main() {_
    // int N; cin >> N;
    // while(N--) {
    solve();
    // }
    return 0;
}

//17 16
//0 9
//1 9
//2 10
//3 9
//4 9
//4 10
//4 11
//5 11
//6 11
//6 12
//7 10
//7 13
//7 15
//8 10
//8 14
//8 16