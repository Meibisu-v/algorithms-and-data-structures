#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const long long INF = 20090009990;

class DSU {
private:
    std::vector<int> tree;
    std::vector<int> rank;
public:
    void make_set(int);
    int find_set (int);
    void union_sets (int a, int b);
    DSU(int n) : tree(n), rank(n){
        for (int i = 0; i < n; ++i) {
            make_set(i);
        }
    }
};

void DSU::make_set (int v) {
    tree[v] = v;
    rank[v] = 0;
}

int DSU::find_set (int v) {
    if (v == tree[v]) {
        return v;
    }
    return tree[v] = find_set (tree[v]);
}

void DSU::union_sets (int a, int b) {
    a = find_set (a);
    b = find_set (b);
    if (a != b) {
        if (rank[a] < rank[b]){
            std::swap (a, b);
        }
        tree[b] = a;
        if (rank[a] == rank[b]) {
            ++rank[a];
        }
    }
}

struct Edge{
    int start;
    int finish;
    long long weight;
    int number;
    Edge(int b, long long c) : start(-1), finish(b), weight(c), number(-1){};
    Edge(int a, int b, long long c, int d = -1) : start(a), finish(b), weight(c), number(d) {};
    struct Comp {
        bool operator()(const Edge &e1, const Edge &e2) {
            return (e1.weight > e2.weight) || (e1.weight == e2.weight && e1.number > e2.number);
        }
    };
};

typedef std::vector<std::vector<Edge> > G_List;

void Dijkstra(const G_List &g, std::vector<int> &stations, std::vector<long long> &distance){
    std::vector<bool> used(g.size(), false);
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int> >, std::greater<std::pair<long long, int> > > queue; // dist, vertex
    int current;
    for (int j = 0; j < stations.size(); ++j) {
        distance[stations[j]] = 0;
        queue.push(std::make_pair(distance[stations[j]], stations[j]));
    }
    while (!queue.empty()) {
        current = queue.top().second;
        queue.pop();
        used[current] = true;
        for (int i = 0; i < g[current].size(); ++i) {
            if(!used[g[current][i].finish] && distance[g[current][i].finish] > distance[current] + g[current][i].weight){
                distance[g[current][i].finish] = distance[current] + g[current][i].weight;
                queue.push(std::make_pair(distance[g[current][i].finish], g[current][i].finish));
            }
        }
        while (!queue.empty() && used[queue.top().second]) {
            queue.pop();
        }
    }
}

int main(){
    int N, M, S;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> N >> S >> M;
    G_List g(N);
    std::vector<Edge> edges;

    std::vector<int> stations(S);
    for (int a, l = 0; l < S; ++l) {
        std::cin >> a;
        stations[l] = a - 1;
    }

    for (int u, v, i = 0; i < M; ++i) {
        long long d;
        std::cin >> u >> v >> d;
        g[u - 1].push_back(Edge(v - 1, d));
        g[v - 1].push_back(Edge(u - 1, d));
        edges.push_back(Edge(u - 1, v - 1, d));
    }

    std::vector<long long> distance(N, INF);
    Dijkstra(g, stations, distance);
    for (int m = 0; m < M; ++m) {
        edges[m].weight += distance[edges[m].start] + distance[edges[m].finish];
    }

    int count_request; 
    std::cin >> count_request;
    std::priority_queue<Edge, std::vector<Edge>, Edge::Comp> requests_and_edges;
    for (int x, y, b, j = 0; j < count_request; ++j) {
        std::cin >> x >> y >> b;
        requests_and_edges.push(Edge(x - 1, y - 1, b, j));
    }
    for (int j = 0; j < edges.size(); ++j) {
        requests_and_edges.push(edges[j]);
    }

    DSU dsu(N);
    std::vector<bool> answers(count_request);
    while (!requests_and_edges.empty()){
        Edge request = requests_and_edges.top();
        requests_and_edges.pop();
        if(request.number == -1){
            dsu.union_sets(request.start, request.finish);
        } else{
            answers[request.number] = (dsu.find_set(request.start) == dsu.find_set(request.finish));
        }
    }
    for (int n = 0; n < answers.size(); ++n) {
        if(answers[n]){
            std::cout << "YES\n";
        } else{
            std::cout << "NO\n";
        }
    }
}
