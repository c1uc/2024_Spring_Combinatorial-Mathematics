//
// Created by duck on 2024/4/29.
//
#include <bits/stdc++.h>
#define _ ios_base::sync_with_stdio(0);cin.tie(0);
using namespace std;
using ll = long long;

class TreeNode {
public:
    TreeNode *parent, *left, *right;
    ll val;
    TreeNode(ll val) : parent(nullptr), left(nullptr), right(nullptr), val(val) {}
};

void solve() {
    int N, K; cin >> N >> K;

    auto dfs = [&](TreeNode* root, string& s, auto self) -> void {
        if(root->left) {
            s += "1";
            self(root->left, s, self);
        } else {
            s += "0";
        }
        if(root->right) {
            s += "1";
            self(root->right, s, self);
        } else {
            s += "0";
        }
    };

    auto encode_tree = [&](vector<ll>& vec) -> string {
        vector<TreeNode*> nodes;
        stack<ll> s;
        TreeNode* root = nullptr;
        for(int i = 0;i < N;i++) {
            ll last = -1;
            nodes.emplace_back(new TreeNode(vec[i]));

            while(!s.empty() && vec[s.top()] > vec[i]) {
                last = s.top();
                s.pop();
            }

            if(!s.empty()) {
                nodes[i]->parent = nodes[s.top()];
                nodes[s.top()]->right = nodes[i];
            } else {
                root = nodes[i];
            }

            if(last != -1) {
                nodes[i]->left = nodes[last];
                nodes[last]->parent = nodes[i];
            }

            s.push(i);
        }

        return [&]() {
            string s;
            dfs(root, s, dfs);
            return s;
        }();
    };
    while(K--) {
        vector<ll> vec(N);
        for(auto &i: vec) cin >> i;

        auto encoded = encode_tree(vec);
        cout << encoded << endl;
    }
}

int main() {_
    solve();
    return 0;
}

//2 3
//1 3
//-1 -2
//20 30
