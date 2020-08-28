//#define _GLIBCXX_DEBUG
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <iomanip>
#include <queue>
#include <map>
#include <assert.h>
#include <fstream>
#include <cmath>

using namespace std;
//#define cout cerr
//#define int long long
//#define double long double
const int MX = 2e9;
//const int MX_NUM = 1e18 + 1;
//int mod = 1000000007;

int opponent(int x) {
    if (x == 1)
        return 2;
    return 1;
}

void printt(vector<vector<int> > a) {
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j)
            cout << a[i][j];
        cout << endl;
    }
}

bool check(int i, int j, int x, int y, int type, vector<vector<int> > &a) {
    if (abs(i - x) <= 1 && abs(j - y) <= 1)
        return false;
    if (j == y && i < x - 1) {
        for (int k = i + 1; k < x; ++k)
            if (a[k][j] == type || a[k][j] == 0)
                return false;
        return true;
    } else if (j == y && i > x + 1) {
        for (int k = x + 1; k < i; ++k)
            if (a[k][j] == type || a[k][j] == 0)
                return false;
        return true;
    } else if (i == x && j < y - 1) {
        for (int k = j + 1; k < y; ++k)
            if (a[i][k] == type || a[i][k] == 0)
                return false;
        return true;
    } else if (i == x && j > y + 1) {
        for (int k = y + 1; k < j; ++k)
            if (a[i][k] == type || a[i][k] == 0)
                return false;
        return true;
    } else if (abs(i - x) == abs(j - y) && i > x && j > y) {
        for (int k = 1; k < abs(i - x); ++k)
            if (a[x + k][y + k] == type || a[x + k][y + k] == 0)
                return false;
        return true;
    } else if (abs(i - x) == abs(j - y) && i < x && j > y) {
        for (int k = 1; k < abs(i - x); ++k)
            if (a[x - k][y + k] == type || a[x - k][y + k] == 0)
                return false;
        return true;
    } else if (abs(i - x) == abs(j - y) && i < x && j < y) {
        for (int k = 1; k < abs(i - x); ++k)
            if (a[x - k][y - k] == type || a[x - k][y - k] == 0)
                return false;
        return true;
    } else if (abs(i - x) == abs(j - y) && i > x && j < y) {
        for (int k = 1; k < abs(i - x); ++k)
            if (a[x + k][y - k] == type || a[x + k][y - k] == 0)
                return false;
        return true;
    }
    return false;
}

