#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <queue>


class Graph {
protected:
	size_t vertex_count, edge_count;
	bool is_directed;

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

	virtual size_t get_vertex_deg(vertex vertex) const = 0;
};



class graph_adj_list_weight : public Graph {
private:
	std::vector<std::vector<std::pair<vertex, long long>>> adj_list_;

public:
	graph_adj_list_weight(size_t vertex_count, bool is_directed) :
	    Graph(vertex_count, is_directed), adj_list_(vertex_count + 1) {}

	void add_edge(vertex start, vertex finish, long long weight) {
		adj_list_[start].push_back({finish, weight});
		if (!is_directed) {
			adj_list_[finish].push_back({start, weight});
		}
		edge_count++;
	}

	size_t get_vertex_deg(vertex vertex) const {
		return adj_list_[vertex].size();
	}

	size_t get_vertex_count() const {
        return vertex_count;
	}

	std::vector<std::vector<std::pair<vertex, long long>>> get_adjList() const {
		return adj_list_;
	}

	std::vector<std::pair<vertex, long long>> get_neighbors(vertex vertex) const {
		return adj_list_[vertex];
	}
};

const long long MAX_DIST = 100000000;

std::vector<long long> ford_bellman(const graph_adj_list_weight &graph, size_t start = 1) {
    std::vector<long long> dist(graph.get_vertex_count() + 1, MAX_DIST);
    dist[start] = 0;
    for (size_t j = 1; j < graph.get_vertex_count(); ++j) {
        for (size_t i = 1; i <= graph.get_vertex_count(); ++i) {
            auto neighbor = graph.get_neighbors(i);
            if (dist[i] == MAX_DIST) continue;
            for (auto next_vertex : neighbor) {
                dist[next_vertex.first] = std::min(dist[next_vertex.first], dist[i] + next_vertex.second);
            }
        }
    }
    return dist;
}

const int64_t NO_EDGE = 0;

class graph_adj_matrix_weight: public Graph {
	std::vector<std::vector<int64_t>> adj_matrix;

public:
	graph_adj_matrix_weight(size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed) {
		adj_matrix = std::vector<std::vector<int64_t>>(vertex_count + 1, std::vector<int64_t>(vertex_count + 1, NO_EDGE));
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

	size_t get_neighbors_count(vertex v) const {
		return get_neighbors(v).size();
	}

	size_t get_vertex_deg(vertex vertex) const {
        size_t cnt = 0;
        for (size_t i = 1; i <= vertex_count; ++i) {
            if (adj_matrix[i][vertex] != NO_EDGE) ++cnt;
            if (adj_matrix[vertex][i] != NO_EDGE) ++cnt;
        }
        return cnt;
	}
};


std::vector<std::vector<long long>> floyd_warshall(const graph_adj_matrix_weight &graph) {

    size_t n = graph.get_vertex_count();
    std::vector<std::vector<long long>> dist(n + 1, std::vector<long long>(n + 1, MAX_DIST));

    //init dist
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            dist[i][j] = graph.get_weight(i, j);
        }
    }
    //init

    for (size_t k = 1; k <= n; ++k) {
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = 1; j <= n; ++j) {
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    return dist;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t n;
    std::cin >> n;
    graph_adj_matrix_weight len_edg(n, true);
    long long weight;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> weight;
            len_edg.add_edge(i + 1, j + 1, weight);
        }
    }
    auto answer = floyd_warshall(len_edg);
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            std::cout << answer[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
