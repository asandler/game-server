#include <bits/stdc++.h>
using namespace std;
#define DB(v) cerr << #v << ' ' << v << endl
#define sz(v) int(v.size())
#define For(i, a, b) for(int i = a;i <= b; ++i)
#define fi first
#define se second

typedef pair <int,int> pii;
const int Bomb = 1e9, S = 10, N = 10, Killed = 1e8;
string translator = "ABCDEFGHIJ";

vector <int> priorities = {4, 5, 3, 6, 2, 7, 1, 8, 0, 9};

int place_condition[N][N];
vector <int> remain (N, 0);
int target[N][N];
int revealed_line = 5, revealed_column = 5;
int shoot_line, shoot_column;

pii add_pair(pii object, int first_add, int second_add) {
    return make_pair(object.fi + first_add, object.se + second_add);
}

vector <vector <pii> > battleships (N);
vector <int> need_to_remain = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
map  <pii, char>  direction;
set <pii> his_wounds;

bool it_is_ship(int x, int y) {
    if(place_condition[x][y] != -1 && place_condition[x][y] != Bomb) return true;
    return false;
}

bool it_is_ship(int number) {
    if(number == -1 || number == Bomb) return false;
    return true;
}

int translate_for_us(char a) {
    For(i, 0, sz(translator) - 1) {
        if(translator[i] == a) return i;
    }
    return a - '0';
}

void parsing(string s, int &a, int &b, int &c, int &d) {
    a = translate_for_us(s[0]), b = translate_for_us(s[1]);
    c = translate_for_us(s[2]), d = translate_for_us(s[3]);
}

bool in_field(int x, int y) {
    if(x < 0 || x > 9) return false;
    if(y < 0 || y > 9) return false;
    return true;
}

bool valid(int x, int y, int number)
{
    if(!in_field(x, y)) return false;
    if(place_condition[x][y] == Bomb) return false;
    For(xx, -1, 1) {
        For(yy, -1, 1) {
            if(in_field(x + xx, y + yy) && it_is_ship(x + xx, y + yy) && place_condition[x+xx][y+yy] != number)
                return false;
        }
    }
    return true;
}


string move_result (int number, int line_add, int column_add, int bomb_line, int bomb_column)
{
    vector <pii> decks = battleships[number];
    if(remain[number] < need_to_remain[number])
        return "Incorrect move";

    For(i, 0, sz(decks) - 1) {
        if(!valid(decks[i].fi + line_add, decks[i].se + column_add, number)) {
            return "Incorrect move";
        }
    }

    For(i, 0, sz(decks) - 1) {
        if(bomb_line == decks[i].fi + line_add && bomb_column == decks[i].se + column_add) {
            return "Without change";
        }
    }
    return "Good";
}


int places_to_go(int number) {
    int ans = 0;
    For(xx, -1, 1) {
        For(yy, -1, 1) {
            if(abs(xx + yy) == 1 && move_result(number, xx, yy, -1, -1) == "Good")
                ans++;
        }
    }
    return ans;
}

void read(int number)
{
    string s; cin >> s;
    int first_line, first_column, second_line, second_column;
    parsing(s, first_column, first_line, second_column, second_line);

    if(first_line == second_line) {
        For(column, min(first_column, second_column), max(first_column, second_column)) {
            remain[number]++;
            place_condition[first_line][column] = number;
            battleships[number].push_back(make_pair(first_line, column));
        }
    }
    else {
        For(line, min(first_line, second_line), max(first_line, second_line)) {
            remain[number]++;
            place_condition[line][first_column] = number;
            battleships[number].push_back(make_pair(line, first_column));
        }
    }
}

pii find_alive_ship()
{
    int chosen_number = 0;
    For(i, 0, 9) {
        if(places_to_go(i) > places_to_go(chosen_number)) {
            chosen_number = i;
        }
    }
    for(auto to: battleships[chosen_number]) {
        if(it_is_ship(to.fi, to.se) && place_condition[to.fi][to.se] != Killed)
            return to;
    }
    For(i, 0, 9 )  {
        for(auto to: battleships[i]) {
            if(it_is_ship(to.fi, to.se) && place_condition[to.fi][to.se] != Killed)
                return to;
        }
    }
}

void shoot(int his_line, int his_column) {
    pii our_ship = find_alive_ship();
    cout << translator[our_ship.se] << our_ship.fi << translator[his_column] << his_line;
    shoot_line = his_line, shoot_column =  his_column;
    target[his_line][his_column] = Bomb;
}

