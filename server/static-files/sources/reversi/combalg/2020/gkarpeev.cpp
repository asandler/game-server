#include <bits/stdc++.h>

using namespace std;

#define ull unsigned long long
#define ld long double
#define sz(a) (int) (a).size()
#define all(a) (a).begin(), (a).end()
#define x first
#define y second

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

auto start = chrono::high_resolution_clock::now();

const int inf = (1 << 16);
const int maxn = 8;

int dx[8] = {-1, -1, -1, 0, 1, 1,  1,  0};
int dy[8] = {-1,  0,  1, 1, 1, 0, -1, -1};

int board[maxn][maxn];
int directions[maxn][maxn][maxn];

map<pair<ull, ull>, int> used;
map<pair<ull, ull>, pair<int, int>> visited;

bool ok(int x, int y) {
    return (0 <= x && x < maxn) && (0 <= y && y < maxn);
}

vector<vector<pair<pair<int, int>, int>>> stack_updates;

void update_board(int x, int y, int color) {
    stack_updates.push_back({});
    stack_updates.back().push_back({{x, y}, board[x][y]});
    board[x][y] = color;
    for (int i = 0; i < maxn; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        while (ok(nx, ny) && board[nx][ny] == (color ^ 1)) {
            nx += dx[i], ny += dy[i];
        }
        if (!ok(nx, ny) || board[nx][ny] != color) {
            continue;
        }
        while (ok(nx, ny) && (nx != x || ny != y)) {
            stack_updates.back().push_back({{nx, ny}, board[nx][ny]});
            board[nx][ny] = color;
            nx -= dx[i], ny -= dy[i];
        }
    }
}

void undo_last_update() {
    for (auto &[el, value] : stack_updates.back()) {
        board[el.x][el.y] = value;
    }
    stack_updates.pop_back();
}

vector<pair<int, int>> get_moves(int color) {
    // [0, 3] directions:
    // 0. up left
    // 1. up
    // 2. up right
    // 3. right
    for (int i = 0; i < maxn; i++) {
        for (int j = maxn - 1; j >= 0; j--) {
            for (int k = 0; k < 4; k++) {
                directions[i][j][k] = 1;
                int nx = i + dx[k], ny = j + dy[k];
                if (ok(nx, ny) && board[nx][ny] == board[i][j]) {
                    directions[i][j][k] += directions[nx][ny][k];
                }
            }
        }
    }
    // [4, 7] directions:
    // 4. down right
    // 5. down
    // 6. down left
    // 7. left
    for (int i = maxn - 1; i >= 0; i--) {
        for (int j = 0; j < maxn; j++) {
            for (int k = 4; k < 8; k++) {
                directions[i][j][k] = 1;
                int nx = i + dx[k], ny = j + dy[k];
                if (ok(nx, ny) && board[nx][ny] == board[i][j]) {
                    directions[i][j][k] += directions[nx][ny][k];
                }
            }
        }
    }
    vector<pair<int, int>> moves;
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            if (board[i][j] != -1) continue;
            bool can = false;
            for (int k = 0; k < maxn && !can; k++) {
                if (directions[i][j][k] != 1) continue;
                int nx = i + dx[k], ny = j + dy[k];
                if (!ok(nx, ny) || board[nx][ny] != (color ^ 1)) {
                    continue;
                }
                int x = nx + directions[nx][ny][k] * dx[k], y = ny + directions[nx][ny][k] * dy[k];
                if (ok(x, y) && board[x][y] == color) {
                    can = true;
                }
            }
            if (can) {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

int get_score(int color) {
    int score = 0;
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            score += (board[i][j] == -1 ? 0 : (board[i][j] == color ? 1 : -1));
        }
    }
    return score;
}

pair<ull, ull> get_hash_board() {
    pair<ull, ull> hash_code = {0, 0};
    for (int i = 0; i < 64; i++) {
        if (i < 32) {
            if (board[i >> 3][i & 7] == -1) {
                hash_code.x |= 3;
            } else {
                hash_code.x |= board[i >> 3][i & 7];
            }
            if (i != 32 - 1) hash_code.x <<= 2;
        } else {
            if (board[i >> 3][i & 7] == -1) {
                hash_code.y |= 3;
            } else {
                hash_code.y |= board[i >> 3][i & 7];
            }
            if (i != 64 - 1) hash_code.y <<= 2;
        }
    }
    return hash_code;
}

