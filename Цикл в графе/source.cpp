#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>


void find_cycle(const std::vector<std::list<int>> &graph, std::vector<char> &color,
         const int &start, std::list<size_t> &path, bool &is_cycle){
    color[start] = 'g';
    if (is_cycle) return;
    for (const auto &neigh : graph[start]){
        if (color[neigh] == 'w' && !is_cycle){
            path.push_back(neigh);
            find_cycle(graph, color, neigh, path, is_cycle);
        } else
        if (color[neigh] == 'g' && !is_cycle){
                std::cout << "YES\n";
                auto it = path.begin();
                while (*it != neigh) ++it;
                while (it != path.end()) {
                    std::cout << *it << ' ';
                    ++it;
                }
                is_cycle = true;
                return;
        }
    }
    color[start] = 'b';
    path.pop_back();
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    size_t n, m;
    std::cin >> n >> m;

    std::vector<std::list<int>> list_edg;
    list_edg.reserve(n + 1);
    for (size_t i = 0; i <= n; ++i){
        list_edg.push_back(std::list<int>());
    }

    int a, b;
    std::vector<char> color(n + 1, 'w');
    for (size_t i = 0; i < m; ++i){
        std::cin >> a >> b;
        list_edg[a].push_back(b);
    }
    for (size_t i = 0; i <= n; ++i){
        list_edg[i].unique();
    }
    bool is_cycle = false;
    for (size_t i = 1; i <= n; ++i){
        std::list<size_t> path{i};
        is_cycle = false;
        if (color[i] == 'w') find_cycle(list_edg, color, i, path, is_cycle);
        if (is_cycle) break;
    }
    if (!is_cycle) std::cout << "NO";
    return 0;
}
