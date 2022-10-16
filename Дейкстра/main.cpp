#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <queue>

const size_t MAX_DIST = 2009000999;


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
	std::vector<std::vector<std::pair<vertex, vertex>>> adj_list_;

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

	std::vector<std::vector<std::pair<vertex, vertex>>> get_adjList() const {
		return adj_list_;
	}

	std::vector<std::pair<vertex, vertex>> get_neighbors(vertex vertex) const {
		return adj_list_[vertex];
	}
};


void dijkstra(const graph_adj_list_weight &adj_list, const size_t& start){
    size_t n = adj_list.get_vertex_count();
    std::vector<size_t> dist(n + 1, MAX_DIST);
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
            if (used[it.first] == 0){
                if (dist[curr_vertex] + it.second < dist[it.first]) {
                    dist[it.first] = std::min(dist[curr_vertex] + it.second, dist[it.first]);
                    queue_vertex.push({dist[it.first], it.first});
                }
            }
        }

    }
    for (int i = 0; i < n; ++i) {
        std::cout << dist[i] << ' ';
    }
    std::cout << '\n';
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t k;
    std::cin >> k;
    for (size_t j = 0; j < k; ++j) {
        size_t n, m;
        std::cin >> n >> m;
        graph_adj_list_weight len_edg(n, false);
        size_t a, b;
        long long weight;
        for (size_t i = 0; i < m; ++i) {
            std::cin >> a >> b >> weight;
            len_edg.add_edge(a, b, weight);
        }
        size_t start;
        std::cin >> start;
        dijkstra(len_edg, start);
    }
    return 0;
}
