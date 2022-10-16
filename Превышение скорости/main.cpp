#include <iostream>
#include <vector>


int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int n;
    std::cin >> n;

    std::vector<int> speed, dist;
    speed.reserve(n);
    dist.reserve(n);
    int temp;

    for (int i = 0; i < n; ++i){
        std::cin >> temp;
        speed.push_back(temp);
    }
    for (int i = 0; i < n; ++i){
        std::cin >> temp;
        dist.push_back(temp);
    }

    int m;
    std::cin >> m;
    std::vector<int> border(m), fine(m + 1);

    for (int i = 1; i < m; ++i){
        std::cin >> border[i];
    }
    for (int i = 1; i < m + 1; ++i){
        std::cin >> fine[i];
    }

    int q;
    std::cin >> q;

    int s, t;
    int left, right, mid;
    double speed_curr;
    for (int i = 0; i < q; ++i){
        std::cin >> s >> t;
        left = -1;
        right = m;
        while (right - left > 1){
            speed_curr = 0;
            mid = (right + left) / 2;
            for (int j = 0; j < n; ++j){
                speed_curr += 1.0 * dist[j] / (speed[j] + border[mid]);
            }
            if (speed_curr <= t - s){
                right = mid;
            } else {
                left = mid;
            }
        }
        std::cout << fine[right] << '\n';
    }

    return 0;
}
