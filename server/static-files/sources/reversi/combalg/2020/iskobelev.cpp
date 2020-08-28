#include <bits/stdc++.h>

using namespace std;

void view(int field[8][8]) {
    cout << "  1 2 3 4 5 6 7 8\n";
    for (int i = 0; i < 8; i++) {
        cout << char(i+97) << " ";
        for (int j = 0; j < 8; j++) {
            cout << field[i][j] << " ";
        }
        cout << "\n";
    }
}

void doTurn(int field[8][8], int side, set<pair<int, int>>& black, set<pair<int, int>>& white, string turn) {
    int x = turn[0] - 97;
    int y = turn[1] - 49;
    if (side == 1) black.insert(make_pair(x, y));
    else white.insert(make_pair(x, y));
    field[x][y] = side;
    for (int k = 0; k < 8; k++) {
        int i = x, j = y;
        bool flg = false;
        
        while(i < 8 && j < 8 && i >= 0 && j >= 0) {
            if (field[i][j] == side%2+1) flg = true;
            if (flg && field[i][j] == side && !(i == x && j == y)) {
                int a = x, b = y;
                while(!(a == i && b == j)) {
                    field[a][b] = side;
                    if (side == 1) {
                        black.insert(make_pair(a, b));
                        white.erase(make_pair(a, b));
                    } else {
                        white.insert(make_pair(a, b));
                        black.erase(make_pair(a, b));
                    }
                    if(k == 0) {a++; b++;}
                    else if(k == 1) {a++;}
                    else if(k == 2) {a++; b--;}
                    else if(k == 3) {b--;}
                    else if(k == 4) {a--; b--;}
                    else if(k == 5) {a--;}
                    else if(k == 6) {a--; b++;}
                    else if(k == 7) {b++;}
                }
                break;
            }
            if (!flg && field[i][j] == side && !(i == x && j == y)) break;
            if (field[i][j] == 0) break;
            
            if(k == 0) {i++; j++;}
            else if(k == 1) {i++;}
            else if(k == 2) {i++; j--;}
            else if(k == 3) {j--;}
            else if(k == 4) {i--; j--;}
            else if(k == 5) {i--;}
            else if(k == 6) {i--; j++;}
            else if(k == 7) {j++;}
        }
    }
    // cout << turn << "\n";
    // view(field);
} 

string p2str(pair<int, int> p) {
    string str = "aa";
    str[0] = p.first + 97;
    str[1] = p.second + 49;
    return str;
}

void existingTurns(int field[8][8], int side, set<pair<int, int>> chips, vector<pair<int, int>> & turns) {
    for (auto it : chips) {
        for (int k = 0; k < 8; k++) {
            int i = it.first, j = it.second;
            bool flg = false;
            while(i < 8 && j < 8 && i >= 0 && j >= 0) {
                if (field[i][j] == side%2+1) flg = true;
                if ((!flg && field[i][j] == 0) || (field[i][j] == side && !(i == it.first && j == it.second))) break;
                if (flg && field[i][j] == 0) {
                    turns.push_back(make_pair(i, j));
                    break;
                }
                if(k == 0) {i++; j++;}
                else if(k == 1) {i++;}
                else if(k == 2) {i++; j--;}
                else if(k == 3) {j--;}
                else if(k == 4) {i--; j--;}
                else if(k == 5) {i--;}
                else if(k == 6) {i--; j++;}
                else if(k == 7) {j++;}
            }
        }
    }
}

const int INF = 1e5;

bool isSafe(int field[8][8], int sq, int dir) {
    int sp = sq+dir;
    while (sp <= 49 && sq <= 49 && field[sp/8][sp%8] == field[sq/8][sq%8]) sp += dir;
    if (sp <= 49 && field[sp/8][sp%8] == 0) return true;
    int sn = sq - dir;
    while (sn <= 49 && sq <= 49 && field[sn/8][sn%8] == field[sq/8][sq%8]) sn -= dir;
    if (sn <= 49 && field[sn/8][sn%8] == 0) return true;
    return field[sp/8][sp%8] == field[sn/8][sn%8];
}

