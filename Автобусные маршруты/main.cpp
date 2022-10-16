#include <iostream>
#include <vector>
#include <list>
#include <set>


void euler_cycle(std::vector<std::list<int>> &graph, int start, std::list<int> &answer) {
    while (!graph[start].empty()) {
        int next_vert = graph[start].front();
        graph[start].pop_front();
        euler_cycle(graph, next_vert, answer);
    }
    answer.push_front(start);
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::list<int>> list_edg;
    list_edg.reserve(m + 1);

    for (int i = 0; i <= m; ++i){
        list_edg.push_back(std::list<int>());
    }
    std::vector<int> vertex_to(m + 1, 0);
    for (int i = 0; i < n; ++i){
        int k, prev, temp;
        std::cin >> k;
        for (int j = 0; j < k; ++j) {
            if (!j) std::cin >> prev;
            std::cin >> temp;
            list_edg[prev].push_back(temp);
            vertex_to[temp]++;
            prev = temp;
        }
    }


    bool is_path = true;
    std::vector<int> without_neib(m + 1, 0);
    int start = 1;
    while (list_edg[start].empty()) {
        ++start;
        if (start > m) {
            is_path = false;
            break;
        }
    }
    for (int i = 1; i <= m; ++i){
        if (list_edg[i].size() != vertex_to[i]) is_path = false;
    }
    std::list<int> answer;
    if (is_path) euler_cycle(list_edg, start, answer);
    for (auto i : answer) {
        if (without_neib[i]) is_path = false;
    }
    for (int i = i; i<= m; ++i) {
        if (!list_edg[i].empty()) is_path = false;
    }

    if (is_path) {
        std::cout << answer.size() << ' ';
        for (auto i : answer) {
            std::cout << i << ' ';
        }
    } else {
        std::cout << '0';
    }
    return 0;
}
