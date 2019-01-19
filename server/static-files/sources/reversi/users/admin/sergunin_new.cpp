#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <ctime>
#include <string.h>
#include <assert.h>

#define all(a) (a).begin(), (a).end()
#define sz(a) (int)(a).size()
#define pb push_back

using namespace std;

class Board {
public:
        Board() {
                board.resize(n, vector<int>(n));
                int k = n / 2 - 1;
                board[k][k] = board[k + 1][k + 1] = 2;
                board[k + 1][k] = board[k][k + 1] = 1;
        }

        void print_board() {
                cout <<" 12345678" << endl;
                for (int i = 0; i < n; ++i) {
                        cout << char(i + 'a');
                        for (int j = 0; j < n; ++j) {
                                if (board[i][j] == 0)
                                        cout << '.';
                                else if (board[i][j] == 1)
                                        cout << 'O';
                                else 
                                        cout << '@';
                        }
                        cout << endl;
                }
        }

        void update(int x, int y, int color) {

                board[x][y] = color;
                for (int i = 0; i < 8; ++i) {
                        int x1 = x + dx[i];
                        int y1 = y + dy[i];
                        if (!inBoard(x1, y1) || board[x1][y1] != 3 - color)
                                continue;
                        for (; inBoard(x1, y1) && board[x1][y1] == 3 - color ; x1 += dx[i], y1 += dy[i]);
                        if (!inBoard(x1, y1) || board[x1][y1] != color)
                                continue;
                        int j = i ^ 4;
                        x1 += dx[j];
                        y1 += dy[j];
                        for (; inBoard(x1, y1) && board[x1][y1] == 3 - color; x1 += dx[j], y1 += dy[j])
                                board[x1][y1] = color;
                }
        }

        void get_turns(vector<pair<int, int>> &turns, int color) {
                for (int i = 0; i < n; ++i)
                        for (int j = 0; j < n; ++j)
                                if (check_move(i, j, color))
                                        turns.push_back({i, j});
        }

        int check_position() {
                int ans = 0;
                for (int i = 0; i < n; ++i)
                        for (int j = 0; j < n; ++j) {
                                if (board[i][j] == 1)
                                        ans += cost[i][j];
                                else if (board[i][j] == 2)
                                        ans += cost[i][j];
                        }
                return ans;
        }

private:

        bool inBoard(int x, int y) {
                return (0 <= x && x < n && 0 <= y && y < n);
        }

        bool check_move(int x, int y, int color) {
                if (board[x][y])
                        return false;
                for (int i = 0; i < 8; ++i) {
                        int x1 = x + dx[i];
                        int y1 = y + dy[i];
                        if (!inBoard(x1, y1) || board[x1][y1] != 3 - color)
                                continue;
                        for (; inBoard(x1, y1) && board[x1][y1] == 3 - color; x1 += dx[i], y1 += dy[i]);
                        if (inBoard(x1, y1) && board[x1][y1] == color)
                                return true;
                }

                return false;
        }

        int n = 8;

        vector<vector<int>> cost = {
                {20, 1, 5, 5, 5, 5, 1, 20},
                {1, 1, 3, 3, 3, 3, 1, 1},
                {5, 3, 3, 3, 3, 3, 3, 5},
                {5, 3, 3, 3, 3, 3, 3, 5},
                {5, 3, 3, 3, 3, 3, 3, 5},
                {5, 3, 3, 3, 3, 3, 3, 5},
                {1, 1, 3, 3, 3, 3, 1, 1},
                {20, 1, 5, 5, 5, 5, 1, 20}
        };

        vector<int> dx = {1, 1, 0, -1, -1, -1, 0, 1};
        vector<int> dy = {0, 1, 1, 1, 0, -1, -1, -1};
        
        vector<vector<int>> board;
};

struct Turn {
        int x, y;
        int result;
};


class ReversiBot {
public:
        ReversiBot() {};

        int minimax(int color, int depth, int maxDepth) {
                if (depth == maxDepth) 
                        return board.check_position();
                vector<pair<int, int>> turns;
                board.get_turns(turns, color);

                if (turns.empty()) {
                        if (depth == 0) 
                                cout << "Skip" << endl;
                        else {
                                return board.check_position();
                        }
                        return 0;
                }

                Turn best = {-1, -1, color == 1 ? -INF : INF};
                for (int i = 0; i < sz(turns); ++i) {
                        Board copy = board;
                        board.update(turns[i].first, turns[i].second, color);
                        int cur = minimax(3 - color, depth + 1, maxDepth);
                        board = copy;

                        if (color == 1) {
                                if (cur > best.result)
                                        best = {turns[i].first, turns[i].second, cur};
                        }
                        if (color == 2) {
                                if (cur < best.result)
                                        best = {turns[i].first, turns[i].second, cur};
                        }
                }
                if (depth == 0) {
                        update(best.x, best.y, color);
                        cout << char(best.x + 'a') << best.y + 1 << endl;
                        return 0;
                }

                return best.result;
        }

        void print_board() {
                board.print_board();
        }

        void player(int color) {
                string s;
                cin >> s;
                if (s == "Skip")
                        return;
                int x = s[0] - 'a', y = s[1] - '1';
                update(x, y, color);
        }

        void update(int x, int y, int color) {
                board.update(x, y, color);
        }

        bool end_of_the_game() {
                vector<pair<int, int>> turns;
                board.get_turns(turns, 1);
                board.get_turns(turns, 2);
                return !sz(turns);
        }

private:

        Board board;
        const int INF = 1e9;
};

const int DEPTH = 4;

int main() {
        int color;
        cin >> color;

        ReversiBot bot;

        // bot.print_board();

        while (!bot.end_of_the_game()) {
                if (color == 1)  {
                        bot.minimax(color, 0, DEPTH);
                        // bot.print_board();
                }

                bot.player(3 - color);
                // bot.print_board();

                if (color == 2) {
                        bot.minimax(color, 0, DEPTH);
                        // bot.print_board();
                }
        }
}
