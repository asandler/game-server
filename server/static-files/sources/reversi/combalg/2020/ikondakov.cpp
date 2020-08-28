#include "list"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include "algorithm"
#include <iomanip>
#include "cmath"
#include "set"
#include <climits>

using namespace std;
vector<vector<int>> posval = {
        {100, 10, 10, 10, 10, 10, 10, 100},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {10, 1, 1, 1, 1, 1, 1, 10},
        {10, 1, 1, 1, 1, 1, 1, 10},
        {10, 1, 1, 1, 1, 1, 1, 10},
        {10, 1, 1, 1, 1, 1, 1, 10},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {100, 10, 10, 10, 10, 10, 10, 100}};
set<pair<int, int>> goodmoves(vector<vector<int>> &pole, int turn){
    set<pair<int,int>> turns;
    for (int y = 0; y < 8; ++y) {
        int lastzero = -1;
        bool goodvpposl = false;
        bool goodnzposl = false;
        for (int x = 1; x < 8; ++x) {
            if(pole[x-1][y] == 0 && pole[x][y] == -turn){
                lastzero = x-1;
                goodnzposl = true;
            }
            if(pole[x-1][y] == turn && pole[x][y] == -turn){
                goodvpposl = true;
            }
            if(goodnzposl && pole[x][y] == turn){
                turns.insert({lastzero, y});
            }
            if(goodvpposl && pole[x][y] == 0){
                turns.insert({x, y});
            }
            if(pole[x][y] != -turn){
                goodnzposl = false;
                goodvpposl = false;
            }
        }
    }
    for (int x = 0; x < 8; ++x) {
        int lastzero = -1;
        bool goodvpposl = false;
        bool goodnzposl = false;
        for (int y = 1; y < 8; ++y) {
            if(pole[x][y-1] == 0 && pole[x][y] == -turn){
                lastzero = y-1;
                goodnzposl = true;
            }
            if(pole[x][y-1] == turn && pole[x][y] == -turn){
                goodvpposl = true;
            }
            if(goodnzposl && pole[x][y] == turn){
                turns.insert({x, lastzero});
            }
            if(goodvpposl && pole[x][y] == 0){
                turns.insert({x, y});
            }
            if(pole[x][y] != -turn){
                goodnzposl = false;
                goodvpposl = false;
            }
        }
    }
    for (int xx = 0; xx < 15; ++xx) {
        int y = max(0, xx - 7);
        int x = min(xx, 7);
        pair<int,int> lastzero = {-1,-1};
        bool goodvpposl = false;
        bool goodnzposl = false;
        for (int k = 1; k <= min(x, 7-y); ++k) {
            if(pole[x-k+1][y+k-1] == 0 && pole[x-k][y+k] == -turn){
                lastzero = {x-k+1, y+k-1};
                goodnzposl = true;
            }
            if(pole[x-k+1][y+k-1] == turn && pole[x-k][y+k] == -turn){
                goodvpposl = true;
            }
            if(goodnzposl && pole[x-k][y+k] == turn){
                turns.insert(lastzero);
            }
            if(goodvpposl && pole[x-k][y+k] == 0){
                turns.insert({x-k, y+k});
            }
            if(pole[x-k][y+k] != -turn){
                goodnzposl = false;
                goodvpposl = false;
            }
        }
    }
    for (int xx = 0; xx < 15; ++xx) {
        int x = xx;
        int y = 0;
        if(xx > 7){
            y = xx - 7;
            x = 0;
        }
        pair<int,int> lastzero = {-1,-1};
        bool goodvpposl = false;
        bool goodnzposl = false;
        for (int k = 1; k <= min(7-x, 7-y); ++k) {
            if(pole[x+k-1][y+k-1] == 0 && pole[x+k][y+k] == -turn){
                lastzero = {x+k-1, y+k-1};
                goodnzposl = true;
            }
            if(pole[x+k-1][y+k-1] == turn && pole[x+k][y+k] == -turn){
                goodvpposl = true;
            }
            if(goodnzposl && pole[x+k][y+k] == turn){
                turns.insert(lastzero);
            }
            if(goodvpposl && pole[x+k][y+k] == 0){
                turns.insert({x+k, y+k});
            }
            if(pole[x+k][y+k] != -turn){
                goodnzposl = false;
                goodvpposl = false;
            }
        }
    }
    return turns;
}
int value(vector<vector<int>> &pole, int turn){
    int res = 0;
    int res2 = 0;
    int res3 = 0;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            res += pole[x][y] * posval[x][y];
            res2 += pole[x][y];
            res3 += abs(pole[x][y]);
        }
    }
    if(res3 > 50) return res2;
    return res - (int)goodmoves(pole, -turn).size() * 3;
}
pair<bool,vector<vector<int>>> mt(vector<vector<int>> &pole,int x, int y, int turn){
    bool g = false;
    vector<vector<int>> ppole = pole;
    ppole[x][y] = turn;
    for (int i = x + 1; i < 8; ++i) {
        if(pole[i][y] == 0)
            break;
        if(pole[i][y] == turn){
            for (int j = i - 1; j > x; --j) {
                ppole[j][y] = turn;
                g = true;
            }
            break;
        }
    }
    for (int i = y + 1; i < 8; ++i) {
        if(pole[x][i] == 0)
            break;
        if(pole[x][i] == turn){
            for (int j = i - 1; j > y; --j) {
                ppole[x][j] = turn;
                g = true;
            }
            break;
        }
    }
    for (int i = x - 1; i > -1; --i) {
        if(pole[i][y] == 0)
            break;
        if(pole[i][y] == turn){
            for (int j = i + 1; j < x; ++j) {
                g = true;
                ppole[j][y] = turn;
            }
            break;
        }
    }
    for (int i = y - 1; i > -1; --i) {
        if(pole[x][i] == 0)
            break;
        if(pole[x][i] == turn){
            for (int j = i + 1; j < y; ++j) {
                ppole[x][j] = turn;
                g = true;
            }
            break;
        }
    }
    for (int k = 1; k < 8 - max(x,y); ++k) {
        if(pole[x + k][y + k] == 0)
            break;
        if(pole[x + k][y + k] == turn){
            for (int j = k - 1; j > 0; --j) {
                ppole[x+j][y+j] = turn;
                g = true;
            }
            break;
        }
    }
    for (int k = 1; k <= min(x,y); ++k) {
        if(pole[x - k][y - k] == 0)
            break;
        if(pole[x - k][y - k] == turn){
            for (int j = k - 1; j > 0; --j) {
                g = true;
                ppole[x-j][y-j] = turn;
            }
            break;
        }
    }



    for (int k = 1; k < min(8-x,y+1); ++k) {
        if(pole[x + k][y - k] == 0)
            break;
        if(pole[x + k][y - k] == turn){
            for (int j = k - 1; j > 0; --j) {
                g = true;
                ppole[x+j][y-j] = turn;
            }
            break;
        }
    }
    for (int k = 1; k < min(x+1,8-y); ++k) {
        if(pole[x - k][y + k] == 0)
            break;
        if(pole[x - k][y + k] == turn){
            for (int j = k - 1; j > 0; --j) {
                ppole[x-j][y+j] = turn;
                g = true;
            }
            break;
        }
    }
    return {g, ppole};
};
pair<int,pair<int,int>> aturn(vector<vector<int>> pole,int turn,int depth = 0) {
    int zn = value(pole, turn) * turn;
    if(zn <= -12 and depth != 0)
        return {zn,{0,0}};
    if (depth == 6)
        return {zn,{0,0}};
    pair<int,int> coord = {-1,-1};
    int bestturn = -10000;
    for(auto i : goodmoves(pole, turn)){
        int x = i.first;
        int y = i.second;
        pair<bool,vector<vector<int>>> inform = mt(pole,x,y,turn);
        if(inform.first){
            pair<int,pair<int,int>> n = aturn(inform.second, turn * -1, depth + 1);
            if(-n.first > bestturn){
                bestturn = -n.first;
                coord = {x,y};
            }
        }
    }
    if(bestturn == -10000){
        bestturn = aturn(pole,-turn,depth+1).first;
    }
    return {bestturn,coord};
}

