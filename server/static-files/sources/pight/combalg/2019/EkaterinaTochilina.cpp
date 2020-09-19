#include<bits/stdc++.h>

using namespace std;

const int EMPTY = 0;
const int FOOD = 1;
const int FULL = 2;
map<char, int> char_to_move = {{'F', 0}, {'L', -1}, {'R', 1}};
map<int, char> move_to_char = {{-1, 'L'}, {0, 'F'}, {1, 'R'}};
vector<pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const int NO_WAY = 1e9;
const int INF = 2e9;
const int MAXX = 32;
const int MAXY = 42;
const int SQ = MAXX * MAXY;
const int CALC_STEPS = 5;

int field[MAXX][MAXY];
int field_hash = 0;
const int HASH_BASE = 3;
const int HASH_MOD = 1e9 + 21;
int base_pow[SQ];

pair<int, int> operator+ (const pair<int, int> &a, const pair<int, int> &b) {
    return {a.first + b.first, a.second + b.second};
}

ostream &operator<< (ostream &out, const pair<int, int> &a) {
    out << "(" << a.first << ',' << a.second << ")";
    return out;
}

stack<pair<int*, int>> changes;

void return_changes(size_t until) {
    while (changes.size() > until) {
        *changes.top().first = changes.top().second;
        changes.pop();
    }
}

void set_val(int &var, int val) {
    if (var == val)
        return;
    changes.push({&var, var});
    var = val;
}

void set_field_val(pair<int, int> p, int val) {
    int base = base_pow[p.first * MAXY + p.second];
    long long add = (long long) (val - field[p.first][p.second]) * base;
    while (add < 0)
        add += HASH_MOD;
    int new_hash_val = (field_hash + add) % HASH_MOD;
    if (new_hash_val < 0) {
        cerr << '!' << new_hash_val << endl;
    }
    set_val(field_hash, new_hash_val);
    set_val(field[p.first][p.second], val);
}

int get_field_val(pair<int, int> p) {
    if (p.first < 0 || p.first >= MAXX || p.second < 0 || p.second >= MAXY)
        return FULL;
    return field[p.first][p.second];
}

int calc_hash() {
    int res = 0;
    for (int i = 0; i < MAXX; i++) {
        for (int j = 0; j < MAXY; j++) {
            res = ((long long) field[i][j] * base_pow[i * MAXY + j] + res) % HASH_MOD;
        }
    }
    return res;
}

struct my_deque {
    pair<int, int> d[SQ];
    int from = 0, to = -1;
    void push_back(pair<int, int> val) {
        set_val(to, to + 1);
        set_val(d[to % SQ].first, val.first);
        set_val(d[to % SQ].second, val.second);
    }
    void pop_front() {
        set_val(from, from + 1);
    }
    pair<int, int> front() {
        return d[from % SQ];
    }
    pair<int, int> back() {
        return d[to % SQ];
    }
    bool empty() {
        return from > to;
    }
    int size() {
        return to - from + 1;
    }
    void print() {
        for (int i = from; i < to; i++) {
            cerr << d[i] << ' ';
        }
        cerr << endl;
    }
};

vector<int> random_order() {
    vector<int> order = {-1, 0, 1};
    random_shuffle(order.begin(), order.end());
    return order;
}

pair<int, int> calc_move(int depth, bool use_precalcs = true, vector<int> forbidden = {});

int last_used[MAXX][MAXY];
int last_id = 0;