int score(int field[8][8], int side, set<pair<int, int>> black, set<pair<int, int>> white) {
    int sum = 0;
    // if (field[0][0] == side) sum += 50;
    // if (field[0][7] == side) sum += 50;
    // if (field[7][0] == side) sum += 50;
    // if (field[7][7] == side) sum += 50;
    
    // if (field[0][0] == side%2+1) sum -= 50;
    // if (field[0][7] == side%2+1) sum -= 50;
    // if (field[7][0] == side%2+1) sum -= 50;
    // if (field[7][7] == side%2+1) sum -= 50;
    
    for (int i = 0; i < 8; i+=7) {
        for (int j = 0; j < 8; j+=7) {
            int triangle[8] = {1};
            int blackEdges[4][2] = {0};
            int whiteEdges[4][2] = {0};
            for (int k = 0; k < 8; k++) {
                if (j == 0 && field[i][k] == 1 && blackEdges[0][i/8] == k) blackEdges[0][i/8]++;
                if (j == 0 && field[i][k] == 2 && whiteEdges[0][i/8] == -k) whiteEdges[0][i/8]--;

                if (j == 7 && field[i][7-k] == 1 && blackEdges[1][i/8] == k) blackEdges[1][i/8]++;
                if (j == 7 && field[i][7-k] == 2 && whiteEdges[1][i/8] == -k) whiteEdges[1][i/8]--;

                if (i == 0 && field[7-k][j] == 1 && blackEdges[2][j/8] == k) blackEdges[2][j/8]++;
                if (i == 0 && field[7-k][j] == 2 && blackEdges[2][j/8] == -k) whiteEdges[2][j/8]--;
                
                if (i == 7 && field[k][j] == 1 && blackEdges[3][j/8] == k) blackEdges[3][j/8]++;
                if (i == 7 && field[k][j] == 2 && whiteEdges[3][j/8] == -k) whiteEdges[3][j/8]--;
                


                int a;
                int b;

                if (i == 0 && j == 0) {
                    a = k; // ++
                    b = 0; // ++
                } else if (i == 0 && j == 7) {
                    a = 0; // --
                    b = 7-k; // ++
                } else if (i == 7 && j == 0) {
                    a = 7-k; // --
                    b = 0; // ++
                } else if (i == 7 && j == 7) {
                    a = 7; // ++
                    b = 7-k; // ++
                }

                while (a >= 0 && a < 8 && b >= 0 && b < 8) {
                    if (field[a][b] == 0 || field[a][b] == 2) triangle[k] = -1;

                    if (i == j) a++;
                    else a--;
                    b++;
                }
            }
            for (int k = 0; k < 8; k++) {
                sum += triangle[k] * k * 4 + whiteEdges[k/2][0] + whiteEdges[k/2][0] + whiteEdges[k/2][1] + blackEdges[k/2][0] + blackEdges[k/2][1];
            }
            
        }
    }
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] == 1) {
                if (isSafe(field, i*8+j, 1) && isSafe(field, i*8+j, 9) && isSafe(field, i*8+j, 10) && isSafe(field, i*8+j, 11)) sum+=1;
            } else if (field[i][j] == 2) {
                if (isSafe(field, i*8+j, 1) && isSafe(field, i*8+j, 9) && isSafe(field, i*8+j, 10) && isSafe(field, i*8+j, 11)) sum-=1;
            }
        }
    }
    if (side == 2) sum = -sum;
    // if (black.size() > 0 && side == 1) return sum/black.size();
    // if (white.size() > 0 && side == 2) return sum/white.size();
    return sum;
}

