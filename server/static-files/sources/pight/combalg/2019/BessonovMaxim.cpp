#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include <stdlib.h>
#define INF 1E9

using namespace std;

struct piece{
    char direct;
    char speed;
    pair<int, int> coords;
    bool is_appled;
};
int amount1 = 0, amount2 = 0;
const int n = 30, m = 40;
vector <pair<int, int>> apple, apple1;
vector <piece> s1, s2;
vector <vector <int>> f(n, vector <int> (m, 0));
vector <vector <pair<int, int>>> gr(n * m);

void init(){
    int tmp1;
    string str;
    for(int i = 0; i < n; i++){
        cin >> str;
        for(int j = 0; j < m; j++){
            tmp1 = ((int)str[j] - (int)('0'));
            if(tmp1){
                if(i == 0 || i == n - 1 || j == 0 || j == m - 1){
                    f[i][j] = tmp1 - 1;
                }
                else{
                    f[i][j] = tmp1;
                    apple.push_back({i,j});
                }
                apple1.push_back({i, j});
            }
            else
                f[i][j] = tmp1;
        }
    }
    // 2 - עוכמ םארוי חלוט s1
    // 3 - דמכמגא םארוי חלוט s1
    // 4 - עוכמ םוםארוי חלוט s2
    // 5 - דמכמגא םוםאשוי חלוט s2
    //f[3][0] = 3;
    //f[2][0] = 2;
    //f[1][0] = 2;
    //f[0][0] = 2;
    piece tmp;
    tmp.coords = {3,0};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'D';
    s1.push_back(tmp);
    tmp.coords = {2,0};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'D';
    s1.push_back(tmp);
    tmp.coords = {1,0};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'D';
    s1.push_back(tmp);
    tmp.coords = {0,0};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'D';
    s1.push_back(tmp);
    //f[26][39] = 5;
    //f[27][39] = 4;
    //f[28][39] = 4;
    //f[29][39] = 4;
    tmp.coords = {26, 39};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'U';
    s2.push_back(tmp);
    tmp.coords = {27, 39};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'U';
    s2.push_back(tmp);
    tmp.coords = {28, 39};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'U';
    s2.push_back(tmp);
    tmp.coords = {29, 39};
    tmp.is_appled = false;
    tmp.speed = 'F';
    tmp.direct = 'U';
    s2.push_back(tmp);
}

void assemble(){
    for(int i = 0; i < gr.size(); i++)
            gr[i].clear();
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            f[i][j] = 0;
        }
    }
    for(int i = 0; i < s1.size(); i++){
        if(!i){
            f[s1[i].coords.first][s1[i].coords.second] = 3;
        }
        else
            f[s1[i].coords.first][s1[i].coords.second] = 2;
    }
    for(int i = 0; i < s2.size(); i++){
        if(!i){
            f[s2[i].coords.first][s2[i].coords.second] = 5;
        }
        else
            f[s2[i].coords.first][s2[i].coords.second] = 4;
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(j > 0){
                if(f[i][j - 1] == 0)
                    gr[i * m + j].push_back({i * m + j - 1, 1});
                else
                    gr[i * m + j].push_back({i * m + j - 1, INF});
            }
            if(i > 0){
                if(f[i-1][j] == 0)
                    gr[i * m + j].push_back({(i-1)*m + j, 1});
                else
                    gr[i * m + j].push_back({(i-1)*m + j, INF});
            }
            if(j < m - 1){
                if(f[i][j + 1] == 0)
                    gr[i * m + j].push_back({i * m + j + 1, 1});
                else
                    gr[i * m + j].push_back({i * m + j + 1, INF});
            }
            if(i < n - 1){
                if(f[i + 1][j] == 0)
                    gr[i * m + j].push_back({(i+1)*m + j, 1});
                else
                    gr[i * m + j].push_back({(i+1)*m + j, INF});
            }
        }
    }
}
/*
char rand_move(int turn){
    vector <char> obosralsya;
    piece top = s1[0];
}*/

