#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>

template<typename T>
struct Summ {
    T operator()(const T &a = 0, const T &b = 0) {
        return a + b;
    }
};

template<typename T, typename Function>
class SegmentTree {

std::vector<T> segment_tree_;
Function func;
static size_t Log_2(size_t value);
static size_t PowerOfTwo_Ceil(size_t value);

T Parent(int64_t idx) const {
    return idx / 2;
}

T LeftChild(int64_t idx) const {
    return idx * 2;
}

T RightChild(int64_t idx) const {
    return idx * 2 + 1;
}

public:
    explicit SegmentTree(const std::vector<T> &values);

    void Update(size_t id_x, const T &value);

    T Query(size_t left, size_t right);
};

template<typename T, typename Function>
size_t SegmentTree<T, Function>::Log_2(size_t value) {
    size_t result = 0;
    while (value >>= 1u) {
        ++result;
    }
    return result;
}

template<typename T, typename Function>
size_t SegmentTree<T, Function>::PowerOfTwo_Ceil(size_t value) {
    return 1u << (Log_2(value - 1) + 1);
}

template<typename T, typename Function>
SegmentTree<T, Function>::SegmentTree(const std::vector<T> &values)
                            : segment_tree_(2 * PowerOfTwo_Ceil(values.size())) {

    const size_t half_tree_size = segment_tree_.size() / 2;
    const size_t values_size = values.size();
    for (size_t idx = 0; idx < values_size; ++idx) {
        segment_tree_[idx + half_tree_size] = values[idx];
    }

    for (size_t i = half_tree_size - 1; i > 0; --i) {
        segment_tree_[i] = func(segment_tree_[LeftChild(i)], segment_tree_[RightChild(i)]);
    }
}

template<typename T, typename Function>
void SegmentTree<T, Function>::Update(size_t id, const T &value) {
    size_t tree_size = segment_tree_.size();
    id += tree_size / 2;
    segment_tree_[id] = value;
    while ((id = Parent(id)) > 0) {
        segment_tree_[id] = func(segment_tree_[LeftChild(id)], segment_tree_[RightChild(id)]);
    }
}

template<typename T, typename Function>
T SegmentTree<T, Function>::Query(size_t left, size_t right) {
    size_t tree_size = segment_tree_.size();
    left += tree_size / 2;
    right += tree_size / 2;
    T left_res = T(0);
    T right_res = T(0);
    while (left > 0 && right > 0 && left < right) {
        if (left == RightChild(Parent(left))) {
            left_res = func(left_res, segment_tree_[left]);
        }
        left = Parent(left + 1);
        if (right == LeftChild(Parent(right))) {
            right_res = func(right_res, segment_tree_[right]);
        }
        right = Parent(right - 1);
    }
    if (left == right) {
        left_res = func(left_res, segment_tree_[left]);
    }
    return func(left_res, right_res);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t values_num;
  std::cin >> values_num;
  std::vector<int64_t> values(values_num);
  for (size_t i = 0; i < values_num; ++i) {
    std::cin >> values[i];
  }

  SegmentTree<int64_t, Summ<int64_t>> segment_tree(values);

  size_t quires_num;
  std::cin >> quires_num;

  for (size_t i = 0; i < quires_num; ++i) {
    char query;
    std::cin >> query;
    if (query == 'u') {
      size_t id_x;
      int64_t new_value;
      std::cin >> id_x >> new_value;
      segment_tree.Update(id_x - 1, new_value);
    } else if (query == 's') {
      size_t left, right;
      std::cin >> left >> right;
      std::cout << segment_tree.Query(left - 1, right - 1) << ' ';
    }
  }
  return 0;
}
