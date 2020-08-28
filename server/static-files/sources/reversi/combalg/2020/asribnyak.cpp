#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

int dx[8] = {1, 1, 0, -1, -1, -1,  0,  1};
int dy[8] = {0, 1, 1,  1,  0, -1, -1, -1};

string SKIP = "Skip";

string mtos(int x, int y) {
    return string({char('a'+x), char('1'+y)});
}

mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

template<class T>
T random_choice(const vector<T>& vec) {
    return vec[rnd() % vec.size()];
}

template<class T>
vector<T> random_shuffle(const vector<T>& a) {
    auto b = a;
    for (int i = 0, j, k; i < a.size()/2; ++i) {
        j = rnd() % a.size();
        k = rnd() % a.size();
        swap(b[j], b[k]);
    }
    return b;
}

struct Board {
    static const int N = 8;
    int loc[N][N], white, black;

    Board() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                loc[i][j] = 0;
        int k = N/2;
        loc[k-1][k-1] = -1;
        loc[k-1][k] = 1;
        loc[k][k-1] = 1;
        loc[k][k] = -1;
        black = white = 2;
    }

    bool is_move_legal(int c, int x, int y) const {
        if (loc[x][y] != 0)
            return false;
        for (int i = 0, nx, ny; i < 8; ++i) {
            nx = x + dx[i];
            ny = y + dy[i];
            while (nx >= 0 && ny >= 0 && nx < N && ny < N &&
                    loc[nx][ny] == -c) {
                nx += dx[i];
                ny += dy[i];
            }
            if (nx >= 0 && ny >= 0 && nx < N && ny < N &&
                    !(nx == x + dx[i] && ny == y + dy[i]) &&
                    loc[nx][ny] == c)
                return true;
        }
        return false;
    }

    vector<string> get_legal_moves(int c) const {
        vector<string> ans;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (is_move_legal(c, i, j))
                    ans.push_back(mtos(i, j));
        return ans;
    }

    void move(int c, const string& coords) {
        if (coords == SKIP)
            return;
        int x = coords[0]-'a', y = coords[1]-'1';
        loc[x][y] = c;
        int rev = 0;

        for (int i = 0, nx, ny; i < 8; ++i) {
            nx = x + dx[i];
            ny = y + dy[i];
            while (nx >= 0 && ny >= 0 && nx < N && ny < N &&
                    loc[nx][ny] == -c) {
                nx += dx[i];
                ny += dy[i];
            }
            if (nx >= 0 && ny >= 0 && nx < N && ny < N &&
                    loc[nx][ny] == c) {
                nx -= dx[i];
                ny -= dy[i];
                while (nx != x || ny != y) {
                    loc[nx][ny] = c;
                    rev++;
                    nx -= dx[i];
                    ny -= dy[i];
                }
            }
        }

        if (c == 1) {
            black += rev+1;
            white -= rev;
        } else {
            white += rev+1;
            black -= rev;
        }
    }
};

int score_board(int c, const Board& board) {
    int black_win = 0, white_win = 0;
    int ct;
    bool skip;
    Board board_copy;
    vector<string> moves;
    for (int i = 0; i < 5; ++i) {
        ct = c;
        skip = false;
        board_copy = board;
        moves = board_copy.get_legal_moves(ct);
        while (true) {
            if (moves.size() != 0) {
                skip = false;
                board_copy.move(ct, random_choice(moves));
            } else {
                if (skip)
                    break;
                skip = true;
            }
            ct = -ct;
            moves = board_copy.get_legal_moves(ct);
        }
        if (board_copy.black > board_copy.white)
            black_win += 2;
        else if (board_copy.black < board_copy.white)
            white_win += 2;
        else {
            black_win++;
            white_win++;
        }
    }
    if (c == 1)
        return black_win - white_win;
    else
        return white_win - black_win;
}

int alphabeta(int c, const Board& board, int alpha, int beta, int depth) {
    if (depth == 0)
        return score_board(c, board);
    auto moves = board.get_legal_moves(c);
    if (moves.size() == 0)
        return score_board(c, board);
    Board board_copy;
    for (string move: moves) {
        board_copy = board;
        board_copy.move(c, move);
        alpha = max(alpha, -alphabeta(-c, board_copy, -beta, -alpha, depth-1));
        if (alpha >= beta)
            break;
    }
    return alpha;
}

struct Player {
    int c;
    Player(int id): c((id == 1) ? 1 : -1) {}
    string get_move(const Board& board) const {
        auto moves = random_shuffle(board.get_legal_moves(c));
        if (moves.size() == 0)
            return SKIP;
        string best_move = moves[0];
        int best_score = -1e7, new_score;
        Board board_copy;
        for (string move: moves) {
            board_copy = board;
            board_copy.move(c, move);
            new_score = -alphabeta(-c, board_copy, -1e7, 1e7, 2);
            if (new_score > best_score) {
                best_score = new_score;
                best_move = move;
            }
        }
        return best_move;
    }
};

/*/
void print(const Board& board) {
    cout << '+';
    for (int j = 0; j < board.N; ++j)
        cout << '-';
    cout << '+' << endl;
    for (int i = 0; i < board.N; ++i) {
        cout << '|';
        for (int j = 0; j < board.N; ++j)
            if (board.loc[i][j] == 0)
                cout << ' ';
            else
                cout << (board.loc[i][j] == 1 ? 'X' : 'O');
        cout << '|' << endl;
    }
    cout << '+';
    for (int j = 0; j < board.N; ++j)
        cout << '-';
    cout << '+' << endl;
}

int main() {
    Board board = Board();
    Player player1 = Player(1), player2 = Player(2);
    string last1 = "", last2 = "";
    auto t = chrono::steady_clock::now();
    while (last1 != SKIP || last2 != SKIP) {
        last1 = player1.get_move(board);
        cout << "player1: " << last1 << endl;
        board.move(1, last1);
        cout << std::chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - t).count() << endl;
        t = chrono::steady_clock::now();
        print(board);
        last2 = player2.get_move(board);
        cout << "player2: " << last2 << endl;
        board.move(-1, last2);
        cout << std::chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - t).count() << endl;
        t = chrono::steady_clock::now();
        print(board);
    }
    cout << board.black << " black" << endl;
    cout << board.white << " white" << endl;
    return 0;
}
/*/
int main() {
    Board board = Board();
    int id; cin >> id;
    Player player = Player(id);
    string move;
    if (id == 1) {
        move = player.get_move(board);
        board.move(1, move);
        //print(board);
        cout << move << endl;
    } else
        id = -1;
    while (true) {
        cin >> move;
        board.move(-id, move);
        //print(board);
        move = player.get_move(board);
        board.move(id, move);
        //print(board);
        cout << move << endl;
    }
    return 0;
}
//*/