pair<vector<vector<int> >, vector<int> > update(vector<vector<int> > &b, vector<int> &help, int x, int y, int type) {
    auto a = b;
    auto helpp = help;
    a[x][y] = type;
    helpp[0]--;
    helpp[type]++;
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            if ((i == x && j == y) || a[i][j] != type || (abs(i - x) <= 1 && abs(j - y) <= 1))
                continue;
            bool flag = true;
            if (j == y && i < x - 1) {
                for (int k = i + 1; k < x; ++k)
                    if (a[k][j] == type || a[k][j] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = i + 1; k < x; ++k) {
                    a[k][j] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (j == y && i > x + 1) {
                for (int k = x + 1; k < i; ++k)
                    if (a[k][j] == type || a[k][j] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = x + 1; k < i; ++k) {
                    a[k][j] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (i == x && j < y - 1) {
                for (int k = j + 1; k < y; ++k)
                    if (a[i][k] == type || a[i][k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = j + 1; k < y; ++k) {
                    a[i][k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (i == x && j > y + 1) {
                for (int k = y + 1; k < j; ++k)
                    if (a[i][k] == type || a[i][k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = y + 1; k < j; ++k) {
                    a[i][k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }

            } else if (abs(i - x) == abs(j - y) && i > x && j > y) {
                for (int k = 1; k < abs(i - x); ++k)
                    if (a[x + k][y + k] == type || a[x + k][y + k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = 1; k < abs(i - x); ++k) {
                    a[x + k][y + k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (abs(i - x) == abs(j - y) && i < x && j > y) {
                for (int k = 1; k < abs(i - x); ++k)
                    if (a[x - k][y + k] == type || a[x - k][y + k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = 1; k < abs(i - x); ++k) {
                    a[x - k][y + k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (abs(i - x) == abs(j - y) && i < x && j < y) {
                for (int k = 1; k < abs(i - x); ++k)
                    if (a[x - k][y - k] == type || a[x - k][y - k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = 1; k < abs(i - x); ++k) {
                    a[x - k][y - k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            } else if (abs(i - x) == abs(j - y) && i > x && j < y) {
                for (int k = 1; k < abs(i - x); ++k)
                    if (a[x + k][y - k] == type || a[x + k][y - k] == 0)
                        flag = false;
                if (!flag)
                    continue;
                for (int k = 1; k < abs(i - x); ++k) {
                    a[x + k][y - k] = type;
                    helpp[opponent(type)]--;
                    helpp[type]++;
                }
            }
        }
    }

    //printt(a);
    return {a, helpp};
}

vector<pair<int, int> > findd(vector<vector<int>> &a, int type) {
    vector<pair<int, int> > ans;
    for (int i = 1; i <= 8; ++i)
        for (int j = 1; j <= 8; ++j)
            if (a[i][j] == 0)
                for (int k = 1; k <= 8; ++k)
                    for (int p = 1; p <= 8; ++p)
                        if (a[k][p] == type && check(i, j, k, p, type, a))
                            ans.push_back({i, j});

    return ans;
}

int scoring_function(int cnt1, int cnt2, int type, vector<vector<int> > &a) {
    if(a[8][1] == 1)
        cnt1 += 7;
    else if(a[8][1] == 2)
        cnt2 += 7;

    if(a[1][1] == 1)
        cnt1 += 7;
    else if(a[1][1] == 2)
        cnt2 += 7;

    if(a[1][8] == 1)
        cnt1 += 7;
    else if(a[1][8] == 2)
        cnt2  += 7;
    if(a[8][8] == 1)
        cnt1 += 7;
    else if(a[8][8] == 2)
        cnt2 += 7;
    if ((cnt1 > cnt2 && type == 1) || (cnt1 < cnt2 && type == 2))
        return abs(cnt1 - cnt2);
    return -abs(cnt1 - cnt2);
}

int alphabeta(vector<vector<int> > a, vector<int> help, int depth, int alpha, int beta, int type,
              int max_depth = 4) {
    vector<pair<int, int> > all_moves = findd(a, type);
    if (depth == max_depth || all_moves.empty()) {
        return scoring_function(help[1], help[2], type, a);
    }
    for (auto u : all_moves) {
        auto k = update(a, help, u.first, u.second, type);
        int new_score = -alphabeta(k.first, k.second, depth + 1, -beta, -alpha, opponent(type));
        alpha = max(alpha, new_score);
        if (alpha >= beta)
            return alpha;
    }
    return alpha;
}


pair<int, int> minmax_for_player(vector<vector<int> > a, vector<int> help, int type) {
    pair<int, int> best_move = {-1, -1};
    int best_score = -MX;
    vector<pair<int, int> > all_moves = findd(a, type);
    for (auto u : all_moves) {
        auto k = update(a, help, u.first, u.second, type);
        int new_score = -alphabeta(k.first, k.second, 0, -MX, MX, opponent(type));
        if (best_score == -MX || new_score >= best_score) {
            best_score = new_score;
            best_move = u;
        }
    }
    return best_move;
}


signed main() {
    //1 = black
    //2 = white
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    vector<vector<int> > a(9, vector<int>(9, 0));
    a[4][4] = 2;
    a[5][5] = 2;
    a[4][5] = 1;
    a[5][4] = 1;
    int type;
    cin >> type;
    vector<int> help = {60, 2, 2};
    if (type == 1) {
        // pair<int, int> b = minmax_for_player(a, help, 1);
        auto k = update(a, help, 5, 6, 1);
        a = k.first;
        help = k.second;
        cout << "e6" << endl;
        //printt(a);
    }
    while (true) {
        char c1, c2;
        cin >> c1 >> c2;
        if (c1 == 'S') {
            cin >> c2;
            cin >> c2;
        } else {
            auto k = update(a, help, c1 - 'a' + 1, c2 - '0', opponent(type));
            a = k.first;
            help = k.second;
            // printt(a);
        }
        //printt(a);
        pair<int, int> b = minmax_for_player(a, help, type);
        if (b.first == -1)
            cout << "Skip" << endl;
        else {
            auto k = update(a, help, b.first, b.second, type);
            a = k.first;
            help = k.second;
            cout << char(b.first + 'a' - 1) << b.second << endl;
            //   printt(a);
        }
    }

}