#include <iostream>
#include <vector>

struct Point {
    int64_t x;
    int64_t y;
    int64_t z;
    Point(int64_t x, int64_t y, int64_t z) : x(x), y(y), z(z) {}
};

template<typename T>
class FenwickTree_3D {
private:
    std::vector<std::vector<std::vector<T>>> tree_;
    T RSQ(const Point &point) const;
public:
    FenwickTree_3D(size_t height, size_t length, size_t width);
    T RSQ(const Point &lhs, const Point &rhs) const;
    void Update(const Point &point, const T &value);
    size_t GetWidth() const {
        return tree_[0][0].size();
    }
    size_t GetHeight() const {
        return tree_.size();
    }
    size_t GetLength() const {
        return tree_[0].size();
    }
};

template<typename T>
FenwickTree_3D<T>::FenwickTree_3D(size_t height, size_t length, size_t width)
        :tree_(height, std::vector<std::vector<T>>(length, std::vector<T>(width))) {};


template <typename T>
void FenwickTree_3D<T>::Update(const Point &point, const T &value) {
    for (size_t i = point.x; i < GetHeight(); i = (i | (i + 1))) {
        for (size_t j = point.y; j < GetLength(); j = (j | (j + 1))) {
            for (size_t k = point.z; k < GetWidth(); k = (k | (k + 1))) {
                tree_[i][j][k] += value;
            }
        }
    }
}

template <typename T>
T FenwickTree_3D<T>::RSQ(const Point &point) const {
    T result = T(0);
    for (int64_t i = point.x; i >= 0; i = ((i & (i + 1)) - 1)) {
        for (int64_t j = point.y; j >= 0; j = ((j & (j + 1)) - 1)) {
            for (int64_t k = point.z; k >= 0; k = ((k & (k + 1))-1)) {
                result += tree_[i][j][k];
            }
        }
    }
    return result;
}

template<typename T>
T FenwickTree_3D<T>::RSQ(const Point &lhs, const Point &rhs) const {
  return RSQ(Point(rhs.x, rhs.y, rhs.z)) - RSQ(Point(lhs.x - 1, rhs.y, rhs.z)) -
         RSQ(Point(rhs.x, lhs.y - 1, rhs.z)) - RSQ(Point(rhs.x, rhs.y, lhs.z - 1)) +
         RSQ(Point(lhs.x - 1, lhs.y - 1, rhs.z)) + RSQ(Point(lhs.x - 1, rhs.y, lhs.z - 1)) +
         RSQ(Point(rhs.x, lhs.y - 1, lhs.z - 1)) - RSQ(Point(lhs.x - 1, lhs.y - 1, lhs.z - 1));
}

int main() {
  size_t n;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> n;
  FenwickTree_3D<int64_t> fenwick_tree(n, n, n);
  int query;
  std::cin >> query;
  while (query != 3) {
    if (query == 1) {
      int64_t x, y, z;
      int64_t value;
      std::cin >> x >> y >> z >> value;
      fenwick_tree.Update(Point(x, y, z), value);
    } else if (query == 2) {
      int64_t x1, y1, z1, x2, y2, z2;
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      std::cout << fenwick_tree.RSQ(Point(x1, y1, z1), Point(x2, y2, z2)) << '\n';
    }
    std::cin >> query;
  }

  return 0;
}