bool upbeat(int wound_line, int wound_column)
{
    For(xx, -1, 1) {
        For(yy, -1, 1) {
            int cur_line = wound_line + xx, cur_column = wound_column + yy;
            if(abs(xx + yy) == 1 && in_field(cur_line, cur_column) && target[cur_line][cur_column] == 0) {
                shoot(cur_line, cur_column);
                return true;
            }
        }
    }
    return false;
}


bool try_upbeat() {
    for(auto to: his_wounds) {
        if(upbeat(to.fi, to.se)) {
            return true;
        }
    }
    return false;
}

void shoot_randomly() {
    for(int i: priorities) {
        for(int j: priorities) {
            if(target[i][j] == 0) {
                shoot(i, j);
                return;
            }
        }
    }
}

void move_ship(int number, int line_add, int column_add)
{
        vector <pii> decks = battleships[number];
        vector <pii> new_decks (sz(decks));
        For(i, 0, sz(decks) - 1) {
            new_decks[i] = add_pair(decks[i], line_add, column_add);
            place_condition[decks[i].fi][decks[i].se] = -1;
        }
        For(i, 0, sz(new_decks) - 1) {
            place_condition[new_decks[i].fi][new_decks[i].se] = number;
        }
        battleships[number] = new_decks;
}

void get_his_move();

void make_move(int number, char where_fleed, char condition, int bomb_line, int bomb_column)
{
    if(bomb_line != -1) {
        if(condition == 'n')
            place_condition[bomb_line][bomb_column] = Bomb;
        else
            place_condition[bomb_line][bomb_column] = Killed;
    }
    cout << number << where_fleed << ' ';
    if(condition == 'n') {
        if(revealed_line == -1) {
            if(!try_upbeat())
                shoot_randomly();
        }
        else
            shoot(revealed_line, revealed_column);
    }
    else
        cout << condition;

    cout << endl;
    get_his_move();
}

void get_benefits(int line, int column, int prev_line = -1, int prev_column = -1)
{
    For(xx, -1, 1) {
        For(yy, -1, 1) {
            int cur_line = line + xx, cur_column = column + yy;
            if(!in_field(cur_line, cur_column)) continue;
            if(abs(xx + yy) == 1 && target[cur_line][cur_column] == Killed && make_pair(cur_line, cur_column) != make_pair(prev_line, prev_column)) {
                get_benefits(cur_line, cur_column, line, column);
            }
            if(target[cur_line][cur_column] == 0)
                target[cur_line][cur_column] = Bomb;
        }
    }
}

void get_his_move() {
    revealed_line = -1;
    string his_move;
    cin >> his_move;

    if(his_move == "K" || his_move == "H") {
        target[shoot_line][shoot_column] = Killed;
        if(his_move == "K")
            get_benefits(shoot_line, shoot_column);
        else
            his_wounds.insert(make_pair(shoot_line, shoot_column));
        make_move(0, 'X', 'n', -1, -1);
    }
    else {
        int his_point_line, his_point_column, point_line, point_column;
        parsing(his_move, his_point_column, his_point_line, point_column, point_line);
        int number = place_condition[point_line][point_column];
        revealed_line = his_point_line, revealed_column = his_point_column;
        if(!it_is_ship (number)) {
            make_move(0, 'X', 'n', point_line, point_column);
        }
        else {
            bool move_done = false;
            for(auto to: direction) {
                if(move_result(number, to.fi.fi, to.fi.se, point_line, point_column) == "Good") {
                    move_ship(number, to.fi.fi, to.fi.se);
                    make_move(number, to.se, 'n', point_line, point_column);
                    move_done = true;
                }
            }

            if(!move_done) {
                remain[number]--;
                if(remain[number] == 0)
                    make_move(0, 'X', 'K', point_line, point_column);
                else
                    make_move(0, 'X', 'H', point_line, point_column);
            }
        }
    }
}

void prepare() {
    memset(place_condition, -1, sizeof(place_condition));
    For(i, 0, S-1) read(i);
    direction[{0, 1}] = 'E';
    direction[{0, -1}] = 'W';
    direction[{1, 0}] = 'N';
    direction[{-1, 0}] = 'S';
}

int main()
{
#ifdef HOME
    //freopen("input.txt","r", stdin);
#endif
    prepare();
    bool not_move; cin >> not_move;
    if(not_move)
        get_his_move();
    else
        make_move(0, 'X', 'n', -1, -1);
    return 0;
}
