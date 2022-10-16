#include <iostream>
#include <vector>

class Graph {
protected:
	size_t vertex_count = 0, edge_count = 0;
	bool is_directed = true;

public:
	typedef size_t vertex;
	Graph(size_t vertex_count, bool is_directed) :
	    vertex_count(vertex_count), is_directed(is_directed), edge_count(0) {}

	size_t get_vertex_count() const {
		return vertex_count;
	}

	size_t get_edge_count() const {
		if (is_directed == 0) {
			return edge_count / 2;
		}
		return edge_count;
	}

	bool get_direction() const {
		return is_directed;
	}
	virtual std::vector<vertex> get_neighbors(vertex v) const = 0;
	virtual size_t get_neighbors_count(vertex v) const = 0;
};

const long long INF = 1000000000;

class graph_adj_matrix : public Graph {
	std::vector<std::vector<int64_t>> adj_matrix;

public:
	graph_adj_matrix(size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed) {
		adj_matrix = std::vector<std::vector<int64_t>>(vertex_count, std::vector<int64_t>(vertex_count, INF));
	}

	void add_edge(vertex start, vertex finish,  int64_t weight = 1) {
		adj_matrix[start][finish] = weight;
		if (!is_directed) {
			adj_matrix[finish][start] = weight;
		}
	}

	std::vector<vertex> get_neighbors(vertex v) const {
		std::vector<vertex> neighbors;
		for (size_t i = 0; i <= vertex_count; ++i) {
			if (adj_matrix[v][i]) {
				neighbors.push_back(i);
			}
		}
		return neighbors;
	}

	int64_t get_weight(vertex from, vertex to) const {
		return adj_matrix[from][to];
	}

	void set_weight(vertex from, vertex to, int64_t new_weight) {
		adj_matrix[from][to] = new_weight;
	}

	size_t get_neighbors_count(vertex v) const override {
		return get_neighbors(v).size();
	}
};


void get_path(const std::vector<std::vector<int>> &parent, const graph_adj_matrix &graph,
              std::vector<long long> &path, int i, int j,
              const std::vector<std::vector<int>> &edge_order){
    if(parent[i][j] == -1) {
        path.push_back(edge_order[i][j]);
        return;
    }
    get_path(parent, graph, path, i, parent[i][j], edge_order);
    get_path(parent, graph, path, parent[i][j], j, edge_order);
}

void Floyd(const graph_adj_matrix &graph, std::vector<std::vector<long long>> &length,
           std::vector<std::vector<int>> &parent){
    for (int i = 0; i < length.size(); ++i) {
        for (int j = 0; j < length.size(); ++j) {
            length[i][j] = graph.get_weight(i, j);
        }
    }
    for (int k = 0; k < length.size(); ++k) {
        for (int i = 0; i < length.size(); ++i) {
            for (int j = 0; j < length.size(); ++j) {
                if (length[i][k] < INF && length[k][j] < INF) {
                    if (length[i][k] + length[k][j] < length[i][j]) {
                        length[i][j] = length[i][k] + length[k][j];
                        parent[i][j] = k;
                    }
                }
            }
        }
    }
    for (int k = 0; k < length.size(); ++k) {
        for (int i = 0; i < length.size(); ++i) {
            for (int j = 0; j < length.size(); ++j) {
                if (length[i][k] < INF && length[k][j] < INF && length[k][k] < 0){
                        length[i][j] = -INF;
                }
            }
        }
    }
}

int main(){
    int vertex, edges, cities;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cin >> vertex >> edges >> cities;

    graph_adj_matrix graph(vertex, true);
    std::vector<std::vector<long long> > length(vertex, std::vector<long long> (vertex, INF));
    std::vector<std::vector<int> > parent(vertex, std::vector<int> (vertex, -1));
    std::vector<std::vector<int>> edge_order(vertex, std::vector<int>(vertex, 0));

    std::vector<long long> path;
    for (long long i, j, weight, k = 0; k < edges; ++k) {
        std::cin >> i >> j >> weight;
            graph.add_edge(i - 1, j - 1, -weight);
            edge_order[i - 1][j - 1] = k + 1;
    }

    Floyd(graph, length, parent);
    bool is_cycle = false; //цикл отрицательного веса
    int prev;
    std::cin >> prev;
    for (int current, l = 0; l < cities - 1; ++l) {
        std::cin >> current;
        if(length[prev - 1][current - 1] == -INF){
            is_cycle = true;
            break;
        }
        if(prev == current){
            continue;
        }
        get_path(parent, graph, path, prev - 1, current - 1, edge_order);
        prev = current;
    }
    if(is_cycle) {
        std::cout << "infinitely kind";
    }
    else {
        std::cout << path.size() << '\n';
        for (int j = 0; j < path.size(); ++j) {
            std::cout << path[j] << ' ';
        }
    }
}
