#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <queue>

#define NO_EDGE 100000

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

	std::vector<std::vector<std::pair<vertex, long long>>> get_adjList() const {
		return adj_list_;
	}

	std::vector<std::pair<vertex, long long>> get_neighbors(vertex vertex) const {
		return adj_list_[vertex];
	}
};

const long long MAX_DIST = 1000000000;

void find_negative_cycle(const graph_adj_list_weight &graph) {
    size_t start = graph.get_vertex_count();
    std::vector<size_t> parent(graph.get_vertex_count() + 1, 0);
    std::vector<long long> dist(graph.get_vertex_count() + 1, MAX_DIST);
    dist[start] = 0;
    for (size_t j = 1; j <= graph.get_vertex_count(); ++j) {
        for (size_t i = 1; i <= graph.get_vertex_count(); ++i) {
            auto neighbor = graph.get_neighbors(i);
            if (dist[i] == MAX_DIST) continue;
            for (auto next_vertex : neighbor) {
                if (dist[next_vertex.first] > dist[i] + next_vertex.second) {
                    dist[next_vertex.first] = dist[i] + next_vertex.second;
                    parent[next_vertex.first] = i;
                }
            }
        }
    }

    bool is_cycle = false;
    size_t vertex_start_cycle;
    for (size_t i = 1; i <= graph.get_vertex_count(); ++i) {
        auto neighbor = graph.get_neighbors(i);
        if (dist[i] == MAX_DIST) continue;
        for (auto next_vertex : neighbor) {
            if (dist[next_vertex.first] > dist[i] + next_vertex.second) {
                is_cycle = true;
                vertex_start_cycle = next_vertex.first;
            }
        }
        if (is_cycle) break;
    }
    if (!is_cycle) {
        std::cout << "NO";
        return;
    }
    std::cout << "YES\n";
    std::list<size_t> cycle;
    size_t neg_path = vertex_start_cycle;
    for (size_t i = 0; i < graph.get_vertex_count(); ++i) {
        neg_path = parent[neg_path];
    }
    vertex_start_cycle = neg_path;
    cycle.push_front(vertex_start_cycle);
    size_t curr_vertex = parent[vertex_start_cycle];
    while (curr_vertex != vertex_start_cycle) {
        cycle.push_front(curr_vertex);
        curr_vertex = parent[curr_vertex];
    }
    cycle.push_front(curr_vertex);
    std::cout << cycle.size() << '\n';
    for (auto i : cycle) {
        std::cout << i << ' ';
    }

}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t n, m;
    std::cin >> n;
    graph_adj_list_weight len_edg(n + 1, true);
    long long weight;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cin >> weight;
            if (weight != NO_EDGE) len_edg.add_edge(i + 1, j + 1, weight);
        }
    }

    //add new vertex
    for (size_t i = 1; i <= n; ++i) {
        len_edg.add_edge(n + 1, i, 0);
    }
    find_negative_cycle(len_edg);
    return 0;
}
