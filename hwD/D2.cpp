//
// Created by duck on 2024/4/29.
//
/* ProbId: D2-AB-Optimal-RMQ */
#include <bits/stdc++.h>
using namespace std;

constexpr int S = 9;
constexpr int INF = 1 << 30;
constexpr int MAX_N = 40000000;
constexpr int MAX_M = MAX_N / S + 1;
constexpr int MAX_LOG_M = 23;
constexpr int MAX_COMB = 1 << (2 * S);

int M;
int CarTree_min_idx_map[MAX_COMB][S][S];
int SparseTable[MAX_M][MAX_LOG_M];
int vec[MAX_N], encoded_tree[MAX_M], built[MAX_COMB];

class TreeNode {
public:
    TreeNode *parent, *left, *right;
    explicit TreeNode() : parent(nullptr), left(nullptr), right(nullptr) {}
    ~TreeNode() {
        delete left;
        delete right;
    }
};

auto lg = [](int x) -> int {
    return x ? __builtin_clz(1) - __builtin_clz(x) : 0;
};

void warm_up(int seq[], int N_) {
    copy(seq, seq + N_, vec);

    int N = S * (N_ / S) + (N_ % S ? S : 0);
    for(int i = N_;i < N;i++) vec[i] = INF;

    M = N / S;

    auto dfs = [&](TreeNode* root, int& s, auto self) -> void {
        s <<= 2;
        if(root->left) {
            s |= 2;
        }
        if(root->right) {
            s |= 1;
        }

        if(root->left) {
            self(root->left, s, self);
        }
        if(root->right) {
            self(root->right, s, self);
        }
    };

    auto encode_tree = [&](int idx) -> int {
        TreeNode* nodes[S];
        int s[S];
        int f = -1;
        TreeNode* root = nullptr;
        for(int i = 0;i < S;i++) {
            int last = -1;
            nodes[i] = new TreeNode();

            while(f != -1 && vec[idx + s[f]] > vec[idx + i]) {
                last = s[f--];
            }

            if(f != -1) {
                nodes[i]->parent = nodes[s[f]];
                nodes[s[f]]->right = nodes[i];
            } else {
                root = nodes[i];
            }

            if(last != -1) {
                nodes[i]->left = nodes[last];
                nodes[last]->parent = nodes[i];
            }

            s[++f] = i;
        }

        return [&]() {
            int ans = 0;
            dfs(root, ans, dfs);
            delete root;
            return ans;
        }();
    };

    auto build_CarTree_min_idx_map = [&](int idx, const int encoded) -> void {
        for(int l = 0;l < S;l++) {
            CarTree_min_idx_map[encoded][l][l] = l;
            for(int r = l + 1;r < S;r++) {
                int min_idx = vec[idx + r] < vec[idx + CarTree_min_idx_map[encoded][l][r - 1]] ? r : CarTree_min_idx_map[encoded][l][r - 1];
                CarTree_min_idx_map[encoded][l][r] = min_idx;
            }
        }
    };

    auto build_block_SparseTable = [&]() {
        for(int k = 0;k < M;k++) {
            SparseTable[k][0] = vec[k * S + CarTree_min_idx_map[encoded_tree[k]][0][S - 1]];
        }
        for(int j = 1;j <= lg(M);j++) {
            for(int k = 0;k < M;k++) {
                if(k + (1 << (j - 1)) >= M) {
                    SparseTable[k][j] = SparseTable[k][j - 1];
                } else {
                    SparseTable[k][j] = min(SparseTable[k][j - 1], SparseTable[k + (1 << (j - 1))][j - 1]);
                }
            }
        }
    };

    for(int k = 0; k < M; k++) {
        int encoded = encode_tree(k * S);
        encoded_tree[k] = encoded;
        if(!built[encoded]++) {
            build_CarTree_min_idx_map(k * S, encoded);
        }
    }
    build_block_SparseTable();
}

int query(int l, int r) {
    if(l > r) return -1;
    int L = l / S, R = r / S;
    int ans = INF;

    if(L == R) {
        int encoded = encoded_tree[L];
        int idx = CarTree_min_idx_map[encoded][l % S][r % S];
        return vec[L * S + idx];
    }

    auto ansL = [&]() {
        int encoded = encoded_tree[L];
        int idx = CarTree_min_idx_map[encoded][l % S][S - 1];
        return vec[L * S + idx];
    };

    auto ansR = [&]() {
        int encoded = encoded_tree[R];
        int idx = CarTree_min_idx_map[encoded][0][r % S];
        return vec[R * S + idx];
    };

    ans = min(ansL(), ansR());

    if(L + 1 < R) {
        int j = lg(R - L - 1);
        ans = min(ans, min(SparseTable[L + 1][j], SparseTable[R - (1 << j)][j]));
    }

    return ans;
}

//int naive_query(int l, int r) {
//    if(l > r) return -1;
//    int ans = INF;
//    for(int i = l;i <= r;i++) ans = min(ans, vec[i]);
//    return ans;
//}
//
//int main() {
//    int N = MAX_N;
//    random_device rd;
//    mt19937 gen(rd());
//    vector<int> seq(N);
//
//    for(auto &i: seq) i = gen() % 10000000;
//
//    warm_up(seq.data(), N);
//
//    int Q = 100;
//    vector<pair<int, int>> wrong_q;
//    while(Q--) {
//        int l = gen() % N, r = gen() % N;
//        if(query(l, r) != naive_query(l, r)) {
//            wrong_q.emplace_back(l, r);
//        }
//    }
//
//    cout << "Wrong queries: " << wrong_q.size() << '\n';
//    for(auto &p: wrong_q) {
//        cout << p.first << ' ' << p.second << '\n';
//        query(p.first, p.second);
//    }
//
//
//    return 0;
//}