void print(vector<vector<int>> &pole){
    for (int i = 7; i > -1; --i) {
        for(auto j : pole[i]){
            if(j==1)
                cout<<'-';
            if(j==-1)
                cout<<'+';
            if(j==0)
                cout<<'=';
        }
        cout << endl;
    }
}
int main() {
    ios_base::sync_with_stdio(0);
    vector<vector<int>> pole(8,vector<int>(8,0));
    pole[3][3] = -1;
    pole[3][4] = 1;
    pole[4][4] = -1;
    pole[4][3] = 1;
    //goodmoves(pole,1);
    //print(pole);
    int turn = -1;
    int n;
    cin >> n;
    if(n == 2)
        n = 1;
    else if(n == 1)
        n = -1;
    long double time = 0;
    while(true){
        turn *= -1;
        int x, y;
        pair<int,pair<int,int>> a;
        if(turn != n) {
            time = clock();
            a = aturn(pole, turn);
            if(a.second == pair<int,int>{-1,-1}) {
                cout << "Skip" << endl;
                continue;
            }
            x = a.second.first;
            y = a.second.second;
            cout << char(y+'a') << x + 1 << endl;
            cerr << a.first << ' ' << (clock() - time)/ CLOCKS_PER_SEC << endl;
        }else {
            char a;
            cin >> a;
            if(a == 'S'){
                cin >> a >> a >> a;
                continue;
            }
            cin >> x;
            y = a - 'a';
            x -= 1;
        }
        pair<bool,vector<vector<int>>> t = mt(pole,x,y,turn);
        //print(t.second);
        pole = t.second;
    }
}
