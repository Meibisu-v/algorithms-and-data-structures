#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

template <class T>
class DSU{
    std::vector<T> ds_set;
public:
    DSU(size_t n) {
        ds_set.resize(n);
    }
    T find_set(T x) {
        if (x == ds_set[x]) return x;
        return ds_set[x] = find_set(ds_set[x]);
    }
    void unite_set(T x, T y) {
        x = find_set(x);
        y = find_set(y);
        if(x == y) return;
        ds_set[x] = y;
    }
    T& operator[](size_t i){
        return ds_set[i];
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t n, m;
    std::cin >> n >> m;
    DSU<int> ds_union(n + 1);

    for(size_t i = 1; i <= n; ++i) {
        ds_union[i] = i;
    }

    std::vector<std::pair<int, std::pair<int, int>>> list_edj(m + 1);
    std::vector<int> weight(n + 1, 0);
    int action;
    for(size_t i = 0; i < m; ++i) {
        std::cin >> action;
        if (action == 1) {
            int a, b, w;
            std::cin >> a >> b >> w;
            int vertex_a = ds_union.find_set(a);
            int vertex_b = ds_union.find_set(b);
            if(vertex_a != vertex_b) {
                ds_union.unite_set(vertex_a, vertex_b);
                weight[ds_union.find_set(a)] = weight[vertex_a] + weight[vertex_b];
            }
            weight[ds_union.find_set(a)] += w;
        }
        if (action == 2) {
            int a;
            std::cin >> a;
            std::cout << weight[ds_union.find_set(a)] << '\n';
        }
    }
    return 0;
}
