#include <iostream>
#include <vector>

const long long MIN_INFINITY = std::numeric_limits<long long>::min();

struct Node {
long long max_value;
size_t max_cnt;

Node() {
    max_value = MIN_INFINITY;
    max_cnt = 0;
}

Node(Node a, Node b) {
    max_value = std::max(a.max_value, b.max_value);
    max_cnt = (a.max_value > b.max_value ? a.max_cnt : b.max_cnt);
    if (a.max_value == b.max_value) {
        max_cnt = a.max_cnt + b.max_cnt;
    }
}
};

template <typename T>
class Segment_tree{
std::vector<Node> tree;
public:
    Segment_tree(const std::vector<T> &values) {
        tree.resize(4 * values.size());
        build(values, 1, 0, values.size() - 1);
    }
    Node get(size_t vertex, size_t tree_left, size_t tree_right,
             size_t left, size_t right) {
        if (left > right) {
            return Node();
        }
        if (left == tree_left && right == tree_right) {
            return tree[vertex];
        }
        size_t mid = (tree_left + tree_right) / 2;
        return Node(get(vertex * 2, tree_left, mid, left, std::min(right, mid)),
                    get(vertex * 2 + 1, mid + 1, tree_right, std::max(left, mid + 1), right));

    }
    void build(const std::vector<T> &values, size_t vertex, size_t left, size_t right) {
        if (left == right) {
            tree[vertex].max_value = values[left];
            tree[vertex].max_cnt = 1;
        } else {
            size_t mid = (left + right) / 2;
            build(values, vertex * 2, left, mid);
            build(values, vertex * 2 + 1, mid + 1, right);
            tree[vertex] = Node(tree[vertex * 2], tree[vertex * 2 + 1]);
        }
    }
};

int main() {
    int N;
    std::cin >> N;
    std::vector<int> a(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> a[i];
    }
    Segment_tree tree(a);
    int k, x, y;
    std::cin >> k;
    for (int i = 0; i < k; ++i) {
        std::cin >> x >> y;
        std::cout << tree.get(1, 0, N - 1, x - 1, y - 1).max_value << ' ';
        std::cout << tree.get(1, 0, N - 1, x - 1, y - 1).max_cnt << '\n';
    }
    return 0;
}
