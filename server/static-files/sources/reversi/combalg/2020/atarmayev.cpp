#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>

using namespace std;

typedef long long ll;
typedef long double ld;

const char default_symbol = '-';

char board[8][8]; //0 - our, 1 - opponent;

void preprocess(int x) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = default_symbol;
        }
    }
    if (x == 1) {
        board[3][4] = 48;
        board[3][3] = 49;
        board[4][3] = 48;
        board[4][4] = 49;
    } else {
        board[3][4] = 49;
        board[3][3] = 48;
        board[4][3] = 49;
        board[4][4] = 48;
    }
}

int get_row (char c) {
    switch (c) {
    case 'a':
        return 0;
    case 'b':
        return 1;
    case 'c':
        return 2;
    case 'd':
        return 3;
    case 'e':
        return 4;
    case 'f':
        return 5;
    case 'g':
        return 6;
    case 'h':
        return 7;
    }
}

char letter_from_digit (int x) {
    switch(x) {
    case 0:
        return 'a';
    case 1:
        return 'b';
    case 2:
        return 'c';
    case 3:
        return 'd';
    case 4:
        return 'e';
    case 5:
        return 'f';
    case 6:
        return 'g';
    case 7:
        return 'h';
    }
}

bool in_range (int coordinate) {
    if (coordinate >= 0 && coordinate < 8) {
        return true;
    }
    return false;
}

void repaint (int x_from, int y_from, int x_to, int y_to, int x_change, int y_change, int color) {
    // cout << "Repaint work! " << x_from << ' ' << y_from << ' ' << x_to << ' ' << y_to << ' ' << x_change << ' '
    // << y_change << ' ' << color << endl;
    board[x_from][y_from] = color + 48;
    while (!(x_from == x_to && y_from == y_to)) {
        x_from += x_change;
        y_from += y_change;
        board[x_from][y_from] = color  + 48;
    }
    board[x_to][y_to] = color + 48;
}

bool direction_check (int x_check, int y_check, int x_change, int y_change, int color) {
    bool between = false;
    int memx = x_check, memy = y_check;
    x_check += x_change;
    y_check += y_change;
    while (in_range(x_check) && in_range(y_check) && board[x_check][y_check] == ((color + 1) % 2) + 48) {
        x_check += x_change;
        y_check += y_change;
        between = true;
    }
    if (!in_range(x_check) || !in_range(y_check)) {
        //there are opponent's stones until the end of the board
        return false;
    } else {
        if (board[x_check][y_check] == color + 48 && between) {
            //move is possible
            repaint (memx, memy, x_check, y_check, x_change, y_change, color);
            return true;
        } else {
            //two stones of the same color locates nearby
            return false;
        }
    }
}

bool possible_move(int i, int j) {
    // need to check an 8 directions
    if (board[i][j] != default_symbol) {
        return false;
    }
    int x_check = i, y_check = j;
    bool ans = false;
    // right
    ans |= direction_check(x_check, y_check, 1, 0, 0);
    // left
    ans |= direction_check(x_check, y_check, -1, 0, 0);
    // up
    ans |= direction_check(x_check, y_check, 0, 1, 0);
    // down
    ans |= direction_check(x_check, y_check, 0, -1, 0);
    // up & right
    ans |= direction_check(x_check, y_check, 1, 1, 0);
    // right & down
    ans |= direction_check(x_check, y_check, 1, -1, 0);
    // down & left
    ans |= direction_check(x_check, y_check, -1, -1, 0);
    // left & up
    ans |= direction_check(x_check, y_check, -1, 1, 0);
    return ans;
}

bool possible_move_for_opponent (int i, int j) {
    // need to check 8 directions
    if (board[i][j] != default_symbol) {
        return false;
    }
    int x_check = i, y_check = j;
    bool ans = false;
    // right
    ans |= direction_check(x_check, y_check, 1, 0, 1);
    // left
    ans |= direction_check(x_check, y_check, -1, 0, 1);
    // up
    ans |= direction_check(x_check, y_check, 0, 1, 1);
    // down
    ans |= direction_check(x_check, y_check, 0, -1, 1);
    // up & right
    ans |= direction_check(x_check, y_check, 1, 1, 1);
    // right & down
    ans |= direction_check(x_check, y_check, 1, -1, 1);
    // down & left
    ans |= direction_check(x_check, y_check, -1, -1, 1);
    // left & up
    ans |= direction_check(x_check, y_check, -1, 1, 1);
    return ans;
}

void make_random_move() {
    int cnt = 0;
    while (true) {
        ++cnt;
        if (cnt > 1e5) {
            cout << "Skip" << '\n';
            break;
        }
        int i = rand() % 8;
        int j = rand() % 8;
        if (possible_move(i, j)) {
            cout << letter_from_digit(i) << j+1 << '\n';
            board[i][j] = 48;
            break;
        }
    }
}

void upd_board (int x_change, int y_change) {
    bool a = possible_move_for_opponent(x_change, y_change);
}

bool is_board_full() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == default_symbol) {
                return true;
            }
        }
    }
    return false;
}
/*
void show() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << "   ";
        }
        cout << endl;
    }
}*/

int main() {
    srand(time(NULL));
    int val; cin >> val;
    char letter; int row, column;
    preprocess(val);
    if (val == 1) {
        make_random_move();
        // show();
    }
    // we make the first move
    while (is_board_full) {
        cin >> letter;
        if (letter == 'S') {
            goto lab;
        }
        cin >> column;
        column--;
        row = get_row(letter);
        upd_board(row, column);
        // show();
        lab:0;
        if (is_board_full) {
            make_random_move();
        }
        // show();
    }
}