struct snake {
    my_deque cells;
    int way = 0;
    int turn = 1;
    my_deque add;
    int add_rank = 0;
    pair<int, int> next_cell(int move_id) {
        int cur_way = (way + move_id + 4) % 4;
        return cells.back() + moves[cur_way];
    }
    bool make_move(int move_id) {
        set_val(add_rank, add_rank - add.size());
        if (add.empty() || add.front() != cells.front()) {
            set_field_val(cells.front(), EMPTY);
            cells.pop_front();
        } else {
            add.pop_front();
        }
        set_val(way, (way + move_id + 4) % 4);
        cells.push_back(cells.back() + moves[way]);
        if (get_field_val(cells.back()) == FOOD) {
            add.push_back(cells.back());
            set_val(add_rank, add_rank + cells.size());
        }
        if (get_field_val(cells.back()) == FULL) {
            cerr << "Incorrect move!" << endl;
            return 1;
        }
        set_field_val(cells.back(), FULL);
        return 0;
    }
    int try_cell(int move_id) {
        return get_field_val(this->next_cell(move_id));
    }
    int random_move(vector<int> forbidden = {}) {
        auto order = random_order();
        for (int i: order) {
            bool forb = false;
            for (int j: forbidden) {
                forb = forb || (i == j);
            }
            if (try_cell(i) != FULL && !forb) {
                return i;
            }
        }
        for (int i: order) {
            if (try_cell(i) != FULL) {
                return i;
            }
        }
        return NO_WAY;
    }
    bool can_move() {
        for (int i = -1; i <= 1; i++) {
            if (try_cell(i) != FULL)
                return true;
        }
        return false;
    }
    int cells_near(int dep, int id, int max_ok, int type1, int type2) {
        int cx, cy;
        tie(cx, cy) = this->cells.back();
        int res = 0;
        if (dep == 0)
            return res;
        last_used[cx][cy] = id;
        size_t sz = changes.size();
        for (int i = -1; i <= 1; i++) {
            int nx, ny;
            tie(nx, ny) = this->next_cell(i);
            if (field[nx][ny] == FULL || last_used[nx][ny] == id)
                continue;
            res += field[nx][ny] == type1 || field[nx][ny] == type2;
            this->make_move(i);
            res += cells_near(dep - 1, id, max_ok, type1, type2);
            return_changes(sz);
            if (res >= max_ok)
                break;
        }
        return res;
    }
    int bfs_move(vector<int> forbidden = {}) {
        const static pair<int, int> d[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int id = ++last_id;
        queue<pair<pair<int, int>, int>> Q;
        int order[] = {0, -1, 1};
        for (int i: order) {
            bool forb = false;
            for (int j: forbidden) {
                forb = forb || (i == j);
            }
            if (try_cell(i) != FULL && !forb) {
                auto p = next_cell(i);
                Q.push({p, i});
                last_used[p.first][p.second] = id;
            }
        }
        if (Q.empty())
            return NO_WAY;
        while (!Q.empty()) {
            int move_id;
            pair<int, int> cell;
            tie(cell, move_id) = Q.front();
            Q.pop();
            for (int i = 0; i < 4; i++) {
                auto n = cell + d[i];
                int val = get_field_val(n);
                if (val == FULL || last_used[n.first][n.second] == id) {
                    continue;
                }
                if (val == FOOD)
                    return move_id;
                last_used[n.first][n.second] = id;
                Q.push({n, move_id});
            }
        }
        return calc_move(CALC_STEPS, true, forbidden).second;
    }
    vector<int> find_forbidden() {
        vector<int> res;
        pair<int, int> bad_pos = {0, 0};
        size_t sz = changes.size();
        for (int i = -1; i <= 1; i++) {
            if (try_cell(i) == FULL)
                continue;
            make_move(i);
            int need = cells.size() * 3;
            int can_reach = cells_near(need, ++last_id, need, FOOD, EMPTY);
            if (can_reach < need) {
                res.push_back(i);
                bad_pos = max(bad_pos, {can_reach, i});
            }
            return_changes(sz);
        }
        if (res.size() == 3) {
            cerr << "Very bad" << endl;
            for (int i = 1; i < MAXX - 1; i++) {
                for (int j = 1; j < MAXY - 1; j++) {
                    cerr << field[i][j];
                }
                cerr << endl;
            }
            vector<int> res1;
            for (int i = -1; i <= 1; i++) {
                if (i != bad_pos.second)
                    res1.push_back(i);
            }
            return res1;
        }
        return res;
    }
    int make_move() {
        auto forb = find_forbidden();
        int cur_move, result;
        int food_near = cells_near(CALC_STEPS - 1, ++last_id, 1, FOOD, FOOD);
        int min_empty = CALC_STEPS * CALC_STEPS;
        if (food_near == 0 && cells_near(CALC_STEPS, ++last_id, min_empty, FOOD, EMPTY) >= min_empty) {
            cerr << "BFS move" << endl;
            cur_move = bfs_move(forb);
            /*for (int i: forb) {
                cerr << "forb " << i << endl;
            }
            cerr << "made " << cur_move << endl;*/
        } else {
            tie(result, cur_move) = calc_move(CALC_STEPS, true, forb);
        }
        if (cur_move == NO_WAY) {
            cur_move = random_move();
        }
        if (cur_move != NO_WAY) {
            this->make_move(cur_move);
        }
        return cur_move;
    }
};

snake me, them;

void set_field_border() {
    for (int i = 0; i < MAXY; i++) {
        field[0][i] = field[MAXX - 1][i] = FULL;
    }
    for (int i = 0; i < MAXX; i++) {
        field[i][0] = field[i][MAXY - 1] = FULL;
    }
}

void calc_pow() {
    base_pow[0] = 1;
    for (int i = 1; i < SQ; i++) {
        base_pow[i] = (long long) base_pow[i - 1] * HASH_BASE % HASH_MOD;
    }
}

int utility_func() {
    const static int STEPS = 3;
    const static int BASE = SQ;
    const static int SQBASE = BASE * BASE;
    int max_ok = (STEPS * 2 + 1) * (STEPS * 2 + 1) / 3;
    int cells_near_rank = min(me.cells_near(STEPS, ++last_id, max_ok, FOOD, EMPTY), max_ok);
    int size_rank = me.cells.size() + me.add.size();
    int result = cells_near_rank * SQBASE + size_rank * BASE + BASE - me.add_rank - 1;
    return result;
}

snake* who(int turn) {
    if (turn == 1) {
        return &me;
    } else {
        return &them;
    }
}

struct ttEntry {
    int depth = 0;
    pair<int, int> value = {-1, -1};
};

unordered_map<long long, ttEntry> mem;

pair<int, int> calc_move(int depth, bool use_precalcs, vector<int> forbidden) {
    if (!me.can_move()) {
        return {-INF, NO_WAY};
    }
    if (depth == 0) {
        return {utility_func(), NO_WAY};
    }
    int x = me.cells.back().first;
    int y = me.cells.back().second;
    long long key = (long long) field_hash * 200000 + x * MAXY * 10 + y * 10 + me.way;
    ttEntry calced = mem[key];
    int best_move = NO_WAY;
    int value = -INF;
    if (use_precalcs && calced.depth >= depth && me.try_cell(calced.value.second) != FULL) {
        return calced.value;
    }
    int order[] = {0, 1, -1};
    if (rand() % 2) {
        swap(order[1], order[2]);
    }
    int child_id = 0;
    for (int i: order) {
        if (me.try_cell(i) == FULL)
            continue;
        bool forb = false;
        for (int j : forbidden) {
            forb = forb || (i == j);
        }
        if (forb)
            continue;
        child_id++;
        size_t sz = changes.size();
        if (me.make_move(i)) {
            cerr << "Smth is very wrong" << endl;
            return {-INF, NO_WAY};
        }
        int val = calc_move(depth - 1, use_precalcs).first;
        return_changes(sz);
        if (val > value) {
            value = val;
            best_move = i;
        }
    }
    if (best_move == NO_WAY) {
        return {value, best_move};
    }
    ttEntry cur;
    cur.depth = depth;
    cur.value = make_pair(value, best_move);
    mem[key] = cur;
    return cur.value;
}

int main() {
    set_field_border();
    calc_pow();
    them.turn = -1;
    int turn;
    cin >> turn;
    me.way = (turn == 1 ? 2 : 0);
    them.way = (turn == 1 ? 0 : 2);
    for (int i = 1; i <= 4; i++) {
        pair<int, int> first = {i, 1};
        pair<int, int> second = {MAXX - i - 1, MAXY - 2};
        me.cells.push_back(turn == 1 ? first : second);
        them.cells.push_back(turn == 1 ? second : first);
        field[i][1] = FULL;
        field[MAXX - i - 1][MAXY - 2] = FULL;
    }
    for (int i = 1; i <= MAXX - 2; i++) {
        string s;
        cin >> s;
        for (int j = 1; j <= MAXY - 2; j++) {
            if (s[j - 1] == '1') {
                field[i][j] = FOOD;
            }
        }
    }
    field_hash = calc_hash();
    if (turn == 2) {
        char c;
        cin >> c;
        them.make_move(char_to_move[c]);
    }
    //freopen("log.txt", "w", stderr);
    while (true) {
        auto start = clock();
        //me.cells.print();
        cerr << "Calculating started" << endl;
        int res = me.make_move();
        /*if (field_hash != calc_hash()) {
            cerr << field_hash << ' ' << calc_hash() << endl;
            return 0;
        }*/
        if (res == NO_WAY) {
            cerr << "Cannot move" << endl;
            cerr << "Total size: " << me.cells.size() << endl;
            for (int i = 1; i <= MAXX - 2; i++) {
                for (int j = 1; j <= MAXY - 2; j++) {
                    cerr << field[i][j];
                }
                cerr << endl;
            }
            return 0;
        }
        //cerr << "stdout: " << move_to_char[res] << endl;
        cout << move_to_char[res] << endl;
        cerr << "Move made in " << clock() - start << " msc" << endl;
        char their_move;
        cin >> their_move;
        them.make_move(char_to_move[their_move]);
    }
}
