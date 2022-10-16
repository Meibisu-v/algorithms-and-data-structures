#include<iostream>
#include<vector>
#include<set>
#include<algorithm>


int find_set(std::vector<int> &ds_set, int x) {
    if (x == ds_set[x]) return x;
    return ds_set[x] = find_set(ds_set, ds_set[x]);
}

void unite_set(std::vector<int> &ds_set, int x, int y){
    x = find_set(ds_set, x);
    y = find_set(ds_set, y);
    if(x == y) return;
    ds_set[x] = y;
}

int main() {
    size_t n, m;
    size_t answer = 0;
    std::cin >> n >> m;
    std::vector <int> ds_union(n + 1);

    for(size_t i = 1; i <= n; ++i) {
        ds_union[i] = i;
    }

    std::vector<std::pair<int, std::pair<int, int>>> list_edj(m + 1);

    for(size_t i = 0; i < m; ++i){
        int a, b, weight;
        std::cin >> a >> b >> weight;
        list_edj[i] = std::make_pair(weight, std::make_pair(a, b));
    }
    std::sort(list_edj.begin(), list_edj.end() - 1);
    size_t vert_cnt = 0;
    for(size_t i = 0; i < m && vert_cnt < n - 1; ++i){
        int vertex_a = find_set(ds_union, list_edj[i].second.first);
        int vertex_b = find_set(ds_union, list_edj[i].second.second);
        if(vertex_a != vertex_b){
            answer += list_edj[i].first;
            ++vert_cnt;
            unite_set(ds_union, vertex_a, vertex_b);
        }
    }
    std::cout << answer;
    return 0;
}