char free_move(piece s){
    assemble();
    int x = s.coords.first;
    int y = s.coords.second;
    char move;
    if(s.direct == 'U'){
        if(x > 0)
            if(f[x - 1][y] == 0){
                return 'F';
            }
        if(y > 0)
            if(f[x][y - 1] == 0){
                return 'L';
            }
        if(y < m - 1)
            if(f[x][y + 1] == 0){
                return 'R';
            }
    }
    else if(s.direct == 'D'){
        if(x < n - 1)
            if(f[x + 1][y] == 0){
                return 'F';
            }
        if(y > 0)
            if(f[x][y - 1] == 0){
                return 'R';
            }
        if(y < m - 1)
            if(f[x][y + 1] == 0){
                return 'L';
            }
    }
    else if(s.direct == 'L'){
        if(x > 0)
            if(f[x - 1][y] == 0){
                return 'R';
            }
        if(y > 0)
            if(f[x][y - 1] == 0){
                return 'F';
            }
        if(x < n - 1)
            if(f[x + 1][y] == 0){
                return 'L';
            }
    }
    else if(s.direct == 'R'){
        if(x > 0)
            if(f[x - 1][y] == 0){
                return 'L';
            }
        if(y < m - 1)
            if(f[x][y + 1] == 0){
                return 'F';
            }
        if(x < n - 1)
            if(f[x + 1][y] == 0){
                return 'R';
            }
    }
}

char new_move(int s, int f, piece top){
    assemble();
    /*piece top;
    if(turn == 1){
        top = s1[0];
    }
    else
        top = s2[0];*/
    char move = 'F';
    vector <int> d(n*m, INF);
    d[s] = 0;
    set <pair<int, int>> unused;
    for(int i = 0; i < n*m; i++){
        unused.insert({d[i], i});
    }
    vector <int> p(n*m);
    while(!unused.empty()){
        int next = unused.begin()->second;
        unused.erase(unused.begin());
        for(pair <int, int> edge : gr[next]){
            int to = edge.first, w = edge.second;
            if(d[to] > d[next] + w){
                unused.erase({d[to], to});
                d[to] = d[next] + w;
                unused.insert({d[to], to});
                p[to] = next;
            }
        }
    }
    if(d[f] < 100){
        vector <int> v;
        int i = f;
        v.push_back(f);
        while(p[i] != s){
            v.push_back(p[i]);
            i = p[i];
        }
        v.push_back(s);
        reverse(v.begin(), v.end());
        int move1 = v[1];
        //cout << "NEW_MOVE: " << v[1] << " prev_s: " << s << endl;
        if(v[1] == v[0] + 1){
            if(top.direct == 'D')
                move = 'L';
            else if(top.direct == 'R')
                move = 'F';
            else
                move = 'R';
        }
        else if(v[1] == v[0] - 1){
            if(top.direct == 'D')
                move = 'R';
            else if(top.direct == 'L')
                move = 'F';
            else
                move = 'L';
        }
        else if(v[1] == v[0] + 40){
            if(top.direct == 'D')
                move = 'F';
            else if(top.direct == 'R')
                move = 'R';
            else
                move = 'L';
        }
        else{
            if(top.direct == 'U')
                move = 'F';
            else if(top.direct == 'R')
                move = 'L';
            else
                move = 'R';
        }
        /*cout << "WAY TO ELDORADO: ";
        for(int i = 0; i < v.size(); i++){
            cout << v[i] << " ";
        }
        cout << endl;*/
        return move;
    }
    else
        return free_move(top);
}

