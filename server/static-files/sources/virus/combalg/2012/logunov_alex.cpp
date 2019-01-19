#pragma comment(linker, "/STACK:67108864")

#include <iostream>
#include <cstdio>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <complex>
#include <bitset>
#include <map>
#include <set>

#define forn(i, n) for(int i = 0; i < (int)(n); i++)
#define ford(i, n) for(int i = (int)(n) - 1; i >= 0; i--)
#define FORN(i, k, n) for(int i = (int)(k); i < (int)(n); i++)
#define FORD(i, n, k) for(int i = (int)(n) - 1; i >= (int)(k); i--)

#define vi vector<int>
#define pii pair<int, int>
#define all(x) (x).begin(), (x).end()
#define sqr(x) ((x)*(x))
#define fi first
#define se second

using namespace std;

typedef long long int64;

const long double pi = 3.1415926535897932384626433832795;
const long double eps = 0.0000001;
const int INF = 1E9;
const int MAXN = 10;
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

struct pt {
    int x, y;
    pt(){};
    pt(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

bool compare(pt a, pt b) {
    return (a.x < b.x || a.x == b.x && a.y < b.y);
}

bool compare2(pair<int, pt> a, pair<int, pt> b) {
    if (a.fi != b.fi) return a.fi < b.fi;
    return (a.se.x < b.se.x || a.se.x == b.se.x && a.se.y < b.se.y);
}

bool compare3(pair<pii, pt> a, pair<pii, pt> b) {
    if (a.fi.fi != b.fi.fi) return a.fi.fi < b.fi.fi;
    if (a.fi.se != b.fi.se) return a.fi.se < b.fi.se;
    return (a.se.x < b.se.x || a.se.x == b.se.x && a.se.y < b.se.y);
}

int turn, cur_step, matrix[MAXN][MAXN];
bool used[MAXN][MAXN];
string s;
bool no_viruses, time_to_attack;

int inverter(int x) {
    if (turn == 2)
        x = MAXN - 1 - x;
    return x;
}

string inverter(string s) {
    if (turn == 2)
        forn(i, s.size())
            s[i] = char('0' + (MAXN - 1 - (s[i] - '0')));
    return s;
}

bool correct(pt a) {
    return 0 <= a.x && a.x < MAXN && 0 <= a.y && a.y < MAXN;
}

bool is_near(pt a) {
    pt b;
    forn(i, 8) {
        b = pt(a.x + dx[i], a.y + dy[i]);
        if (correct(b) && matrix[b.x][b.y] == 1)
            return 1;
    }
    return 0;
}

void print_matrix() {
    ford(i, MAXN) {
        ford(j, MAXN)
            cerr << matrix[i][j];
        cerr << endl;
    }
}

void first_turn() {
    matrix[0][0] = matrix[0][1] = matrix[1][1] = 1;
    string s = "000111";
    cout << inverter(s) << endl;
    cout.flush();
    no_viruses = 0;
}

void dfs(pt a) {
    used[a.x][a.y] = 1;
    pt b;

    forn(i, 8) {
        b = pt(a.x + dx[i], a.y + dy[i]);
        if (correct(b) && !used[b.x][b.y]) {
            if (matrix[b.x][b.y] == 2)
                used[b.x][b.y] = 1;
            else if (matrix[b.x][b.y] == 1 || matrix[b.x][b.y] == 4)
                dfs(b);
        }            
    }
}

int dist_to_live(pt a) {
    int answer = INF;
    forn(i, MAXN)
        forn(j, MAXN)
            if (matrix[i][j] == 2 || matrix[i][j] == 3)
                answer = min(answer, max(abs(a.x - i), abs(a.y - j)));

    if (answer <= 2) {
        if (answer == 2 && !time_to_attack)
            answer = INF;
        if (answer == 1 && !time_to_attack && cur_step > 0) 
            answer = INF;
    }

    return answer;
}

int dist_to_kill(pt a) {
    int answer = INF;
    forn(i, MAXN)
        forn(j, MAXN)
            if (matrix[i][j] == 1 || matrix[i][j] == 3)
                answer = min(answer, max(abs(a.x - i), abs(a.y - j)));
    return answer;
}

int count_square(pt a) {
    matrix[a.x][a.y] = 1;

    bool mine[MAXN][MAXN];
    memset(mine, 0, sizeof(mine));
    forn(i, MAXN) {
        ford(j, MAXN)
            if (matrix[i][j] == 1) {
                forn(k, j + 1)
                    mine[i][k] = 1;
                break;
            }
    }
    forn(i, MAXN) {
        ford(j, MAXN)
            if (matrix[j][i] == 1) {
                forn(k, j + 1)
                    mine[k][i] = 1;
                break;
            }
    }

    int answer = 0;
    forn(i, MAXN)
        forn(j, MAXN)
            answer += mine[i][j];
    matrix[a.x][a.y] = 0;
    return answer;
}

//secret code:
//0 = free cell
//1 = my cell
//2 = opponent cell
//3 = my dead cell
//4 = opponent dead cell

void change(string s) {
    pt cur;
    for (int i = 0; i < int(s.size()); i += 2) {
        cur = pt(s[i] - '0', s[i + 1] - '0');
        if (matrix[cur.x][cur.y] == 1) {
            time_to_attack = 1;
            matrix[cur.x][cur.y] = 3; //killed
        } else
            matrix[cur.x][cur.y] = 2; //owned
    }
}

void next_step() {
    string s;
    pt cur;
    vector<pair<pii, pt> > operations0;
    vector<pair<int, pt> > operations1;
    operations0.clear();
    operations1.clear();

    //create
    forn(i, MAXN)
        forn(j, MAXN) {
            cur = pt(i, j);
            if (matrix[i][j] == 0 && is_near(cur))
                operations0.push_back(make_pair(make_pair(dist_to_live(cur), -count_square(cur)), cur));
        }
    random_shuffle(all(operations0));
    sort(all(operations0), compare3);

    //attack
    memset(used, 0, sizeof(used));
    forn(i, MAXN)
        forn(j, MAXN)
            if (matrix[i][j] == 1 && !used[i][j])
                dfs(pt(i, j));

    forn(i, MAXN)
        forn(j, MAXN) {
            cur = pt(i, j);
            if (used[i][j] && matrix[i][j] == 2)
                operations1.push_back(make_pair(dist_to_kill(cur), cur));
        }

    random_shuffle(all(operations1));
    sort(all(operations1), compare2);
    
    int pos = 0;
    if (!operations1.empty()) {
        cur = operations1[0].se;
        cout << inverter(cur.x) << inverter(cur.y);
        assert(matrix[cur.x][cur.y] == 2);
        matrix[cur.x][cur.y] = 4;
        return;
    }
    if (!operations0.empty()) {
        cur = operations0[0].se;
        cout << inverter(cur.x) << inverter(cur.y);
        assert(matrix[cur.x][cur.y] == 0);
        matrix[cur.x][cur.y] = 1;
    }
}

void go() {
    for (cur_step = 0; cur_step < 3; cur_step++)
        next_step();
    cout << endl;
    cout.flush();
}

int main() {

    //freopen("a.in", "r", stdin);
    //freopen("a.out", "w", stdout);

    no_viruses = 1;
    time_to_attack = 0;
    cin >> turn;
    if (turn == 1) {
        first_turn();
        //print_matrix();
    }    

    while (true) {
        cin >> s;

        if (s == "x")
            return 0;

        change(inverter(s));

        //print_matrix();

        //cerr << "wait..." << endl;
        
        if (no_viruses)
            first_turn();
        else
            go();

        //print_matrix();
    }

    return 0;
}
