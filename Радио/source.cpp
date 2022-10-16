#include <iostream>
#include <map>
#include <set>

#define MAX 20000000

class Radio{
private:
    std::map<int, std::pair<int, bool>, std::greater<int> > id_score; 
    std::map<int, std::set<int, std::greater<int> >, std::greater<int> > score_id; 
    int j;
public:
    Radio() : j(1) {};
    int push(int track_id, int score, int c = 0);
    void get();
    void out();
};

int Radio::push(int track_id, int score, int c) {
    auto &cur_score = id_score[track_id];
    if(score != 0 || c == -1) {
        if (cur_score.second) { 
            int k = cur_score.first;
            score_id[k].erase(track_id); 
            if (score_id[k].size() == 0) { 
                score_id.erase(k);
            }
        }
        if(c == -1) cur_score.first = c; 
        else cur_score.first += score;
        cur_score.second = 1; 
        score_id[id_score[track_id].first].insert(track_id); 
    } else{ 
        if(!cur_score.second){
            cur_score.second = 1; 
            score_id[id_score[track_id].first].insert(track_id); 
        }
    }
    return cur_score.first;
}

void Radio::get() {
    if((id_score.size() == 0 || score_id.begin()->first <= 0) && score_id.size() < MAX){ 
        while (id_score[j].first != 0 && j < MAX) ++j; 
        push(j, 0);
    }
    out();
}

void Radio::out() {
    int result1 = *score_id.begin()->second.rbegin(); 
    int result2 = score_id.begin()->first; 
    std::cout << result1 << ' ' << result2 << '\n';
    push(result1, 0, -1);
}

int main() {

    std::cin.tie(0);
    std::ios_base::sync_with_stdio(false);

    std::string s, ip;
    int track_id, time;
    int score;
    Radio r;
    
    std::map<std::string, std::pair<long long, bool> > ip_time;
    
    for(int i = 0, j = 1; s != "EXIT"; ++i){
        std::cin >> s;
        if(s == "VOTE"){
            std::cin >> ip >> track_id >> score >> time;
            int result;
            long long temp = ip_time[ip].first;
            auto &ip_time_it = ip_time[ip];
            if((temp == 0 && ip_time[ip].second == 0)|| temp + 600 <= time){
                result = r.push(track_id, score);
                ip_time_it.first = time; 
                ip_time_it.second = 1;
            } else{
                result = r.push(track_id, 0);
            }
            std::cout << result << '\n';
        }
        if(s == "GET"){
            r.get();
        }
    }
    std::cout << "OK";
    return 0;
}