#include <iostream>
#include <vector>
#include <queue>

const long long INF = 10000000005;

class Network {
private:

    std::queue<int> queue_;
    std::vector<int> parent;
    std::vector<int> col;

public:

    long long BFS(int start, int finish, int &);
    long long EdmondsKarp(int, int);

    Network(int N) : flow_value(N, std::vector<long long> (N)), adjacency(N), parent(N, -1), col(N) {};
    void add_edge(int x, int y, long long throughput);

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

void Network::add_edge (int x, int y, long long throughput){
    flow_value[x][y] = throughput;
    adjacency[x].push_back(y);
    adjacency[y].push_back(x);
}

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

long long valence(char c){
    if(c == '.') {
        return -1;
    }
    if(c == 'H') {
        return 1;
    }
    if(c == 'O') {
        return 2;
    }
    if(c == 'N') {
        return 3;
    }
    return 4;
}

int main(){
    int N, M;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> N >> M;
    std::vector<std::vector<long long> > table(N, std::vector<long long>(M));
    Network transport(N * M + 2);
    long long out = 0, in = 0;
    char elem;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            std::cin >> elem;
            table[i][j] = valence(elem);
            if(elem == '.') {
                continue;
            }
            if((i + j) % 2 == 0) {
                transport.add_edge(N * M, M * i + j, table[i][j]);
                out += table[i][j];
            } else {
                transport.add_edge(M * i + j, N * M + 1, table[i][j]);
                in += table[i][j];
            }
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if(table[i][j] == -1 || (i + j) % 2 == 1) {
                continue;
            }
            int current_vertex = i * M + j; //текущая вершина; ниже - её соседи
            int right_neighbour = i * M + j + 1;
            int left_neighbour = i * M + j - 1;
            int up_neighbour = (i - 1) * M + j;
            int down_neighbour = (i + 1) * M + j;
            if ((j + 1) < M && table[i][j + 1] != -1) {
                transport.add_edge(current_vertex, right_neighbour, 1);
            }
            if ((j - 1) >= 0 && table[i][j - 1] != -1) {
                transport.add_edge(current_vertex, left_neighbour, 1);
            }
            if ((i - 1) >= 0 && table[i - 1][j] != -1) {
                transport.add_edge(current_vertex, up_neighbour, 1);
            }
            if ((i + 1) < N && table[i + 1][j] != -1) {
                transport.add_edge(current_vertex, down_neighbour, 1);
            }
        }
    }
    long long throughput = transport.EdmondsKarp(N * M, N * M + 1);
    if (throughput == in && throughput == out && throughput != 0){
        std::cout << "Valid";
    } else {
        std::cout << "Invalid";
    }
    return 0;
}
