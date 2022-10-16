#include <iostream>
#include <vector>

struct Node {
long long left_brackets, right_brackets, len;

Node() {
    left_brackets = 0;
    right_brackets = 0;
    len = 0;
}

Node(Node a, Node b) {
    left_brackets = a.left_brackets + b.left_brackets - std::min(a.left_brackets, b.right_brackets);
    right_brackets = a.right_brackets + b.right_brackets - std::min(a.left_brackets, b.right_brackets);
    len = a.len + b.len + std::min(a.left_brackets, b.right_brackets);
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
            if (values[left] == '(') {
                tree[vertex].left_brackets = 1;
                tree[vertex].right_brackets = 0;
            } else {
                tree[vertex].left_brackets = 0;
                tree[vertex].right_brackets = 1;
            }
            tree[vertex].len = 0;
        } else {
            size_t mid = (left + right) / 2;
            build(values, vertex * 2, left, mid);
            build(values, vertex * 2 + 1, mid + 1, right);
            tree[vertex] = Node(tree[vertex * 2], tree[vertex * 2 + 1]);
        }
    }
};

int main() {
    long long M, l, r;
    std::string input;
    std::cin >> input;
    std::vector<char> input_to_vector(input.length());
    for (size_t i = 0; i < input_to_vector.size(); ++i) {
        input_to_vector[i] = input[i];
    }
    Segment_tree<char> tree(input_to_vector);
    std::cin >> M;
    for (int i = 0; i < M; ++i) {
        std::cin >> l >> r;
        std::cout << 2 * tree.get(1, 0, input.size() - 1, l - 1, r - 1).len << '\n';
    }
    return 0;
}
