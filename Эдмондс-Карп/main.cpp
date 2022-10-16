#include <iostream>
#include <vector>
#include <queue>

const long long INF = 1000000009;

class Network {
private:

    std::queue<int> queue_;
    std::vector<int> parent;
    std::vector<int> col;

public:

    long long BFS(int start, int finish, int &);
    long long EdmondsKarp(int, int);

    Network(int N) : flow_value(N, std::vector<long long> (N)), adjacency(N), parent(N, -1), col(N) {};

    struct Edge {
        int finish;
        long long stream;
        Edge(int a = -1, long long b = 0){
            finish = a;
            stream = b;
        }
    };

    typedef std::vector<std::vector<int> > G_list;
    typedef std::vector<std::vector<long long> > G_matrix;

    G_list adjacency;
    G_matrix flow_value;

};


long long Network::BFS(int start, int finish, int &count_bfs) {
    queue_.push(start);

    bool found = false;

    while (!queue_.empty()) {
        int v = queue_.front();
        queue_.pop();
        for (int i = 0; i < adjacency[v].size() && !found; ++i) {
            if (flow_value[v][adjacency[v][i]] && col[adjacency[v][i]] != count_bfs) {
                col[adjacency[v][i]] = count_bfs;
                parent[adjacency[v][i]] = v;
                if(adjacency[v][i] == finish){
                    found = true;
                    break;
                }
                queue_.push(adjacency[v][i]);
            }
        }
        col[v] = count_bfs;
    }

    if(found){
        long long stream = flow_value[parent[finish]][finish];
        int current = finish;
        while (current != start) {
            if (flow_value[parent[current]][current] < stream) {
                stream = flow_value[parent[current]][current];
            }
            current = parent[current];
        }
        current = finish;
        while (current != start) {
            flow_value[parent[current]][current] -= stream;
            flow_value[current][parent[current]] += stream;
            current = parent[current];
        }
        return stream;
    }
    return 0;
}

long long Network::EdmondsKarp(int start, int finish){
    long long current = INF, stream = 0;
    int count_bfs = 1;
    while(current != 0){
        current = BFS(start, finish, count_bfs);
        stream += current;
        ++count_bfs;
    }
    return stream;
}

int main(){
    int vertex, edges;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> vertex >> edges;

    Network flow(vertex);

    for (size_t x, y, i = 0; i < edges; ++i) {
        long long throughput;
        std::cin >> x >> y >> throughput;
        flow.flow_value[x - 1][y - 1] = throughput;
        flow.adjacency[x - 1].push_back(y - 1);
        flow.adjacency[y - 1].push_back(x - 1);
    }
    std::cout << flow.EdmondsKarp(0, vertex - 1);
    return 0;
}
