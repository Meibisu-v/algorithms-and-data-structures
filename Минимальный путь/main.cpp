#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <limits>


int dijkstra(const std::vector<std::list<int>> &adj_list, const std::vector<std::vector<int>> &len_edg,
             const int& start, const int& finish){
    size_t n = adj_list.size();
    std::vector<int> dist(n + 1, std::numeric_limits<int>::max());
    std::vector<int> used(n + 1, 0);
    dist[start] = 0;
    int curr_vertex = start;

    while (true){

        int next_vertex = 0;

        for (size_t i = 0; i <= n; ++i) {
            if ((next_vertex == 0 || dist[i] < dist[next_vertex]) && used[i] == 0){
                next_vertex = i;
            }
        }

        if (dist[next_vertex] == dist[0]) break;

        curr_vertex = next_vertex;
        used[curr_vertex] = 1;

        for (auto it : adj_list[curr_vertex]) {
            if (used[it] == 0){
                dist[it] = std::min(dist[curr_vertex] + len_edg[curr_vertex][it], dist[it]);
            }
        }

    }
    if (dist[finish] == dist[0]) {
        return -1;
    } else return dist[finish];
}

int main(){
    size_t n, m;
    size_t start, finish;
    std::cin >> n >> m;
    std::cin >> start >> finish;
    std::vector<std::list<int>> list_edg;
    std::vector<std::vector<int>> len_edg;
    list_edg.reserve(n + 1);
    for (size_t i = 0; i <= n; ++i){
        list_edg.push_back(std::list<int>());
        len_edg.push_back(std::vector<int>(n + 1));
    }
    int a, b, t;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> a >> b >> t;
        list_edg[a].push_back(b);
        len_edg[a][b] = t;
    }
    std::cout << dijkstra(list_edg, len_edg, start, finish);
    return 0;
}
