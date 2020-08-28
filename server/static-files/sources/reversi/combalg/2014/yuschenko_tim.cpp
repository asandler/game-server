#include <iostream>
#include <algorithm>
#include <vector>
#include <memory.h>
#include <map>

using namespace std;

enum cell {EMPTY, WHITE, BLACK};

bool white_m;

const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

class board{
public:
    int field[8][8];
    int state[8][8];
    pair<int, int> calc_score();
    bool move(cell type, pair<int, int> cell, bool flag);
    board() {
        memset(field, 0, sizeof(field));
        memset(state, -1, sizeof(state));
        field[3][3] = WHITE;
        field[4][4] = WHITE;
        field[3][4] = BLACK;
        field[4][3] = BLACK;
    }
    bool cellValid(int x, int y) {
        if (x < 0 || y < 0 || x >= 8 || y >= 8)
            return false;
        return true;
    }
    bool move_correct(cell type, pair<int, int> cell) {
        pair<int, int> c1 = this->calc_score();
        board b2 = *this;
        if (!b2.move(type, cell, false))
            return false;
        pair<int, int> c2 = b2.calc_score();
        if (c1.first != c2.first && c1.second != c2.second)
            return true;
        else return false;
    }
   void print() {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j)
                cerr << this->field[i][j];
            cerr << endl;
        }
        cerr << "_______________________________\n";
    }
};

pair<int, int> board::calc_score() {
    int ans1 = 0;
    int ans2 = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (this->field[i][j] == WHITE) ++ans1;
            else if (this->field[i][j] == BLACK) ++ans2;
    return make_pair(ans1, ans2);
}
bool board::move(cell type, pair<int, int> cell, bool flag = true) {
//    cerr << type << " " << cell.first << " " << cell.second << " " << flag << endl;
    if (flag && !this->move_correct(type, cell))
        return false;
    if (cell.first == -1) return false;
    int x = cell.first;
    int y = cell.second;
    field[cell.first][cell.second] = type;
    bool one_way = false;
    for (int k = 0; k < 8; ++k) {
        board rb = *this;
        int cur_x = x + dx[k];
        int cur_y = y + dy[k];
        if (!cellValid(cur_x, cur_y)) continue;
        if (this->field[cur_x][cur_y] == EMPTY) continue;
        bool any_cell = false;
//        while (cellValid(cur_x, cur_y)) {
        while (true) {
            if (!cellValid(cur_x, cur_y) || this->field[cur_x][cur_y] == EMPTY) {
                for (int i = 0; i < 8; ++i)
                    for (int j = 0; j < 8; ++j)
                        this->field[i][j] = rb.field[i][j];
//                cerr << "OLOLOL "  << cur_x << " " << cur_y << " " << x << " " << y << endl;
//                return false;
                break;
            }
            if (type != this->field[cur_x][cur_y])
                this->field[cur_x][cur_y] = type, any_cell = true;
            else {
                one_way |= any_cell;
                break;
            }
            cur_x += dx[k];
            cur_y += dy[k];
        }
        if (!cellValid(cur_x, cur_y)) continue;
    }
    if (!one_way) return false;
    return true;
}
 

pair<int, int> convert_move(string move) {
    return make_pair(move[0] - 'a', move[1] - '1');
}

map<pair<int, int>, double> pref;

void initPref() {
    pref[make_pair(0, 0)] = 100;
    pref[make_pair(0, 1)] = 10;
    pref[make_pair(0, 2)] = 90;
    pref[make_pair(0, 3)] = 80;
    pref[make_pair(1, 1)] = 10;
    pref[make_pair(1, 2)] = 30;
    pref[make_pair(1, 3)] = 40;
    pref[make_pair(2, 2)] = 70;
    pref[make_pair(2, 3)] = 60;
    pref[make_pair(3, 3)] = 50;
}

double getPositional(int x, int y) {
    if (x > 3) x = 7 - x;
    if (y > 3) y = 7 - y;
    if (x > y) swap(x, y);
    return pref[make_pair(x, y)];
}

