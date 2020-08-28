#include<bits/stdc++.h>
//#define int long long
#define p pair<int, int>
//#define endl '\n'
const int INF = 1000000001;

using namespace std;

p inp(){
    string s;
    cin >> s;
    if (s == "Skip"){
        return {-1, -1};
    }
    return {s[0]-'a'+1, s[1]-'0'};
}

vector<p> st = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
vector<vector<int>> pos;
vector<p> corners = {{1, 1}, {1, 8}, {8, 1}, {8, 8}};
double z;

vector<vector<int>> build(){
    vector<vector<int>> board(10, vector<int>(10, 2));
    board[4][4] = board[5][5] = 1, board[4][5] = board[5][4] = 0;
    pos.assign(10, vector<int>(10, 0));
    pos[1][1] = 500, pos[1][2] = pos[2][1] = -200, pos[2][2] = -300, pos[1][3] = pos[3][1] = 300, pos[3][3] = 150;
    pos[2][3] = pos[3][2] = -150, pos[3][4] = pos[4][3] = 100, pos[2][4] = pos[4][2] = -100, pos[1][4] = pos[4][1] = 200;
    for (int q = 1; q <= 4; q++){
        for (int q1 = 5; q1 <= 8; q1++){
            pos[q][q1] = pos[q][9-q1];
        }
        pos[9-q] = pos[q];
    }
    return board;
}

void print(p x){
    char row = 'a'+x.first-1;
    cout << row << x.second << endl;
}

vector<p> moves(int player, vector<vector<int>> &board){
    vector<p> move;
    for (int q = 1; q < 9; q++){
        for (int q1 = 1; q1 < 9; q1++){
            if (board[q][q1] == 2){
                for (p q2: st){
                    int q3 = q+q2.first, q4 = q1+q2.second;
                    while (board[q3][q4] == 1-player){
                        q3 += q2.first, q4 += q2.second;
                    }
                    if ((abs(q3-q) > 1 || abs(q4-q1) > 1) && board[q3][q4] == player){
                        move.push_back({q, q1});
                        break;
                    }
                }
            }
        }
    }
    return move;
}

