#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::cin;
using std::cout;
using std::queue;

const int N_MAX = 1e5;
const int INF = 1e9;


int bfs(vector <int> graph[N_MAX],
        vector <int> graph_dist[N_MAX],
        int start, int finish, int n, int m) {
    queue <int> dist[2];
    vector<int> dist_to_vertex(n + 1, INF);

	dist_to_vertex[start] = 0;
	dist[0].push(start);
	while (dist[0].size() + dist[1].size() > 0) {
		while (dist[0].size() > 0) {
			int v = dist[0].front(); dist[0].pop();
			for (size_t i = 0; i < graph[v].size(); i ++) {
				int cur_vert = graph[v][i];
				int edj_len = graph_dist[v][i];
				if (dist_to_vertex[cur_vert] > dist_to_vertex[v] + edj_len) {
					dist_to_vertex[cur_vert] = dist_to_vertex[v] + edj_len;
					dist[edj_len].push(cur_vert);
				}
			}
		}
		swap(dist[0], dist[1]);
	}
	if (dist_to_vertex[finish] == INF) return -1;
        else return dist_to_vertex[finish];
}

int main() {
    int n, m;
	cin >> n >> m;
    vector <int> graph[N_MAX];
    vector <int> graph_dist[N_MAX];
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		--a;
		--b;
		graph[a].push_back(b);
		graph_dist[a].push_back(0);
		graph[b].push_back(a);
		graph_dist[b].push_back(1);
	}
	int k;
	cin >> k;
	for (int i = 0; i < k; i ++) {
		int a, b;
		cin >> a >> b;
		cout << bfs(graph, graph_dist, --a, --b, n, m) << '\n';
	}
	return 0;
}
