#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <queue>

#define NO_EDGE -1


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
			if (adj_matrix[v][i] != NO_EDGE) {
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


const long long MAX_DIST = 1000000000;

std::vector<long long> ford_bellman(const graph_adj_matrix &graph, size_t start = 1) {
    std::vector<long long> dist(graph.get_vertex_count() + 1, MAX_DIST);
    dist[start] = 0;
    for (size_t j = 1; j < graph.get_vertex_count(); ++j) {
        for (size_t i = 1; i <= graph.get_vertex_count(); ++i) {
            auto neighbor = graph.get_neighbors(i);
            if (dist[i] == MAX_DIST) continue;
            for (auto next_vertex : neighbor) {
                dist[next_vertex] = std::min(dist[next_vertex], dist[i] + graph.get_weight(i, next_vertex));
            }
        }
    }
    return dist;
}


std::vector<long long> SPFA (const graph_adj_matrix &graph, size_t start = 1) {
    std::vector<long long> dist(graph.get_vertex_count() + 1, MAX_DIST);
    std::vector<int> in_queue(graph.get_vertex_count() + 1, 0);
    dist[start] = 0;
    std::queue<size_t> queue_;
    queue_.push(start);
    while(!queue_.empty()) {
        size_t curr_vertex = queue_.front();
        in_queue[curr_vertex] = 0;
        queue_.pop();
        auto neighbor = graph.get_neighbors(curr_vertex);
        for (auto next_vertex : neighbor) {
            if (dist[curr_vertex] + graph.get_weight(curr_vertex, next_vertex) < dist[next_vertex] && !in_queue[next_vertex]) {
                queue_.push(next_vertex);
                in_queue[next_vertex] = 1;
            }
            dist[next_vertex] = std::min(dist[next_vertex], dist[curr_vertex] + graph.get_weight(curr_vertex, next_vertex));
            }
    }
    return dist;
}


std::vector<long long> dijkstra(const graph_adj_matrix &adj_list, size_t start){
    size_t n = adj_list.get_vertex_count();
    std::vector<long long> dist(n + 1, MAX_DIST);
    std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>,
                        std::greater<std::pair<size_t, size_t>>> queue_vertex;
    std::vector<int> used(n + 1, 0);
    dist[start] = 0;
    int curr_vertex = start;
    queue_vertex.push({0, start});
    while (!queue_vertex.empty()){
        size_t next_vertex = queue_vertex.top().second;
        long long next_dist = queue_vertex.top().first;
        queue_vertex.pop();
        if (next_dist > dist[next_vertex]) continue;
        curr_vertex = next_vertex;
        used[curr_vertex] = 1;
        auto neighbors = adj_list.get_neighbors(curr_vertex);
        for (auto it : neighbors) {
            if (used[it] == 0){
                long long weight = adj_list.get_weight(curr_vertex, it);
                if (dist[curr_vertex] + weight < dist[it]) {
                    dist[it] = std::min(dist[curr_vertex] + weight, dist[it]);
                    queue_vertex.push({dist[it], it});
                }
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
    graph_adj_matrix len_edg(n + 1, true);
    long long weight;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> weight;
            //if (i == j) weight = NO_EDGE;
            len_edg.add_edge(i + 1, j + 1, weight);
        }
    }

    for (size_t i = 1; i <= n; ++i) {
        len_edg.add_edge(n + 1, i, 0);
    }

    auto change_weight = SPFA(len_edg, n + 1);
    for (size_t j = 1; j <= n; ++j) {
        for (auto i : len_edg.get_neighbors(j)) {
            int64_t new_weight = len_edg.get_weight(j, i) + change_weight[j] - change_weight[i];
            len_edg.set_weight(j, i, new_weight);
        }
    }
    long long max_dist = 0;
    std::vector<std::vector<long long>> min_dist;
    for (size_t i = 1; i <= n; ++i) {
        min_dist.emplace_back(dijkstra(len_edg, i));
        for (size_t j = 1; j <= n; ++j) {
            if (min_dist[i - 1][j] != MAX_DIST)
            max_dist = std::max(min_dist[i - 1][j] + change_weight[j] - change_weight[i], max_dist);
        }
    }
    std::cout << max_dist;
    return 0;
}
