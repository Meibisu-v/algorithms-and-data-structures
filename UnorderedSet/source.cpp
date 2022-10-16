#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>
#include <list>
#include <string>
#include <algorithm>

#define FACTOR 2
#define DEFAULT_SIZE 100


template <typename T>
class hash_table{
private:

    std::vector<std::list<T>> table;
    size_t size_table = 0;;
    size_t elem_count = 0;

public:
    hash_table(const size_t &size_){
        table.reserve(size_);
        size_table = size_;
        std::list<T> new_list;
        for (size_t i = 0; i < size_table; ++i){
            table.push_back(new_list);
        }
    }
    ~hash_table(){
        table.clear();
        elem_count = 0;
        size_table = 0;
    }
    void insert_t(const T &elem){
        if (elem_count + 1 >= size_table) rehash();
        size_t index = std::hash<T>{}(elem) % size_table;

        ++elem_count;
        table[index].push_back(elem);
    }

    bool erase_t(const T& elem){
        size_t index = std::hash<T>{}(elem) % size_table;
        if (table[index].empty()) return false;
        table[index].remove(elem);
        --elem_count;
        return true;
    }

    bool find_t(const T& elem){
        size_t index = std::hash<T>{}(elem) % size_table;
        const auto& list_ = table[index];
        const auto& find_it = std::find(list_.begin(), list_.end(), elem);
        return find_it != list_.end();
    }

    void rehash(){
        size_t new_size = size_table * FACTOR;
        std::vector<std::list<T>> new_table;

        new_table.reserve(new_size);
        std::list<T> new_list;

        for (size_t i = 0; i < new_size; ++i){
            new_table.push_back(new_list);
        }


        for (size_t index = 0; index < size_table; ++index){
            auto it = table[index].begin();
            while (it != table[index].end()){
                size_t new_index = std::hash<T>{}(*it) % new_size;
                new_table[new_index].push_back(*it);
                ++it;
            }
        }
    
        table.resize(new_size);
        std::swap(table, new_table);
        size_table = new_size;
    }
};



int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    hash_table<std::string> table(DEFAULT_SIZE);
    char sign;
    std::cin >> sign;
    while (sign != '#'){
        std::string value;
        std::cin >> value;

        if (sign == '+'){
            table.insert_t(value);
        }
        if (sign == '?'){
            if (table.find_t(value)) std::cout << "YES\n";
                else std::cout << "NO\n";
        }
        if (sign == '-'){
            table.erase_t(value);
        }
        std::cin >> sign;
    }
    return 0;
}
