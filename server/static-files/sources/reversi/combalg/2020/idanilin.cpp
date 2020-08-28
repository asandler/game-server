#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>

#define black 1
#define white 2

using namespace std;

const int board_size = 8;
const long double mcts_c = sqrtl(2); // magic constant
const int games_in_node = 1;
const int mcts_iterations = 800;
int dx[] = {-1, 0, 1};

random_device rd;
mt19937 gen(rd());

struct Board {

    int board[board_size][board_size]{};
    int turn = black;

    void init() {
        for (auto & i : board) {
            for (int & j : i)
                j = 0;
        }
        board[3][3] = board[4][4] = white;
        board[3][4] = board[4][3] = black;
    }

    Board() { init(); }

    explicit Board(int turn) : turn(turn) { init(); }

    explicit Board(Board *_Board) {
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j) {
                board[i][j] = _Board->board[i][j];
            }
        }
        turn = _Board->turn;
    }

    int get_score() {
        // result black - result white
        int score = 0;
        for (auto & i : board) {
            for (int j : i) {
                if (j == black)
                    ++score;
                else if (j == white)
                    --score;
            }
        }
        return score;
    }

    int black_result() {
        int res = get_score();
        if (res > 0)
            return 2;
        if (res == 0)
            return 1;
        return 0;
    }

    static bool is_valid(int num) {
        return 0 <= num && num < board_size;
    }

    bool is_legal_move(int i, int j) {
        if (board[i][j])
            return false;
        for (int di : dx) {
            for (int dj : dx) {
                if (di == 0 && dj == 0)
                    continue;
                bool found_op_move = false;
                bool found_our_move = false;
                for (int ni = i + di, nj = j + dj; is_valid(ni) && is_valid(nj) && board[ni][nj]; ni += di, nj += dj) {
                    if (board[ni][nj] == 3 - turn)
                        found_op_move = true;
                    else if (board[ni][nj] == turn) {
                        found_our_move = true;
                        break;
                    }
                }
                if (found_op_move && found_our_move)
                    return true;
            }
        }
        return false;
    }

    vector<pair<int, int>> legal_moves() { // skip do not check
        vector<pair<int, int>> res;
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j) {
                if (is_legal_move(i, j))
                    res.emplace_back(i, j);
            }
        }
        if (res.empty())
            res.emplace_back(-1, -1);
        return res;
    }

    Board make_move(int i, int j) {
        Board new_board = Board(this);
        new_board.turn = 3 - this->turn;
        if (i == -1 && j == -1) // Skip
            return new_board;

        new_board.board[i][j] = turn;

        for (int di : dx) {
            for (int dj : dx) {
                if (di == 0 && dj == 0)
                    continue;

                bool found_my_move = false;

                for (int ni = i + di, nj = j + dj; is_valid(ni) && is_valid(nj) && board[ni][nj]; ni += di, nj += dj) {
                    if (board[ni][nj] == turn) {
                        found_my_move = true;
                        break;
                    }
                }

                if (found_my_move) {
                    for (int ni = i + di, nj = j + dj; is_valid(ni) && is_valid(nj) && board[ni][nj] != turn; ni += di, nj += dj) {
                        new_board.board[ni][nj] = turn;
                    }
                }
            }
        }

        return new_board;
    }

    Board make_move(pair<int, int> mv) {
        return make_move(mv.first, mv.second);
    }

    void visual() {
        cout << "Turn now: " << turn << endl;
        for (auto & i : board) {
            for (int j : i) {
                cout << j;
            }
            cout << endl;
        }
        cout << endl;
    }

    Board &operator = (Board t) {
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j)
                board[i][j] = t.board[i][j];
        }
        turn = t.turn;
        return *this;
    }
};

pair<int, int> random_player(Board board) {
    auto lst = board.legal_moves();
    int sz = lst.size();
    if (sz == 0)
        return make_pair(-1, -1);
    int random_choice = gen() % sz;
    return lst[random_choice];
}

int cnt = 0;

int random_game(Board board) {
    ++cnt;
    pair<int, int> pr_move = {-2, -2};
    while (true) {
        auto move = random_player(board);
        if (move == pr_move)
            break;
        pr_move = move;
        board = board.make_move(move);
    }
    return board.black_result();
}

struct mcts {
    vector<mcts *> childs;
    vector<pair<int, int>> move_to_child;
    mcts * parent = nullptr;
    Board board;

    int wins = 0, playouts = 0; // wins by black // wins = 2 * wins, playouts = 2 * playouts because of draws

    explicit mcts(Board _board) {
        board = _board;
        wins = playouts = 0;
    }

    mcts(Board _board, mcts *par) {
        board = _board;
        wins = playouts = 0;
        parent = par;
    }

    long double get_ucb_value() {
        assert(parent != nullptr && playouts);
        int N = parent->playouts;
        int wi = wins;
        int ni = playouts;
        long double val = (board.turn == white) ? (long double)wi / ni : 1. - (long double)wi / ni;
//        cerr << "get ucb value: N wi ni boar.turn val: " << N << ' ' << wi << ' ' << ni << ' ' << (board.turn == black ? "black" : "white") << ' ' << val << endl;
        return val + mcts_c * sqrt(log(N) / ni);
    }
    
