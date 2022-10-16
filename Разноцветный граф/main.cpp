#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>

typedef std::vector<std::vector<int>> Graph;

void DFS_visit(const Graph &, std::vector<int> &, int, bool &);

void DFS(const Graph &graph, bool &is_cycle) {
    std::vector<int> col(graph.size(), 0);
    for(size_t cur_vert = 0; !is_cycle && cur_vert < graph.size(); ++cur_vert) {
        if(col[cur_vert] == 0) {
            DFS_visit(graph, col, cur_vert, is_cycle);
        }
    }
}

void DFS_visit(const Graph &graph, std::vector<int> &col, int cur_ver,  bool &is_cycle) {
    col[cur_ver] = 1;
    for(size_t i = 0; !is_cycle && i < graph[cur_ver].size(); i++) {
        if(col[graph[cur_ver][i]] == 0) {
            DFS_visit(graph, col,  graph[cur_ver][i], is_cycle);
        }
        if(col[graph[cur_ver][i]] == 1) {
            is_cycle = true;
        }
    }
    if(is_cycle) {
        return;
    }
    col[cur_ver] = 2;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t n;
    std::cin >> n;
    Graph graph(n + 1);
     for (size_t i = 1; i <= n - 1; ++i){
        for (size_t j = 1; j <= n - i; ++j) {
            char tmp_val;
            std::cin >> tmp_val;
            if (tmp_val == 'B' ) {
                graph[i].push_back(j + i);
            } else {
                graph[j + i].push_back(i);
            }
        }
    }
    bool is_cycle = false;
    DFS(graph, is_cycle);
    if(!is_cycle) {
        std::cout << "YES" << '\n';
    }
    else {
        std::cout << "NO";
    }
    return 0;
}
