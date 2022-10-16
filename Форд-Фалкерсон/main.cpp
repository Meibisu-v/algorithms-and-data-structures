#include <iostream>
#include <vector>

const size_t INF = 100000 * 1001;

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

class graph_adj_matrix : public Graph {
	std::vector<std::vector<int64_t>> adj_matrix;

public:
	graph_adj_matrix(size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed) {
		adj_matrix = std::vector<std::vector<int64_t>>(vertex_count + 1, std::vector<int64_t>(vertex_count + 1, 0));
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


int64_t min_flow(graph_adj_matrix &list_adj, size_t start, size_t finish, int64_t min_capacity,
             std::vector<int> &used) {
    if (start == finish) {
        return min_capacity;
    }
    used[start] = 1;
    auto neighbors = list_adj.get_neighbors(start);
    for (auto i : neighbors) {
        if (used[i] == 0) {
            int64_t result = min_flow(list_adj, i, finish, std::min(min_capacity, list_adj.get_weight(start, i)), used);
            if (result) {
                list_adj.set_weight(start, i, list_adj.get_weight(start, i) - result);
                list_adj.set_weight(i, start, list_adj.get_weight(i, start) + result);
                return result;
            }
        }
    }
    used[start] = 2;
    return 0;
}

int64_t ford_fulkerson(graph_adj_matrix &adj_list) {
    int64_t current = INF, result = 0;
    size_t n = adj_list.get_vertex_count();
    while (current != 0) {
        std::vector<int> color(n + 1, 0);
        current = min_flow(adj_list, 1, n, INF, color);
        result += current;
    }
    return result;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    graph_adj_matrix list_adj(n, true);
    for (size_t i = 0; i < m; ++i) {
        size_t from, to, weight = 0;
        std::cin >> from >> to >> weight;
        list_adj.add_edge(from, to, weight);
    }
    int64_t answer = ford_fulkerson(list_adj);
    std::cout << answer;
    return 0;
}
