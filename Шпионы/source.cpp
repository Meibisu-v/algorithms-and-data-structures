#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int maxn = 100000;


class DSU {
    std::vector<int> ds_set;
    int ranks[maxn] = {0,};

public:
    DSU(size_t n) {
        ds_set.resize(n);
    }

    int find_set(int x) {
        if (x == ds_set[x])
            return x;
        return ds_set[x] = find_set(ds_set[x]);
    }
    
    void union_sets(int x, int y) {
        x = find_set(x);
        y = find_set(y);
        if (x != y) {
            if (ranks[x] < ranks[y]) {
                swap(x, y);
            }
            ds_set[y] = x;
            if (ranks[x] == ranks[y]) {
                ranks[x]++;
            }
        }
    }

    int& operator[](size_t i){
        return ds_set[i];
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    cin >> n;
    DSU parent(maxn);
    for (int i = 0; i <= maxn; i++) {
        parent[i] = i;
    }
    int result = 0;
    vector<pair<int, pair<int, int>>> adj_list;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; ++j) {
            int weight;
            cin >> weight;
            if (weight) {
                adj_list.push_back({weight, {i, j}});
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        int w;
        std::cin >> w;
        adj_list.push_back({w, {i, n}});
    }
    std::sort(adj_list.begin(), adj_list.end());
    for (auto vert_dist : adj_list) {
        int set_num1 = parent.find_set(vert_dist.second.first);
        int set_num2 = parent.find_set(vert_dist.second.second);
        if (set_num1 != set_num2) {
            result += vert_dist.first;
            parent.union_sets(set_num1, set_num2);
        }
    }
    cout << result;

    return 0;
}