bool stable_cells(board & b, int x, int y, cell type) {
    if (x < 0 || y < 0 || x >= 8 || y >= 8)
        return true;
    if (b.state[x][y] != -1)
        return b.state[x][y];
    if (b.field[x][y] != type)
        return false;
    if (x == 0 && y == 0) return true;
    if (x == 0 && y == 7) return true;
    if (x == 7 && y == 0) return true;
    if (x == 7 && y == 7) return true;

    b.state[x][y] = false;
    b.state[x][y] |= stable_cells(b, x, y - 1, type) && stable_cells(b, x - 1, y - 1, type) && stable_cells(b, x - 1, y, type); 
    b.state[x][y] |= stable_cells(b, x - 1, y, type) && stable_cells(b, x - 1, y + 1, type) && stable_cells(b, x, y + 1, type); 
    b.state[x][y] |= stable_cells(b, x, y + 1, type) && stable_cells(b, x + 1, y + 1, type) && stable_cells(b, x + 1, y, type); 
    b.state[x][y] |= stable_cells(b, x + 1, y, type) && stable_cells(b, x + 1, y - 1, type) && stable_cells(b, x, y - 1, type);
    return b.state[x][y];
}

double magic_function(board b, cell type) {
    memset(b.state, -1, sizeof(b.state));
    int stable_cells_c = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (stable_cells(b, i, j, type))
                ++stable_cells_c;
//            if (b.field[i][j] == EMPTY)
//                ++free_cells;
    double ans = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (b.field[i][j] == type)
                ans += getPositional(i, j) / 100 * stable_cells_c;
//                ans += getPositional(i, j) / 100 * free_cells;
    return ans;
}

bool compare_moves(board b, pair<int, int> m1, pair<int, int> m2, cell type) {
    board tmp = b;
    tmp.move(type, m1);
    double c1 = magic_function(tmp, type);

    tmp = b;
    tmp.move(type, m2);
    double c2 = magic_function(tmp, type);

    return c1 < c2;
}

pair<int, int> choose_move(board b, cell type) {
    double best_score = -1;
    pair<int, int> best_move = make_pair(-1, -1);
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            if (b.field[i][j] == EMPTY && b.move_correct(type, make_pair(i, j))) {
                if (getPositional(i, j) == best_score && compare_moves(b, best_move, make_pair(i, j), type))
                    best_move = make_pair(i, j);
                if (getPositional(i, j) > best_score)
                    best_score = getPositional(i, j), best_move = make_pair(i, j);
            }
        }
    return best_move;
}
pair<int, int> choose_move2(board b, cell type) {
    double best_score = -1;
    pair<int, int> best_move = make_pair(-1, -1);
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            if (b.field[i][j] == EMPTY && b.move_correct(type, make_pair(i, j))) {
                if (compare_moves(b, best_move, make_pair(i, j), type)) {
                    board tmp = b;
                    tmp.move(type, make_pair(i, j));
                    best_move = make_pair(i, j), best_score = magic_function(tmp, type);
                }
//                if (getPositional(i, j) > best_score)
//                    best_score = getPositional(i, j), best_move = make_pair(i, j);
            }
        }
    return best_move;
}

int main() {
    initPref();
    board cur_board;
    int color;
    cin >> color;
    if (color == 1) white_m = false;
    else white_m = true;

    string move;

    if (white_m == true) {
        //cur_board.print();
        cin >> move;
        //cerr << move << endl;
        cur_board.move(BLACK, convert_move(move), false);
        //cur_board.print();
    }

    int move_n = 0;

    while (true) {
        pair<int, int> my_move = move_n < 25 ? choose_move(cur_board, white_m ? WHITE : BLACK) :
                                               choose_move2(cur_board, white_m ? WHITE : BLACK);
        if (my_move.first == -1 || my_move.second == -1) {
            cout << "Skip" << endl;
            //cerr << "Skip" << endl;
        } else {
            cur_board.move(white_m ? WHITE : BLACK, my_move);
            cout << char(my_move.first + 'a') << char(my_move.second + '1') << endl;
            //cerr << char(my_move.first + 'a') << char(my_move.second + '1') << endl;
        }
        cout.flush();
        //cur_board.print();
        cin >> move;
        //cerr << move << endl;
        if (move != "Skip")
            cur_board.move(white_m ? BLACK : WHITE, convert_move(move), false);
        //cur_board.print();
        ++move_n;
    }

    return 0;
}