void print_field(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            f[i][j] = 0;
        }
    }
    //cout << "APPLEEES: " << endl;
    for(int i = 0; i < apple1.size(); i++){
        f[apple1[i].first][apple1[i].second] = 1;
        //cout << i << ": " << apple[i].first << " " << apple[i].second << endl;
    }
    for(int i = 0; i < s1.size(); i++){
        if(!i){
            f[s1[i].coords.first][s1[i].coords.second] = 3;
        }
        else
            f[s1[i].coords.first][s1[i].coords.second] = 2;
    }
    for(int i = 0; i < s2.size(); i++){
        if(!i){
            f[s2[i].coords.first][s2[i].coords.second] = 5;
        }
        else
            f[s2[i].coords.first][s2[i].coords.second] = 4;
    }
    cout << "------------" << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << f[i][j];
        }
        cout << endl;
    }
}

void move_forward(piece &a){
    if(a.direct == 'U')
        a.coords.first--;
    else if(a.direct == 'D')
        a.coords.first++;
    else if(a.direct == 'R')
        a.coords.second++;
    else
        a.coords.second--;
}

void move_right(piece &a){
    if(a.direct == 'U'){
        a.coords.second++;
        a.direct = 'R';
    }
    else if(a.direct == 'D'){
        a.coords.second--;
        a.direct = 'L';
    }
    else if(a.direct == 'R'){
        a.coords.first++;
        a.direct = 'D';
    }
    else{
        a.coords.first--;
        a.direct = 'U';
    }
}

void move_left(piece &a){
    if(a.direct == 'U'){
        a.coords.second--;
        a.direct = 'L';
    }
    else if(a.direct == 'D'){
        a.coords.second++;
        a.direct = 'R';
    }
    else if(a.direct == 'R'){
        a.coords.first--;
        a.direct = 'U';
    }
    else{
        a.coords.first++;
        a.direct = 'D';
    }
}

int is_appled(piece a){
    for(int i = 0; i < apple1.size(); i++){
        if((a.coords.first == apple1[i].first) && (a.coords.second == apple1[i].second)){
            return i;
        }
    }
    return -1;
}