    mcts * selection() {
        if (childs.empty())
            return this;
        long double mx = childs[0]->get_ucb_value(), ps = 0;
//        cerr << "selection here:" << endl;
//        cerr << childs[0]->wins << ' ' << childs[0]->playouts << ' ' << mx << endl;
        for (int i = 1; i < childs.size(); ++i) {
            long double cur_val = childs[i]->get_ucb_value();
//            cerr << childs[i]->wins << ' ' << childs[i]->playouts << ' ' << cur_val << endl;
            if (cur_val > mx) {
                mx = cur_val;
                ps = i;
            }
        }
        return childs[ps]->selection();
    }

    void expansion() {
        assert(childs.empty());
        auto lst = board.legal_moves();
        childs.resize(lst.size());
        move_to_child.resize(lst.size());
        for (int i = 0; i < lst.size(); ++i) {
            childs[i] = new mcts(board.make_move(lst[i]), this);
            move_to_child[i] = lst[i];
        }
    }

    void simulation(int number_of_games) {
        for (int i = 0; i < number_of_games; ++i) {
            wins += random_game(board);
            playouts += 2;
        }
    }

    void backpropagation(int sim_wins = -1, int sim_playouts = -1) {
        if (sim_wins != -1) {
            auto cur = this;
            while (true) {
                cur->wins += sim_wins;
                cur->playouts += sim_playouts;
                if (cur->parent == nullptr)
                    break;
                cur = cur->parent;
            }
        }
        else {
            assert(this->parent != nullptr);
            for (auto cur = &*this->parent;; cur = &*cur->parent) {
                cur->playouts += playouts;
                cur->wins += wins;
                if (cur->parent == nullptr)
                    break;
            }
        }
    }
    
    mcts * change_root(pair<int, int> move) {
        for (auto i = 0; i < move_to_child.size(); ++i) {
            if (move_to_child[i] == move) {
                childs[i]->parent = nullptr;
                return childs[i];
            }
        }
        childs.push_back(new mcts(board.make_move(move)));
        return childs.back();
    }
    
    pair<int, int> best_move() { // lets play most safest move!
        if (childs.empty())
            return {-1, -1};
        long double mx, ps = 0;
        mx = 0;
        for (int i = 0; i < childs.size(); ++i) {
            long double tt = (long double)childs[i]->wins / childs[i]->playouts;
            cerr << move_to_child[i].first << ' ' << move_to_child[i].second << ' ' << tt * 100 << "%" << ' ' << childs[i]->wins << ' ' << childs[i]->playouts << endl;
            if (childs[i]->playouts > mx) {
                mx = childs[i]->playouts;
                ps = i;
            }
        }
        cerr << 100. * childs[ps]->wins / childs[ps]->playouts << "%" << endl;
        cerr << childs[ps]->wins << ' ' << childs[ps]->playouts << endl;

        if (ps == -1)
            return make_pair(-1, -1);
        return move_to_child[ps];
    }

    void debug() {
        cerr << "mcts node debug, childs" << endl;
        cerr << move_to_child.size() << endl;
        for (auto now : move_to_child)
            cerr << now.first << ' ' << now.second << endl;
        cerr << "board in mcts:" << endl;
        cerr << "be careful, reverse" << endl;
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j) {
                cerr << board.board[j][i];
            }
            cerr << endl;
        }
    }
    
};


mcts *root;

pair<int, int> monte_carlo(int playouts = games_in_node) {
    cnt = 0;
    for (int i = 0; i < mcts_iterations; ++i) {
        mcts *lst_vrtx = root->selection();
        lst_vrtx->expansion();

        int sim_wins = 0, sim_playouts = 0;
        for (auto child : lst_vrtx->childs) {
            child->simulation(playouts);
            sim_wins += child->wins;
            sim_playouts += child->playouts;
        }
        lst_vrtx->backpropagation(sim_wins, sim_playouts);
    }
    auto move = root->best_move();
    root = root->change_root(move);
    cerr << "number of simulations " << cnt << endl;
    return move;
}


void print_move(pair<int, int> move) {
    if (move.first == -1 && move.second == -1) {
        cout << "Skip" << endl;
    }
    else {
        cout << (char) ('a' + move.first) << 1 + move.second << endl;
    }
}

pair<int, int> scan_move() {
    string in;
    cin >> in;
    if (in == "Skip")
        return make_pair(-1, -1);
    return make_pair((int)(in[0] - 'a'), in[1] - '0' - 1);
}

signed main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);

    Board board;

    pair<int, int> skip = {-1, -1};

    int st_color;
    cin >> st_color;

    //freopen((st_color == black ? "debug_black.out" : "debug_white.out"), "w", stderr);

    int nummv = 5;

    if (st_color == white) {
        auto op_move = scan_move();
        board = board.make_move(op_move.first, op_move.second);
        ++nummv;
    }

    root = new mcts(board);


    while (true) {

        cerr << "!!! nummv = " << nummv++ << endl;
//        auto my_move = random_player(board);
        auto my_move = monte_carlo();
//        board = board.make_move(my_move);
        print_move(my_move);

//        cerr << "after my move" << endl;
//        root->debug();

        cerr << "!!! nummv = " << nummv++ << endl;

        auto op_move = scan_move();
//        board = board.make_move(op_move);
        root = root->change_root(op_move);

//        cerr << "after op move" << endl;
//        root->debug();
    }

//    cout << board.get_score() << endl;

    return 0;
}

