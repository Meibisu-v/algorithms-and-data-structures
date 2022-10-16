#include <iostream>
#include <vector>

int next_point(int dist){
    if (dist <= 1) return 0;
    return (dist / 2) + (dist % 2);
}
void merge_array(int* first,int* second, int n, int m){
    int min_arr = std::min(n, m);
    for (int i = 0; i < min_arr; i++) {
        if (first[n - i - 1] > second[i])
            std::swap(first[n - i - 1], second[i]);

    }
    for (int dist = next_point(n); dist > 0; dist = next_point(dist)){
        for (int i = 0; i + dist < n; i++)
            if (first[i] > first[i + dist])
                std::swap(first[i], first[i + dist]);
    }
    for (int dist = next_point(m); dist > 0; dist = next_point(dist)) {
        for (int i = 0; i + dist < m; i++)
            if (second[i] > second[i + dist])
                std::swap(second[i], second[i + dist]);
    }
}


struct pair{
    int beg_, end_;
};

int main(){
    int n, k;
    std::cin >> k >> n;
    int* first_arr = new int[n];
    std::vector<pair> array_ind;
    array_ind.reserve(k);
    int size_mas, curr_size = n;
    for (int i = 0; i < n; ++i){
        std::cin >> first_arr[i];
    }
    array_ind.push_back({0, n - 1});
    for (int t = 1; t < k; ++t){
        std::cin >> size_mas;
        int temp;
        int *new_arr = new int[curr_size + size_mas];
        for (int i = 0; i < curr_size; ++i)
            new_arr[i] = first_arr[i];
        for (int i = curr_size; i < curr_size + size_mas; ++i){
            std::cin >> new_arr[i];
        }
        array_ind.push_back({curr_size, curr_size + size_mas - 1});
        //merge_array(new_arr, &new_arr[curr_size], curr_size, size_mas);
        curr_size += size_mas;
        std::swap(first_arr, new_arr);
        delete[]new_arr;
    }
    int curr = 0;
    int len1, len2, next;
    while (array_ind.size() > 1){
        if (curr + 1 >= array_ind.size()) {
            curr = 0;
            next = 1;
        } else next = curr + 1;
        len1 = array_ind[curr].end_ - array_ind[curr].beg_ + 1;
        len2 = array_ind[next].end_ - array_ind[next].beg_ + 1;
        merge_array(&first_arr[array_ind[curr].beg_], &first_arr[array_ind[next].beg_], len1, len2);
        array_ind[next].beg_ = array_ind[curr].beg_;
        array_ind.erase(array_ind.begin() + curr);
        (++curr) % array_ind.size();
    }
    for (int i = 0; i < curr_size; ++i){
        std::cout << first_arr[i] << ' ';
    }
    delete[]first_arr;
    return 0;
}
