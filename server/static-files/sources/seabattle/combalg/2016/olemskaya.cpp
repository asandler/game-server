#include<bits/stdc++.h>

#define x first
#define y second
#define pb push_back
#define forn(i, n) for (int i = 0; i < (int)(n); ++i)
#define all(a) (a).begin(), (a).end()
#define len(a) (int)(a).size()
#define pw(n) (1 << (n))
#define lst(a) a[len(a) - 1]
#define INF (int)(1e9)
//#define endl '\n'
//#define int long long
#define n 10
#define Cell pair<int, int>
#define elif else if

using namespace std;

struct Ship {
    int alive; /// кол-во целых клеток в этом корабле
    int orient; /// 0 - вертикальныя(1x1 тоже), 1 - горизонтальная
    int c, d1, d2;
    Ship() = default;
    friend iostream& operator>>(istream& stream, Ship &a) {
        char x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 == x2) {
            a.c = x1 - 'A';
            a.orient = 0;
            a.alive = abs(y1 - y2) + 1;
            a.d1 = min(y1, y2) - '0';
            a.d2 = max(y1, y2) - '0';
        }
        else {
            a.c = y1 - '0';
            a.orient = 1;
            a.alive = abs(x1 - x2) + 1;
            a.d1 = min(x1, x2) - 'A';
            a.d2 = max(x1, x2) - 'A';
        }
    }
};

vector<vector<int> > a, b;
/// -2 - туда не стреляли, 0..n - там кораблик и в него не попадали,
/// -3 - туда стреляли, но там ничего нет, -1 - туда стреляли, и попали в кораблик
vector<Ship> ships(n);
int visible_ship = 0;
int k1, k2;
int timer = 0;
vector<Cell> last_shots;

void print(vector<vector<int> > &a) {
    forn(i, n) {
        forn(j, n) {
            if (a[i][j] == -2) {
                //cerr << ".";
            }
            else if (a[i][j] == -1) {
                //cerr << "#";
            }
            else if (a[i][j] == -3) {
                //cerr << "*";
            }
            else {
                //cerr << a[i][j];
            }
        }
        //cerr << endl;
    }
}

void build_a_field() {
    ships.clear();
    a.clear();
    b.clear();
    ships.resize(n);
    a.resize(n, vector<int> (n, -2));
    b.resize(n, vector<int> (n, -2));
    forn(i, n) {
        cin >> ships[i];
        if (!ships[i].orient) {
            for (int j = ships[i].d1; j < ships[i].d2 + 1; ++j) {
                a[ships[i].c][j] = i;
            }
        }
        else {
            for (int j = ships[i].d1; j < ships[i].d2 + 1; ++j) {
                a[j][ships[i].c] = i;
            }
        }
    }
}

bool good(int x, int y) {
    return x > -1 && y > -1 && x < n && y < n;
}

bool good2(int x, int y, int k) {
    if (!good(x, y) || a[x][y] == -3) return 0;
    for (int dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            if (good(x + dx, y + dy) && (a[x + dx][y + dy] > -1 && a[x + dx][y + dy] != k || a[x + dx][y + dy] == -1)) {
                return 0;
            }
        }
    }
    return 1;
}

void do_a_move(int i, int j) {
    /*cout << "0X ";
    return;*/
    int k = a[i][j];
    if (ships[k].alive < abs(ships[k].d1 - ships[k].d2) + 1) {
        cout << "0X ";
        return;
    }
    for (int dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            int cnt = 0;
            if (!ships[k].orient) {
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    if (good2(ships[k].c + dx, d + dy, k) && abs(dx) != abs(dy) && !(ships[k].c + dx == i && d + dy == j)) {
                        cnt++;
                    }
                }
            }
            else {
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    if (good2(d + dx, ships[k].c + dy, k) && abs(dx) != abs(dy) && !(d + dx == i && ships[k].c + dy == j)) {
                        cnt++;
                    }
                }
            }
            if (cnt != ships[k].alive) continue;
            int c = ships[k].c;
            if (!ships[k].orient) {
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    a[c][d] = -2;
                }
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    a[c + dx][d + dy] = k;
                }
                ships[k].c += dx;
                ships[k].d1 += dy;
                ships[k].d2 += dy;
            }
            else {
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    a[d][c] = -2;
                }
                for (int d = ships[k].d1; d < ships[k].d2 + 1; ++d) {
                    a[d + dx][c + dy] = k;
                }
                ships[k].c += dy;
                ships[k].d1 += dx;
                ships[k].d2 += dx;
            }
            char ch = 'W';
            if (dx == 0 && dy == 1) ch = 'N';
            elif (dx == 0 && dy == -1) ch = 'S';
            elif (dx == 1 && dy == 0) ch = 'E';
            cout << k << ch << ' ';
            return;
        }
    }
    cout << "0X ";
}

void do_a_move() {
    cout << "0X ";
    /*while (1) {
        int k = rand() % n;
        if (ships[k].alive == 0) continue;
        if (!ships[k].orient) do_a_move(ships[k].c, ships[k].d1);
        else do_a_move(ships[k].d1, ships[k].c);
        return;
    }*/
}