bool is_end(vector<vector<int>> &board){
    vector<p> move;
    for (int q = 1; q < 9; q++){
        for (int q1 = 1; q1 < 9; q1++){
            if (board[q][q1] == 2){
                for (p q2: st){
                    int q3 = q+q2.first, q4 = q1+q2.second;
                    if (board[q3][q4] == 2){
                        continue;
                    }
                    int player = 1-board[q3][q4];
                    while (board[q3][q4] == 1-player){
                        q3 += q2.first, q4 += q2.second;
                    }
                    if ((abs(q3-q) > 1 || abs(q4-q1) > 1) && board[q3][q4] == player){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

p stable(int now, vector<vector<int>> &board){
    vector<vector<int>> elem(10, vector<int>(10, 2));
    for (int player = 0; player < 2; player++){
        if (board[1][1] == player){
            int max1 = 1, q1 = 1;
            while (board[q1][max1] == player){
                elem[q1][max1++] = player;
            }
            q1++, max1--;
            while (max1 > 1){
                int q2 = 1;
                while (board[q1][q2] == player && q2 < max1){
                    elem[q1][q2++] = player;
                }
                max1 = q2-1, q1++;
            }
            max1 = 1, q1 = 1;
            while (board[max1][q1] == player){
                elem[max1++][q1] = player;
            }
            q1++, max1--;
            while (max1 > 1){
                int q2 = 1;
                while (board[q2][q1] == player && q2 < max1){
                    elem[q2++][q1] = player;
                }
                max1 = q2-1, q1++;
            }
        }
        if (board[1][8] == player){
            int max1 = 8, q1 = 1;
            while (board[q1][max1] == player){
                elem[q1][max1--] = player;
            }
            q1++, max1++;
            while (max1 < 8){
                int q2 = 8;
                while (board[q1][q2] == player && q2 > max1){
                    elem[q1][q2--] = player;
                }
                max1 = q2+1, q1++;
            }
            max1 = 1, q1 = 8;
            while (board[max1][q1] == player){
                elem[max1++][q1] = player;
            }
            q1--, max1--;
            while (max1 > 1){
                int q2 = 1;
                while (board[q2][q1] == player && q2 < max1){
                    elem[q2++][q1] = player;
                }
                max1 = q2-1, q1--;
            }
        }
        if (board[8][1] == player){
            int max1 = 1, q1 = 8;
            while (board[q1][max1] == player){
                elem[q1][max1++] = player;
            }
            q1--, max1--;
            while (max1 > 1){
                int q2 = 1;
                while (board[q1][q2] == player && q2 < max1){
                    elem[q1][q2++] = player;
                }
                max1 = q2-1, q1--;
            }
            max1 = 8, q1 = 1;
            while (board[max1][q1] == player){
                elem[max1--][q1] = player;
            }
            q1++, max1++;
            while (max1 < 8){
                int q2 = 8;
                while (board[q2][q1] == player && q2 > max1){
                    elem[q2--][q1] = player;
                }
                max1 = q2+1, q1++;
            }
        }
        if (board[8][8] == player){
            int max1 = 8, q1 = 8;
            while (board[q1][max1] == player){
                elem[q1][max1--] = player;
            }
            q1--, max1++;
            while (max1 < 8){
                int q2 = 8;
                while (board[q1][q2] == player && q2 > max1){
                    elem[q1][q2--] = player;
                }
                max1 = q2+1, q1--;
            }
            max1 = 8, q1 = 8;
            while (board[max1][q1] == player){
                elem[max1--][q1] = player;
            }
            q1--, max1++;
            while (max1 < 8){
                int q2 = 8;
                while (board[q2][q1] == player && q2 > max1){
                    elem[q2--][q1] = player;
                }
                max1 = q2+1, q1--;
            }
        }
    }
    p ans = {0, 0};
    for (int q = 1; q < 9; q++){
        for (int q1 = 1; q1 < 9; q1++){
            if (elem[q][q1] == now){
                ans.first++;
            }else if (elem[q][q1] == 1-now){
                ans.second++;
            }
        }
    }
    return ans;
}

int score_wall(int player, int past, vector<int> &a){
    if (a[1] != 2 || a[8] != 2){
        return 0;
    }
    int we = 0, other = 0;
    for (int q = 1; q < 9; q++){
        we += (a[q] == player), other += (a[q] == 1-player);
    }
    return (we == 0 ? (other == 0)-1 : other == 0);
}

vector<vector<int>> make(p x, int player, vector<vector<int>> board){
    int q = x.first, q1 = x.second;
    board[x.first][x.second] = player;
    for (p q2: st){
        int q3 = q+q2.first, q4 = q1+q2.second;
        for (; board[q3][q4] == 1-player; q3 += q2.first, q4 += q2.second){}
        if (abs(q3-q)+abs(q4-q1) > 1 && board[q3][q4] == player){
            for (; q3 != q || q4 != q1; q3 -= q2.first, q4 -= q2.second){
                board[q3][q4] = player;
            }
        }
    }
    return board;
}

int score(int player, int past, vector<vector<int>> &board){
    int we = 0, other = 0;
    for (int q = 1; q < 9; q++){
        for (int q1 = 1; q1 < 9; q1++){
            we += (board[q][q1] == player), other += (board[q][q1] == 1-player);
        }
    }
    if (is_end(board)){
        return (we > other ? 1000000 : we == other ? 0 : -1000000)+we;
    }
    p quan = stable(player, board);
    vector<int> will, will1;
    for (int q = 0; q < 10; q++){
        will.push_back(board[q][1]);
        will1.push_back(board[q][8]);
    }
    int w = score_wall(player, past, board[1]), w1 = score_wall(player, past, board[8]), w2 = score_wall(player, past, will), w3 = score_wall(player, past, will1);
    int corner1 = (board[1][1] == player)+(board[1][8] == player)+(board[8][1] == player)+(board[8][8] == player);
    int corner2 = (board[1][1] == 1-player)+(board[1][8] == 1-player)+(board[8][1] == 1-player)+(board[8][8] == 1-player);
    int wall1 = (w == 1)+(w1 == 1)+(w2 == 1)+(w3 == 1), wall2 = (w == -1)+(w1 == -1)+(w2 == -1)+(w3 == -1);
    int ans = (corner1 == 0 ? -corner2*20000 : corner2 == 0 ? corner1*20000 : corner1*10000-corner2*10000);
    ans += wall1*3000-wall2*3000+quan.first*1000-quan.second*1000+we-other;
    return ans;
}

int alphabeta(int player, int who, int h, vector<vector<int>> &board, int alpha, int beta){
    if ((clock()-z)/CLOCKS_PER_SEC > 1.9){
        return INF;
    }
    if (h == 0){
        return score(who, 1-player, board);
    }
    vector<p> move = moves(player, board);
    if (move.empty()){
        if (is_end(board)){
            return score(who, 1-player, board);
        }
        return alphabeta(1-player, who, h-1, board, alpha, beta);
    }
    int ans;
    if (player == who) {
        ans = -INF;
        for (p q: move) {
            auto new_board = make(q, player, board);
            int res = alphabeta(1-player, who, h-1, new_board, max(alpha, ans), beta);
            if (res == INF){
                return INF;
            }
            ans = max(ans, res);
            if (ans > beta){
                return ans;
            }
        }
    }else{
        ans = INF;
        for (p q: move) {
            auto new_board = make(q, player, board);
            int res = alphabeta(1-player, who, h-1, new_board, alpha, min(beta, ans));
            if (res == INF){
                return INF;
            }
            ans = min(ans, res);
            if (ans < alpha){
                return ans;
            }
        }
    }
    return ans;
}

void choice(int player, vector<vector<int>> &board){
    vector<p> move = moves(player, board);
    if (move.empty()){
        cout << "Skip" << endl;
    }else {
        p max_max = {-1, -1};
        int q1 = 4;
        bool flag = true;
        while (flag) {
            pair<int, p > max1 = {-INF, {-1, -1}};
            for (p q: move) {
                vector<vector<int>> new_board = make(q, player, board);
                int res = alphabeta(1 - player, player, q1, new_board, -INF, INF);
                if (res == INF){
                    flag = false;
                    break;
                }
                max1 = max(max1, {res, q});
            }
            if (flag){
                max_max = max1.second, q1++;
            }
        }
        print(max_max);
        cerr << (clock()-z)/CLOCKS_PER_SEC << endl;
        board = make(max_max, player, board);
    }
    if (is_end(board)){
        return;
    }
    p now = inp();
    z = clock();
    if (now.first == -1){
        choice(player, board);
    }else{
        board = make(now, 1-player, board);
        if (is_end(board)){
            return;
        }
        choice(player, board);
    }
}

signed main(){
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);
    //cout.tie(nullptr);
    cerr << setprecision(17) << fixed;
    int player;
    cin >> player;
    auto board = build();
    if (player == 2){
        p now = inp();
        if (now.first != -1) {
            board = make(now, 0, board);
        }
    }
    z = clock();
    choice(player-1, board);
    return 0;
}
