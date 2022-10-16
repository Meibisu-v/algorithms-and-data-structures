#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>


void dfs_visit(const std::vector<std::list<int>> &graph, int start, std::list<int> &answer,
               std::vector<int> &colors, int &flag) {
    colors[start] = 1;
    for (auto elem : graph[start]) {
        if (colors[elem] == 0) {
            dfs_visit(graph, elem, answer, colors, flag);
        }
    }
    colors[start] = 2;
    answer.push_front(start);
}

std::list<int> top_sort(const std::vector<std::list<int>> &graph) {
    std::vector<int> colors(graph.size() + 1, 0);
    std::list<int> answer;

    for (size_t i = 1; i < graph.size(); ++i) {
        if (colors[i] == 0) {
            int flag = 0;
            dfs_visit(graph, i, answer, colors, flag);
        }
    }
    return answer;
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
    }

    std::vector<char> color(n + 1, 'w');
    std::vector<std::pair<int, int>> t_out(n + 1);


    std::list<int> order = top_sort(list_edg);
    std::vector<std::list<int>> graph_transp(n + 1, std::list<int>());
    for (size_t i = 1; i <= n; ++i) {
        for (auto elem : list_edg[i]) {
            graph_transp[elem].push_back(i);
        }
    }
    std::vector<char> vis_color(n + 1, 'w');
    int comp_cnt = 0;
    std::vector<int> list_comp(n + 1, 0);
    for (auto i : order) {
        if (vis_color[i] == 'w') {
            std::list<int> elements;
            elements.push_back(i);
            components(graph_transp, vis_color, i, elements);
            ++comp_cnt;
            for (auto i : elements) {
                list_comp[i] = comp_cnt;
            }
        }
    }
    std::cout << comp_cnt << '\n';
    for (size_t i = 1; i <= n; ++i) {
        std::cout << list_comp[i] << ' ';
    }
    return 0;
}
