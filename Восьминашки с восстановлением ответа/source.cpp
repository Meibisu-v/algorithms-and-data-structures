#include <iostream>
#include <queue>
#include <map>
#include <string>

class CPuzzleVertex {
public:

    CPuzzleVertex();

    CPuzzleVertex( std::string status_, char move_, int position_ );

    std::string status;
    char move;
    int position;
};

class CPuzzle {
public:

    CPuzzle();

    bool Solved();

    void GetWay( std::map<std::string, char>& way );

    void Resolve();

private:

    int status[9];
    int position;
};

CPuzzleVertex::CPuzzleVertex() {}

CPuzzleVertex::CPuzzleVertex( std::string status_, char move_, int position_ ):
        status(status_), move( move_ ), position( position_ )
{}

CPuzzle::CPuzzle()
{
    for( int i = 0; i < 9; ++i ) {
        std::cin >> status[i];
        if( status[i] == 0 ){
            position = i;
        }
    }



    if ( !Solved() ) {
        std::cout << -1;
    } else {
        Resolve();
    }
}

bool CPuzzle::Solved() {
    int snack[9];
    for( int i = 0; i < 9; ++i ) {
        snack[i] = status[i];
    }
    std::swap(snack[3], snack[5]);
    int zero_position;
    for( int i = 0; i < 9; ++i ) {
        if ( snack[i] == 0 ) {
            zero_position = i;
        }
    }
    for( int i = zero_position; i < 8; ++i) {
        snack[i] = snack[i + 1];
    }
    int inversion = 0; 
    for( int i = 0; i < 8; ++i ) {
        for( int j = i; j < 8; ++j ) {
            if( snack[i] > snack[j] ) {
                inversion++;
            }
        }
    }
    
    if( inversion % 2 == 0 ) {
        return false;
    } else {
        return true;
    }
}

void CPuzzle::GetWay( std::map<std::string, char>& way )
{
    std::string answer = ""; 
    std::map<std::string, char>::iterator iterator = way.find("123456780");
    while( iterator->second != 'S' ) {

        std::string tmp_status = iterator->first;
        answer += iterator->second;

        if( iterator->second == 'R' ) {
            int position = tmp_status.find('0');
            std::swap( tmp_status[position], tmp_status[position - 1] );
        }

        if( iterator->second == 'L' ) {
            int position = tmp_status.find('0');
            std::swap( tmp_status[position], tmp_status[position + 1] );
        }

        if( iterator->second == 'U' ) {
            int position = tmp_status.find('0');
            std::swap( tmp_status[position], tmp_status[position + 3] );
        }

        if( iterator->second == 'D' ) {
            int position = tmp_status.find('0');
            std::swap( tmp_status[position], tmp_status[position - 3] );
        }

        iterator = way.find(tmp_status);
    }

    for( int i = 0; i < answer.length() / 2; ++i ){
        std::swap( answer[i], answer[answer.length() - i - 1] );
    }

    std::cout << answer.length() << std::endl << answer;
}

void CPuzzle::Resolve()
{
    std::queue<CPuzzleVertex> puzzles; 
    std::map<std::string, char> visited; 
    std::string status_str;
    for( int i = 0; i < 9; ++i ) {
        status_str += std::to_string(status[i]);
    }
    CPuzzleVertex start( status_str, 'S', position);
    puzzles.push(start);
    visited.insert(std::make_pair(status_str, 'S'));

    CPuzzleVertex tmp; 

    bool finish = false; 

    if( status_str == "123456780" ) {
        std::cout << -1;
    }

    while( !finish ) {

        tmp = puzzles.front();
        puzzles.pop();

        if( tmp.status == "123456780" ) {
            GetWay( visited );
            break;
        }

        if( tmp.position == 0 || tmp.position == 3 || tmp.position == 6 || tmp.position == 1 || tmp.position == 4 || tmp.position == 7 ) {
            std::string status_tmp;
            status_tmp = tmp.status;
            std::swap(status_tmp[tmp.position], status_tmp[tmp.position + 1]);
            if( visited.find(status_tmp) == visited.end() ) {
                CPuzzleVertex vertex( status_tmp, 'R' , tmp.position + 1 );
                puzzles.push( vertex );
                visited.insert(std::make_pair(vertex.status, 'R'));
            }
        }

        if( tmp.position == 2 || tmp.position == 5 || tmp.position == 8 || tmp.position == 1 || tmp.position == 4 || tmp.position == 7 ) {

            std::string status_tmp;
            status_tmp = tmp.status;
            std::swap(status_tmp[tmp.position], status_tmp[tmp.position - 1]);
            if( visited.find(status_tmp) == visited.end() ) {
                CPuzzleVertex vertex( status_tmp, 'L' , tmp.position - 1 );
                puzzles.push( vertex );
                visited.insert(std::make_pair(vertex.status, 'L'));
            }
        }

        if( tmp.position == 6 || tmp.position == 7 || tmp.position == 8 || tmp.position == 3 || tmp.position == 4 || tmp.position == 5 ) {

            std::string status_tmp;
            status_tmp = tmp.status;
            std::swap(status_tmp[tmp.position], status_tmp[tmp.position - 3]);
            if( visited.find(status_tmp) == visited.end() ) {
                CPuzzleVertex vertex( status_tmp, 'U' , tmp.position - 3 );
                puzzles.push( vertex );
                visited.insert(std::make_pair(vertex.status, 'U'));
            }
        }

        if( tmp.position == 0 || tmp.position == 1 || tmp.position == 2 || tmp.position == 3 || tmp.position == 4 || tmp.position == 5 ) {
            std::string status_tmp;
            status_tmp = tmp.status;
            std::swap(status_tmp[tmp.position], status_tmp[tmp.position + 3]); 
            if( visited.find(status_tmp) == visited.end() ) {
                CPuzzleVertex vertex( status_tmp, 'D' , tmp.position + 3 );
                puzzles.push( vertex );
                visited.insert(std::make_pair(vertex.status, 'D'));
            }
        }
    }
}

int main() {
    CPuzzle n;
    return 0;
}