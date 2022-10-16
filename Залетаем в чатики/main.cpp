#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>


//class comp{
    bool comp(const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first > b.first;
    }
//};

void DFS(const std::vector<std::list<int>> &graph, std::vector<char> &color,
         int start, std::vector<std::pair<int, int>> &t_out, size_t &time){
    color[start] = 'g';
    ++time;
    for (const auto &neigh : graph[start]){
        if (color[neigh] == 'w'){
            DFS(graph, color, neigh, t_out, time);
        }
    }
    ++time;
    t_out[start] = std::make_pair(time, start);
    color[start] = 'b';
}

void components(const std::vector<std::list<int>> &graph, std::vector<char> &color,
                int start, std::list<int> &elements) {
    color[start] = 'g';
    for (auto i : graph[start]){
        if (color[i] == 'w') {
            elements.push_front(i);
            components(graph, color, i, elements);
        }
    }
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
    for (size_t i = 0; i < m; ++i){
        std::cin >> a >> b;
        list_edg[a].push_back(b);
        list_edg[b].push_back(a);
    }

    std::vector<char> color(n + 1, 'w');
    std::vector<std::pair<int, int>> t_out(n + 1);
            size_t time = 0;

    for (size_t i = 1; i <= n; ++i) {
        if (color[i] == 'w') {
            DFS(list_edg, color, i, t_out, time);
        }
    }
    //
    std::sort(t_out.begin() + 1, t_out.end(), comp);
    //
    std::vector<std::list<int>> graph_transp(n + 1, std::list<int>());
    for (size_t i = 1; i <= n; ++i) {
        for (auto elem : list_edg[i]) {
            graph_transp[elem].push_back(i);
        }
    }
    //
    std::vector<char> vis_color(n + 1, 'w');
    std::vector<std::list<int>> graph_comp;
    for (size_t i = 1; i <= n; ++i) {
        if (vis_color[t_out[i].second] == 'w') {
            std::list<int> elements;
            elements.push_back(t_out[i].second);
            components(graph_transp, vis_color, t_out[i].second, elements);
            graph_comp.push_back(elements);

        }
    }
    std::cout << graph_comp.size() << '\n';
    for (size_t i = 0; i < graph_comp.size(); ++i) {
        std::cout << graph_comp[i].size() << '\n';
        for (auto j : graph_comp[i]) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