void print_a_ship() {
    /// тут должно быть что-то поумнее: выбирать подбитый кораблик(visible ship)
    forn(i, n) {
        forn(j, n) {
            if (a[i][j] > -1 && ships[a[i][j]].alive != abs(ships[a[i][j]].d2 - ships[a[i][j]].d1) + 1) {
                //cerr << char(i + 'A') << char(j + '0');
                cout << char(i + 'A') << char(j + '0');
                return;
            }
        }
    }
    forn(k, n) {
        //int k = rand() % n;
        if (ships[k].alive == 0) continue;
        if (!ships[k].orient) cout << char(ships[k].c + 'A') << char(ships[k].d1 + '0');
        else cout << char(ships[k].d1 + 'A') << char(ships[k].c + '0');
        return;
    }

}

Cell do_a_shot() {
    timer++;
    forn(i, n) {
        forn(j, n) {
            if (b[i][j] == -1) {
                for (int dx = -1; dx < 2; ++dx) {
                    for (int dy = -1; dy < 2; ++dy) {
                        if (good(i + dx, j + dy) && abs(dx) != abs(dy) && good(i - dx, j - dy)) {
                            if (b[i + dx][j + dy] == -2 && b[i - dx][j - dy] == -1) {
                                cout << char('A' + i + dx) << char('0' + j + dy);
                                return {i + dx, j + dy};
                            }
                        }
                    }
                }
            }
        }
    }

    forn(i, n) {
        forn(j, n) {
            if (b[i][j] == -1) {
                /// если не нашли -##
                for (int dx = -1; dx < 2; ++dx) {
                    for (int dy = -1; dy < 2; ++dy) {
                        if (good(i + dx, j + dy) && abs(dx) != abs(dy)) {
                            if (b[i + dx][j + dy] == -2) {
                                cout << char('A' + i + dx) << char('0' + j + dy);
                                return {i + dx, j + dy};
                            }
                        }
                    }
                }
            }
        }
    }
    if (timer % 4 < 3) {
        while (!last_shots.empty()) {
            Cell q = last_shots[len(last_shots) - 1];
            last_shots.pop_back();
            if (b[q.x][q.y] == -2) {
                cout << char('A' + q.x) << char('0' + q.y);
                return q;
            }
        }

    }

    int k = 0;
    while (k < 100) {
        int i = (abs(rand())) % n;
        int j = (abs(rand())) % n;
        if (b[i][j] == -2) {
            cout << char(i + 'A') << char(j + '0');
            return {i, j};
        }
        k++;
    }
    forn(i, n) {
        forn(j, n) {
            if (b[i][j] == -2) {
                cout << char('A' + i) << char(j + '0');
                return {i, j};
            }
        }
    }
}

Cell read_an_answer() {
    string s;
    cin >> s;
    if (len(s) == 1) {
        if (s == "H") {
            return {-1, -1};
        }
        else {
            k2--;
            return {-2, -2};
        }
    }
    else {
        last_shots.pb({s[0] - 'A', s[1] - '0'});
        return {(int)(s[2] - 'A'), (int)(s[3] - '0')};
    }
}

void print_ships() {
    forn(i, n) cerr << ships[i].alive << ' ';
    cerr << endl;
}

void dfs_to_convex(int i, int j) {
    b[i][j] = -3;
    for (int dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            if (good(i + dx, j + dy)) {
                if (b[i + dx][j + dy] == -1) {
                    dfs_to_convex(i + dx, j + dy);
                }
                else {
                    b[i + dx][j + dy] = -3;
                }
            }
        }
    }
}

void solve() {
    build_a_field();
    int tmp;
    cin >> tmp;
    k1 = 10, k2 = 10;
    bool fl = 1, fl2 = 1;
    while (1) {
        cerr << 1 << endl;
        print_ships();
        Cell my_shot;
        //cerr << " Alexandra " << endl;
        print(a);
        //cerr << " Test " << endl;
        print(b);
        //cerr << "----------------------------------------\n";
        if (!tmp) {
            //cerr << "!!!!!!!!!!!!!!!!!!!!!!\n";
            if (fl2) do_a_move();
            else fl2 = 1;
            print_a_ship();
            my_shot = do_a_shot();
            //cerr << "eurioefeofeojgh\n";
            cout << endl;
        }
        auto ans = read_an_answer();
        while (ans.x > -1 && a[ans.x][ans.y] > -1) {
            do_a_move(ans.x, ans.y);
            if (a[ans.x][ans.y] < 0) {
                fl2 = 0;
                break;
            }
            cerr << 2 << endl;
            print_ships();
            //cerr << " Alexandra " << endl;
            print(a);
            //cerr << "----------------------------------------\n";
            if (ships[a[ans.x][ans.y]].alive == 1) {
                cout << "K";
                cout << endl;
                ships[a[ans.x][ans.y]].alive = 0;
                a[ans.x][ans.y] = -1;
                ans = read_an_answer();
                k1--;

            }
            else {
                cout << "H";
                cout << endl;
                ships[a[ans.x][ans.y]].alive--;
                a[ans.x][ans.y] = -1;
                ans = read_an_answer();
            }
        }
        if (ans.x > -1) { /// если противник попал не в кораблик
            a[ans.x][ans.y] = -3;
        }
        if (!tmp) {
            if (ans.x == -1) {
                b[my_shot.x][my_shot.y] = -1;
            }
            else if (ans.x == -2) {
                b[my_shot.x][my_shot.y] = -1;
                dfs_to_convex(my_shot.x, my_shot.y);
            }
            else {
                b[my_shot.x][my_shot.y] = -3;
            }
        }
        else {
            tmp = 0;
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    ///freopen(".in", "r", stdin);
    ///freopen(".out", "w", stdout);
    srand(time(0));
    solve();
    return 0;
}
