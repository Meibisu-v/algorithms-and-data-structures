#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
T GCD_calc(T a, T b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (a > b) return GCD_calc(a % b, b);
    else return GCD_calc(a, b % a);
}

template<typename T>
struct GCD {
  T operator()(const T &lhs, const T &rhs) const {
    return GCD_calc(lhs, rhs);
  }
};

template<class T, class Function>
class Sparse_table {
    std::vector<std::vector<T>> sparse_table;
    std::vector<size_t> log_;
    Function func;
public:
    Sparse_table(const std::vector<T> &values) {
        log_.resize(values.size() + 1, 0);
        size_t table_size = values.size();

        for (size_t i = 2; i < table_size; ++i) {
            log_[i] = log_[i / 2] + 1;
        }

        sparse_table.push_back(values);

        for (size_t next_size = 2; next_size <= table_size; next_size *= 2) {
            std::vector<T> next_level(table_size - next_size + 1);
            for (size_t i = 0; i < next_level.size(); ++i) {
                next_level[i] = func(sparse_table.back()[i], sparse_table.back()[i + next_size / 2]);
            }
            sparse_table.push_back(next_level);
        }
    }

    T query(size_t left, size_t right) const {
        size_t border = log_[right - left];
        return func(sparse_table[border][left], sparse_table[border][right - (1u << border) + 1]);
    }

};


int main() {
    size_t soldiers_cnt;
    std::cin >> soldiers_cnt;
    std::vector<long long> values(soldiers_cnt, 0);
    for (size_t i = 0; i < soldiers_cnt; ++i) {
        std::cin >> values[i];
    }
    Sparse_table<long long, GCD<long long>> sparse_table(values);
    size_t left, right, request;
    std::cin >> request;
    for (size_t i = 0; i < request; ++i) {
        std::cin >> left >> right;
        std::cout << sparse_table.query(left - 1, right - 1) << '\n';
    }
    return 0;
}
