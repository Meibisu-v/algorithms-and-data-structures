#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>


bool compare_grades(const std::pair<std::string, int>& lv,
                    const std::pair<std::string, int>& rv){
    return lv.second > rv.second;
}


template<class Iterator>
void inplace_mrg(Iterator begin_it, Iterator middle, Iterator end_it) {

    auto end_1 = middle;
    auto beg_2 = middle;

    if (beg_2 == end_it|| begin_it == end_1 || !compare_grades(*beg_2, *(end_1 - 1))) {
        return;
    }

    if (end_1 - begin_it >= end_it - beg_2) {

        auto mid = begin_it + (end_1 - begin_it) / 2;

        auto pivot = std::lower_bound(beg_2, end_it, *mid, compare_grades);
        auto it = std::rotate(mid, beg_2, pivot);

        inplace_mrg(begin_it, mid, it);
        inplace_mrg(it, pivot, end_it);
    } else {

        auto mid = beg_2 + (end_it - beg_2) / 2;

        auto pivot = std::upper_bound(begin_it, end_1, *mid, compare_grades);
        auto it = std::rotate(pivot, beg_2, mid);

        inplace_mrg(begin_it, pivot, it);
        inplace_mrg(it, mid, end_it);
    }
}


template<class Iterator>
void merge_sort(Iterator begin_it, Iterator end_it) {

    if (end_it == begin_it + 1) {
        return;
    }

    auto mid = begin_it + (end_it - begin_it) / 2;

    merge_sort(begin_it, mid);
    merge_sort(mid, end_it);

    inplace_mrg(begin_it, mid, end_it);
}


int main() {
    int n;
    std::string name, surname;
    std::vector<std::pair<std::string, int>> stud_raiting;
    std::cin >> n;
    int inf, math, rus;
    stud_raiting.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> name >> surname;
        std::cin >> inf >> math >> rus;
        stud_raiting.emplace_back(name + ' ' + surname, inf + math + rus);
    }

    merge_sort(stud_raiting.begin(), stud_raiting.end());
    for (const auto& i : stud_raiting){
        std::cout << i.first << '\n';
    }
    return 0;
}