void move(int turn, char speed){
    if(turn == 1){
        //cout << "APPLE IS HERERERERERERRERERER - > "  << is_appled(s1[0]) << " tut lico -> " << s1[0].coords.first << " " << s1[0].coords.second<< endl;

        if(!amount1){
            s1[0].speed = speed;
        }
        else{
            for(int i = s1.size() - 1; i > 0; i--){
                s1[i].speed = s1[i - 1].speed;
            }
            s1[0].speed = speed;
        }
        /*cout <<"DEBUG 11111" << endl;
        for(int j = 0; j < s1.size(); j++){
            cout << j << ": "<< s1[j].is_appled  << " speed: " << s1[j].speed << " direct: " << s1[j].direct<< endl;
        }
        cout << "END DEBUG 1"<<endl;*/
        amount1++;
        int last_direct = s1[s1.size() - 1].direct;
        //f[s1[s1.size()-1].coords.first][s1[s1.size()-1].coords.second] = 0;
        for(int i = 0; i < s1.size(); i++){
            if(s1[i].speed == 'F')
                move_forward(s1[i]);
            else if(s1[i].speed == 'L')
                move_left(s1[i]);
            else
                move_right(s1[i]);
        }
        if(s1[s1.size() - 1].is_appled){
            //cout << "SOSI UROD" << endl;
            piece tmp, last = s1[s1.size() - 1];
            if(last.direct == 'U'){
                tmp.coords.first = last.coords.first + 1;
                tmp.coords.second = last.coords.second;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else if(last.direct == 'D'){
                tmp.coords.first = last.coords.first - 1;
                tmp.coords.second = last.coords.second;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else if(last.direct == 'R'){
                tmp.coords.first = last.coords.first;
                tmp.coords.second = last.coords.second - 1;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else{
                tmp.coords.first = last.coords.first;
                tmp.coords.second = last.coords.second + 1;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            tmp.is_appled = false;
            s1[s1.size() - 1].is_appled = false;
            s1.push_back(tmp);
            for(int i = s1.size() - 1; i > 0; i--){
                s1[i].is_appled = s1[i-1].is_appled;
                    //s1[i].is_appled = -1;
            }
            //cout << endl << "NEW PIECE ADDED: "<< tmp.direct << " sppeeedd: " << tmp.speed << endl;
        }
        else{
            //cout << "VSTAVITb TEKST" << endl;
            for(int i = s1.size() - 1; i > 0; i--){
                s1[i].is_appled = s1[i-1].is_appled;
                    //s1[i].is_appled = -1;
            }
            //s1[0].is_appled = -1;
        }
        if(is_appled(s1[0]) > -1){
            s1[0].is_appled = true;
            for(int i = 0; i < apple1.size(); i++){
                if((apple1[i].first == s1[0].coords.first) && (apple1[i].second == s1[0].coords.second)){
                    apple1.erase(apple1.begin() + i, apple1.begin() + i + 1);
                }
            }
            for(int i = 0; i < apple.size(); i++){
                if((apple[i].first == s1[0].coords.first) && (apple[i].second == s1[0].coords.second)){
                    apple.erase(apple.begin() + i, apple.begin() + i + 1);
                }
            }
        }
        else
            s1[0].is_appled = false;
    }
    else{
        if(!amount2){
            s2[0].speed = speed;
        }
        else{
            for(int i = s2.size() - 1; i > 0; i--){
                s2[i].speed = s2[i - 1].speed;
            }
            s2[0].speed = speed;
        }
        /*cout <<"DEBUG 11111" << endl;
        for(int j = 0; j < s1.size(); j++){
            cout << j << ": "<< s1[j].is_appled  << " speed: " << s1[j].speed << " direct: " << s1[j].direct<< endl;
        }
        cout << "END DEBUG 1"<<endl;*/
        amount2++;
        int last_direct = s2[s2.size() - 1].direct;
        //f[s1[s1.size()-1].coords.first][s1[s1.size()-1].coords.second] = 0;
        for(int i = 0; i < s2.size(); i++){
            if(s2[i].speed == 'F')
                move_forward(s2[i]);
            else if(s2[i].speed == 'L')
                move_left(s2[i]);
            else
                move_right(s2[i]);
        }
        if(s2[s2.size() - 1].is_appled){
            //cout << "SOSI UROD" << endl;
            piece tmp, last = s2[s2.size() - 1];
            if(last.direct == 'U'){
                tmp.coords.first = last.coords.first + 1;
                tmp.coords.second = last.coords.second;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else if(last.direct == 'D'){
                tmp.coords.first = last.coords.first - 1;
                tmp.coords.second = last.coords.second;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else if(last.direct == 'R'){
                tmp.coords.first = last.coords.first;
                tmp.coords.second = last.coords.second - 1;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            else{
                tmp.coords.first = last.coords.first;
                tmp.coords.second = last.coords.second + 1;
                tmp.direct = last_direct;
                tmp.speed = last.speed;
            }
            tmp.is_appled = false;
            s2[s2.size() - 1].is_appled = false;
            s2.push_back(tmp);
            for(int i = s2.size() - 1; i > 0; i--){
                s2[i].is_appled = s2[i-1].is_appled;
                    //s1[i].is_appled = -1;
            }
            //cout << endl << "NEW PIECE ADDED: "<< tmp.direct << " sppeeedd: " << tmp.speed << endl;
        }
        else{
            //cout << "VSTAVITb TEKST" << endl;
            for(int i = s2.size() - 1; i > 0; i--){
                s2[i].is_appled = s2[i-1].is_appled;
                    //s1[i].is_appled = -1;
            }
            //s1[0].is_appled = -1;
        }
        if(is_appled(s2[0]) > -1){
            s2[0].is_appled = true;
            for(int i = 0; i < apple1.size(); i++){
                if((apple1[i].first == s2[0].coords.first) && (apple1[i].second == s2[0].coords.second)){
                    apple1.erase(apple1.begin() + i, apple1.begin() + i + 1);
                }
            }
            for(int i = 0; i < apple.size(); i++){
                if((apple[i].first == s2[0].coords.first) && (apple[i].second == s2[0].coords.second)){
                    apple.erase(apple.begin() + i, apple.begin() + i + 1);
                }
            }
        }
        else
            s2[0].is_appled = false;
    }
}

double dist(pair<int,int> a, pair<int, int> b){
    return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second));
}

int nearest_apple(piece top, bool flag){
    double min_dist = 1E9;
    int min_i = -1;
    if(!flag){
        for(int i = 0; i < apple.size(); i++){
            if(dist(top.coords, apple[i]) < min_dist){
                min_dist = dist(top.coords, apple[i]);
                min_i = i;
            }
        }
    }
    else{
        for(int i = 0; i < apple1.size(); i++){
            if(dist(top.coords, apple1[i]) < min_dist){
                min_dist = dist(top.coords, apple1[i]);
                min_i = i;
            }
        }
    }
    return min_i;
}

void graph_debug(){
    cout << "GRAPH DEBUG" << endl;
        for(int i = 0; i < gr.size(); i++){
            cout << i << ": ";
            for(int j = 0; j < gr[i].size(); j++){
                cout << gr[i][j].first << "," << gr[i][j].second<< ";";
            }
            cout << endl;
        }
    cout << "GRAPH DEBUG END" << endl;
}

int main(){
    //print_field();
    int k;
    //cin >> k;
    char tmp2;
    char tmp3;
    int turn;
    cin >> turn;
    init();
    for(int i = 0; i < 600; i++){
        //print_field();
        /*for(int j = 0; j < s1.size(); j++){
            cout << j << ": "<< s1[j].is_appled << endl;
        }*/
        //assemble();
        if(turn == 1){
            if(apple.size() > 0)
                tmp2 = new_move(s1[0].coords.first * m + s1[0].coords.second, apple[nearest_apple(s1[0], false)].first * m + apple[nearest_apple(s1[0], false)].second, s1[0]);
            else if(apple1.size() > 0)
                tmp2 = new_move(s1[0].coords.first * m + s1[0].coords.second, apple1[nearest_apple(s1[0], true)].first * m + apple1[nearest_apple(s1[0], true)].second, s1[0]);
            else
                tmp2 = free_move(s1[0]);
            move(1, tmp2);
            cout << tmp2 << endl;
            cin >> tmp3;
            move(2, tmp3);
            if(tmp3 == 't')
                graph_debug();
            //turn = (turn % 2 + 1);
        }
        if(turn == 2){
            cin >> tmp3;
            move(1, tmp3);
            if((apple.size() > 0))
                tmp2 = new_move(s2[0].coords.first * m + s2[0].coords.second, apple[nearest_apple(s2[0], false)].first * m + apple[nearest_apple(s2[0], false)].second, s2[0]);
            else if((apple1.size() > 0))
                tmp2 = new_move(s2[0].coords.first * m + s2[0].coords.second, apple1[nearest_apple(s2[0], true)].first * m + apple1[nearest_apple(s2[0], true)].second, s2[0]);
            else
                tmp2 = free_move(s2[0]);
            move(2, tmp2);
            cout << tmp2 << endl;
            //turn = (turn % 2 + 1);
        }
        //cin >> tmp3;
        //cout << "DLINA ZMEI: " << s1.size() << endl;
        //cout << "NEAREST APPLE IS......: " << apple[nearest_apple()].first << ";" << apple[nearest_apple()].second << endl;
        //move(1, tmp2);
        //move(2, tmp3);
        //cout << "doiufvoidufoi: " << gr.size() << endl;
        //gr.clear();
        //cout << "sdlhqivbadiv:      " << gr.size() << endl;
        //cin >> tmp3;
    }
    return 0;
}
