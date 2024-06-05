#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> pll;
typedef tuple<ll, ll, ll> tll;
typedef pair<int, int> pii;
typedef tuple<int, int, int> tii;

int M, N;

struct blossom {
    int t, n, root;
    vector<vector<int>> g;
    vector<int> match, base, q, parent, label, timer;

    explicit blossom(int n_) : t(0), n(n_), g(n_), match(n_, -1), base(n_), q(n_), parent(n_), label(n_), timer(n_, -1) {}

    int find(int x) {return x == base[x] ? x : base[x] = find(base[x]);}

    void add_edge(const int u, const int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int lca(int u, int v) {
        ++t;
        while(true) {
            if(timer[u] == t) return u;
            timer[u] = t;
            u = (u == root ? u : find(parent[match[u]]));
            swap(u, v);
        }
    }

    void contract_one_side(int u, int v, int b) {
        while(find(u) != b) {
            parent[u] = v;
            v = match[u];
            if(label[v] == 1) {
                label[v] = 0;
                q.push_back(v);
            }
            base[u] = b;
            base[v] = b;
            u = parent[v];
        }
    }

    void aug_path(int v) {
        while(v != -1) {
            match[v] = parent[v];
            swap(v, match[parent[v]]);
        }
    }

    int bfs(int r) {
        root = r;
        fill(label.begin(), label.end(), -1);
        // fill(timer.begin(), timer.end(), -1);
        fill(parent.begin(), parent.end(), -1);
        iota(base.begin(), base.end(), 0);

        q.clear();
        q.push_back(root);
        label[root] = 0;
        for(int i = 0; i < (int) q.size(); i++) {
            int u = q[i];
            for(int v : g[u]) {
                if(label[v] == -1) {
                    parent[v] = u;
                    if(match[v] == -1) {
                        aug_path(v);
                        return 1;
                    }
                    label[v] = 1;

                    q.push_back(match[v]);
                    label[match[v]] = 0;
                } else if(label[v] == 0 && base[u] != base[v]) {
                    int b = lca(u, v);
                    contract_one_side(u, v, b);
                    contract_one_side(v, u, b);
                }
            }
        }
        return 0;
    }

    void solve() {
        vector<int> V(n);
        iota(V.begin(), V.end(), 0);
        shuffle(V.begin(), V.end(), mt19937{random_device{}()});
        for(auto& u: V) if(!match[u]) for(auto& v: g[u]) if(!match[v]) {
            match[u] = v;
            match[v] = u;
            break;
        }

        for(int i = 0; i < n; i++) {
            if(match[i] == -1) bfs(i);
        }
    }

};



void solve() {
    cin >> N >> M;
    blossom B(N);
    for(int i = 0; i < M; i++) {
        int u, v; cin >> u >> v;
        B.add_edge(u, v);
    }
    B.solve();
    int ans = 0;
    for(int i = 0; i < N; i++) {
        if(B.match[i] != -1 && B.match[i] > i) ans++;
    }
    cout << ans << '\n';
    for(int i = 0; i < N; i++) {
        if(B.match[i] != -1 && B.match[i] > i)
            cout << i << ' ' << B.match[i] << '\n';
    }
}




int main() {_
    // int N; cin >> N;
    // while(N--) {
    solve();
    // }
    return 0;
}


//4 3
//1 0
//2 0
//2 3