vector<pair<pair<int, int>, pair<int, pair<ull, ull>>>> get_moves_with_score_hash(int color, int max_len = -1) {
    vector<pair<int, int>> moves = get_moves(color);
    vector<pair<pair<int, int>, pair<int, pair<ull, ull>>>> result;
    for (auto &[x, y] : moves) {
        update_board(x, y, color);
        result.push_back({{x, y}, {get_score(color), get_hash_board()}});
        undo_last_update();
    }
    if (max_len == -1 || sz(result) <= max_len) return result;
    sort(all(result), [&] (const pair<pair<int, int>, pair<int, pair<ull, ull>>> &ft, const pair<pair<int, int>, pair<int, pair<ull, ull>>> &sd){
        if (ft.y.x != sd.y.x) return ft.y.x < sd.y.x;
        return ft.x < sd.x;
    });
    result.erase(result.begin() + max_len, result.end());
    return result;
}

void add_from_visited(pair<ull, ull> &hash_1, pair<ull, ull> &hash_2) {
    auto tmp1 = visited[hash_1];
    auto tmp2 = visited[hash_2];
    tmp1.x += tmp2.x;
    tmp1.y += tmp2.y;
    visited[hash_1] = tmp1;
}

void del_from_visited(pair<ull, ull> &hash_1, pair<ull, ull> &hash_2) {
    auto tmp1 = visited[hash_1];
    auto tmp2 = visited[hash_2];
    tmp1.x -= tmp2.x;
    tmp1.y -= tmp2.y;
    visited[hash_1] = tmp1;
}

bool check_time() {
    auto tl = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count() / 1000000000.0;
    return tl > 1.8;
}

void MCTS(int color, pair<ull, ull> hash_position, int mx_cnt = 7) {
    if (check_time()) return;
    used[hash_position] = 1;
    auto moves = get_moves_with_score_hash(color, 10);
    shuffle(all(moves), rnd);
    int cnt = 0;
    for (auto &el : moves) {
        auto hash = el.y.y;
        if (used.find(hash) != used.end()) {
            used.erase(used.find(hash));
            continue;
        }
        del_from_visited(hash_position, hash);
        update_board(el.x.x, el.x.y, color);
        MCTS(color ^ 1, hash, max(1, mx_cnt - 1));
        undo_last_update();
        add_from_visited(hash_position, hash);
        cnt++;
        if (cnt >= mx_cnt) break;
    }
    if (sz(moves) == 0) {
        visited[hash_position] = {get_score(0) > 0, 1};
    }
}

ld get_function_of_position(pair<ull, ull> hash) {
    auto result = visited[hash];
    if (result.y == 0) return 0;
    return (result.x * 1.0 / result.y) + sqrtl(2.0 * log(result.y) / result.y);
}

pair<int, int> find_best_move(int color) {
    pair<int, int> best_move = {-1, -1};
    ld best_score = -inf;
    auto moves = get_moves_with_score_hash(color);
    auto hash = get_hash_board();
    MCTS(color, hash);
    for (auto &el : moves) {
        auto tmp_hash = el.y.y;
        ld tmp_result = get_function_of_position(tmp_hash);
        if (tmp_result > best_score) {
            best_score = tmp_result;
            best_move = el.x;
        }
    }
    return best_move;
}

bool make_move() {
    pair<int, int> best_move = find_best_move(0);
    if (best_move.x == -1 && best_move.y == -1) return false;
    update_board(best_move.x, best_move.y, 0);
    cout << char(best_move.x + 'a') << char(best_move.y + '1') << endl;
    return true;
}

void check_status(int status) {
    if (status == 0) return;
    string s;
    cin >> s;
    int x = s[0] - 'a';
    int y = s[1] - '1';
    update_board(x, y, status);
}

void build(int status) {
    memset(board, -1, sizeof board);
    update_board(3, 3, status ^ 1);
    update_board(4, 4, status ^ 1);
    update_board(3, 4, status);
    update_board(4, 3, status);
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int status = 1;
    cin >> status;
    status--;
    build(status);
    while (1) {
        check_status(status);
        if (!make_move()) {
            cout << "Skip" << endl;
//            cerr << "Skip" << endl;
        }
        /*for (int i = 0; i < maxn; i++) {
            for (int j = 0; j < maxn; j++) {
                if (board[i][j] == -1) {
                    cerr << '-';
                } else {
                    cerr << board[i][j];
                }
                cerr << " ";
            }
            cerr << endl;
        }
        cerr << endl;*/
        status = 1;
    }
    return 0;
}