int minimax(int field[8][8], int side, int mySide, set<pair<int, int>> & black, set<pair<int, int>> & white, int depth, int alpha, int beta) {
    if (depth == 0) {
        // vector<pair<int, int>> turns;
        // existingTurns(field, side, (side == 1) ? black : white, turns);
        // set<pair<int, int>> uniqTurns;
        // for (auto it : turns) uniqTurns.insert(it);
        // return uniqTurns.size();
        // return (side == 1) ? black.size()-white.size() : white.size()-black.size();
        return score(field, mySide, black, white);
    }
    if (side == mySide) {
        int bestVal = -INF;
        vector<pair<int, int>> turns;
        existingTurns(field, side, (side == 1) ? black : white, turns);
        set<pair<int, int>> uniqTurns;
        for (auto it : turns) uniqTurns.insert(it);
        for (auto it : uniqTurns) {
            int copyField[8][8];
            set<pair<int, int>> copyBlack;
            set<pair<int, int>> copyWhite;
            for (auto it : black) copyBlack.insert(it);
            for (auto it : white) copyWhite.insert(it);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    copyField[i][j] = field[i][j];
                }
            }
            doTurn(copyField, side, copyBlack, copyWhite, p2str(it));
            int value = minimax(copyField, side%2+1, mySide, copyBlack, copyWhite, depth-1, alpha, beta);
            bestVal = max(bestVal, value);
            alpha = max(alpha, bestVal);
            if (beta < alpha) break;
            return bestVal;
        }
    } else {
        int bestVal = INF;
        vector<pair<int, int>> turns;
        existingTurns(field, side, (side == 1) ? black : white, turns);
        set<pair<int, int>> uniqTurns;
        for (auto it : turns) uniqTurns.insert(it);
        for (auto it : uniqTurns) {
            int copyField[8][8];
            set<pair<int, int>> copyBlack;
            set<pair<int, int>> copyWhite;
            for (auto it : black) copyBlack.insert(it);
            for (auto it : white) copyWhite.insert(it);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    copyField[i][j] = field[i][j];
                }
            }
            doTurn(copyField, side, copyBlack, copyWhite, p2str(it));
            int value = minimax(copyField, side%2+1, mySide, copyBlack, copyWhite, depth-1, alpha, beta);
            bestVal = min(bestVal, value);
            beta = min(beta, bestVal);
            if (beta < alpha) break;
            return bestVal;
        }
    }
}

int main() {
    // srand(time(NULL));
    int globalSide = 1; // 1 - черные, 2 - белые
    int mySide;
    int enemySide;
    int globalField[8][8] = {0}; // 1 - черные, 2 - белые

    globalField[3][3] = 2;
    globalField[3][4] = 1;
    globalField[4][3] = 1;
    globalField[4][4] = 2;

    set<pair<int, int>> black;
    black.insert(make_pair(3, 4));
    black.insert(make_pair(4, 3));
    set<pair<int, int>> white;
    white.insert(make_pair(3, 3));
    white.insert(make_pair(4, 4));
    
    // view(globalField);

    int a;
    cin >> a;

    if (a == 2) {
        string b;
        cin >> b;
        doTurn(globalField, globalSide, black, white, b);
        globalSide = 2;
        mySide = 2;
        enemySide = 1;
    } else {
        mySide = 1;
        enemySide = 2;
    }
    

    while (1) {
        if (globalSide == mySide) {
            vector<pair<int, int>> turns;
            existingTurns(globalField, mySide, (mySide == 1) ? black : white, turns);
            if (turns.size()) {
                set<pair<int, int>> uniqTurns;
                for (auto it : turns) uniqTurns.insert(it);
                string turn = p2str(turns.front());
                int best = 0;
                for (auto it : uniqTurns) {
                    int val = minimax(globalField, globalSide, mySide, black, white, 6, -INF, INF);
                    if (best < val) {
                        best = val;
                        turn = p2str(it);
                    }
                }
                // string turn = p2str(turns[rand() % turns.size()]);
                doTurn(globalField, mySide, black, white, turn);
                cout << turn << "\n";
            } else cout << "Skip\n";
        } else {
            string turn;
            cin >> turn;
            if (turn != "Skip") doTurn(globalField, enemySide, black, white, turn);
        }
        globalSide = (globalSide == 1) ? 2 : 1;

        // view(globalField);
    }

    return 0;   
}