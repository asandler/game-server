#include <iostream>
#include <algorithm>

using namespace std;

struct data {
    int8_t me[10][10],///-1 - nothing, 0-9 - ship, 10-19 - bursted ship, -2 - mine
    enemy[10][10],///0 - nothing, -1 - miss/no ship, -2 - spoted
    ships[10] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4},
    spoted[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    s_pos[10][4], dx[8] = {-1, 0, 1, 0, -1, 1, -1, 1}, dy[8] = {0, -1, 0, 1, 1, 1, -1, -1}, me_total = 20, enemy_total = 20, last_s[2];
    data() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                me[i][j] = -1;
                enemy[i][j] = 0;
            }
        }
    }
    bool move(int id, int dir, bool ph) {
        int nw[4];
        nw[0] = s_pos[id][0] + dx[dir];
        nw[1] = s_pos[id][1] + dy[dir];
        nw[2] = s_pos[id][2] + dx[dir];
        nw[3] = s_pos[id][3] + dy[dir];
        if (nw[0] < 0 || nw[1] < 0 || nw[2] >= 10 || nw[3] >= 10)
            return false;
        for (int i = max(0, nw[0] - 1); i < min(nw[2] + 2, 10); i++) {
            //cerr << i << ' ';
            for (int j = max(0, nw[1] - 1); j < min(nw[3] + 2, 10); j++) {
                if (me[i][j] != -1 && me[i][j] != -2 && me[i][j] != id)
                    return false;
            }
        }
        for (int i = nw[0]; i <= nw[2]; i++)
        for (int j = nw[1]; j <= nw[3]; j++)
            if (me[i][j] != -1 && me[i][j] != id)
                return false;
        if (ph)
            return true;
        for (int i = s_pos[id][0]; i <= s_pos[id][2]; i++)
        for (int j = s_pos[id][1]; j <= s_pos[id][3]; j++)
            me[i][j] = -1;
        for (int i = nw[0]; i <= nw[2]; i++)
        for (int j = nw[1]; j <= nw[3]; j++)
            me[i][j] = id;
        for (int i = 0; i < 4; i++)
            s_pos[id][i] = nw[i];
        return true;
    }
};

struct data1: public data {
    int step = 0;
};

struct player {
    data1 *a;
    player() {
        a = new data1();
    }
    void init(string &in) {
        //cerr << in << endl;
        for (int i = 0; i < 10; i ++){
            a->s_pos[i][0] = in[i * 5] - 'A';
            a->s_pos[i][1] = in[i * 5 + 1] - '0';
            a->s_pos[i][2] = in[i * 5 + 2] - 'A';
            a->s_pos[i][3] = in[i * 5 + 3] - '0';
            if (a->s_pos[i][0] > a->s_pos[i][2]) swap(a->s_pos[i][0], a->s_pos[i][2]);
            if (a->s_pos[i][1] > a->s_pos[i][3]) swap(a->s_pos[i][1], a->s_pos[i][3]);
            //for (int k = 0; k < 4; k++)
            //    cerr << int(a->s_pos[i][k]) << ' ';
            //cerr << endl;
            for (int ii = a->s_pos[i][0]; ii <= a->s_pos[i][2]; ii++)
            for (int jj = a->s_pos[i][1]; jj <= a->s_pos[i][3]; jj++)
                a->me[ii][jj] = i;
            a->ships[i] = (a->s_pos[i][2] - a->s_pos[i][0] + 1) * (a->s_pos[i][3] - a->s_pos[i][1] + 1);
        }

        /*for (int i = 0; i < 10; i++) {

        for (int j = 0; j < 10; j++)
            cerr << char((a->me[i][j] == -1) ? '.' : a->me[i][j] + '0') << ' ';
        cerr << endl;
        }*/
    }
    bool left_undone() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (a->enemy[i][j] == -2)
                    return true;
        return false;
    }

    string finish_undone() {
        string res(2, 0);
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
            if (a->enemy[i][j] == -2) {
                res[0] = i;
                res[1] = j;
            }
        for (int d : {0, 1}) {
            if (0 <= res[0] + a->dx[d] && 0 <= res[1] + a->dy[d] &&
                a->enemy[res[0] + a->dx[d]][res[1] + a->dy[d]] == -2) {
                if (res[0] - a->dx[d] < 10 && res[1] - a->dy[d] < 10 &&
                    a->enemy[res[0] - a->dx[d]][res[1] - a->dy[d]] != -1) {
                    res[0] += 'A' - a->dx[d];
                    res[1] += '0' - a->dy[d];
                    return res;
                }
                for (int l = 1; 0 <= res[0] + a->dx[d] * l && 0 <= res[1] + a->dy[d] * l; l++) {
                    if (a->enemy[res[0] + a->dx[d] * l][res[1] + a->dy[d] * l] != -2) {
                            res[0] += 'A' + a->dx[d] * l;
                            res[1] += '0' + a->dy[d] * l;
                            return res;
                    }
                }
            }
        }
        for (int d = 0; d < 4; d++)
            if (0 <= res[0] + a->dx[d] && res[0] + a->dx[d] < 10 && 0 <= res[1] + a->dy[d] && res[1] + a->dy[d] < 10 &&
                a->enemy[res[0] + a->dx[d]][res[1] + a->dy[d]] != -1) {
            res[0] += 'A' + a->dx[d];
            res[1] += '0' + a->dy[d];
            return res;
            }
    }
    string Random_shoot() {
        string res;
        vector <pair<int, int> > tar;
        for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++) {
            if (!tar.empty() && a->enemy[tar.back().first][tar.back().second] < a->enemy[i][j])
                tar.clear();
            if (tar.empty() || a->enemy[i][j] == a->enemy[tar.back().first][tar.back().second])
                tar.push_back({i, j});
        }
        int choice = rand() % tar.size();
        res = "A0";
        res[res.size() - 2] += tar[choice].first;
        res[res.size() - 1] += tar[choice].second;
        return res;
    }
    string move(const string &in) {
        a->step++;
        string res = "0X ";
        if (in.size() > 1) {///enemy shoot
            a->enemy[in[0] - 'A'][in[1] - '0'] = a->step;
            int b_d, b_n = -1, c_n, att = a->me[in[2] - 'A'][in[3] - '0'];
            if (att < 0 || 10 <= att) { ///No result
                res = "0X ";
            } else {
                //cerr << "ESC " << att << endl;
                for (int dir = 0; dir < 4; dir++) {
                    //cerr << "CD " << dir << endl;
                    if (a->move(att, dir, false)) {
                        //cerr << "CD " << dir << ' ';
                        if (a->me[in[2] - 'A'][in[3] - '0'] == -1) {
                            c_n = 0;
                            for (int ddir = 0; ddir < 4; ddir++)
                                if (a->move(att, ddir, true))
                                    c_n++;
                            //cerr << c_n;
                            if (c_n > b_n) {
                                //cerr << " NB";
                                b_n = c_n;
                                b_d = dir;
                            }
                        }
                        //cerr << endl;
                        a->move(att, (dir + 2) % 4, false);
                    }
                }
                if (b_n == -1 || a->spoted[att] == 1) {
                    a->ships[att]--;
                    a->me_total--;
                    a->spoted[att] = 0;
                    a->me[in[2] - 'A'][in[3] - '0'] += 10;
                    if (a->ships[att] == 0)
                        return "0X K";
                    else
                        return "0X H";
                } else {
                    a->move(att, b_d, false);
                    res = "00 ";
                    res[0] += att;
                    switch(b_d) {
                    case 0:
                        res[1] = 'W';
                        break;
                    case 1:
                        res[1] = 'S';
                        break;
                    case 2:
                        res[1] = 'E';
                        break;
                    case 3:
                        res[1] = 'N';
                        break;
                    }
                }
            }
            if (a->me[in[2] - 'A'][in[3] - '0'] == -1)
                a->me[in[2] - 'A'][in[3] - '0'] = -2;
        } else {
            if (in != "X") {
                a->enemy_total--;
                a->enemy[a->last_s[0]][a->last_s[1]] = -2;
                for (int k = 4; k < 8; k++)
                if (0 <= a->last_s[0] + a->dx[k] && a->last_s[0] + a->dx[k] < 10 &&
                    0 <= a->last_s[1] + a->dy[k] && a->last_s[1] + a->dy[k] < 10)
                    a->enemy[a->last_s[0] + a->dx[k]][a->last_s[1] + a->dy[k]] = -1;
                if (in == "K") {
                    for (int k = 0; k < 4; k++) {
                        bool exit = false;
                        for (int len = 1; 0 <= a->last_s[0] + a->dx[k] * len && a->last_s[0] + a->dx[k] * len < 10 &&
                                          0 <= a->last_s[1] + a->dy[k] * len && a->last_s[1] + a->dy[k] * len < 10 && !exit; len++) {
                            if (a->enemy[a->last_s[0] + a->dx[k] * len][a->last_s[1] + a->dy[k] * len] != -2) {
                                exit = true;
                            }
                            a->enemy[a->last_s[0] + a->dx[k] * len][a->last_s[1] + a->dy[k] * len] = -1;
                        }
                    }
                    a->enemy[a->last_s[0]][a->last_s[1]] = -1;
                }
            }
            if (a->enemy_total == 0)
                return "";
        }
        int resi, resj, n_res = 1e9;
        for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++) {
            if (a->me[i][j] >= 0 && a->me[i][j] < 10 && a->ships[a->me[i][j]] < n_res) {
                n_res = a->ships[a->me[i][j]];
                resi = i;
                resj = j;
            }
        }
        res += "A0";
        res[res.size() - 2] += resi;
        res[res.size() - 1] += resj;
        if (left_undone()) {
            res += finish_undone();
        } else {
            res += Random_shoot();
        }
        a->last_s[0] = res[res.size() - 2] - 'A';
        a->last_s[1] = res[res.size() - 1] - '0';
        a->enemy[a->last_s[0]][a->last_s[1]] = -1;
        return res;
    }
};

void out(data *a) {
    cerr << "  ";
    for (int i = 0; i < 10; i++)
        cerr << i << ' ';
    cerr << "  ";
    for (int i = 0; i < 10; i++)
        cerr << i << ' ';
    cerr << endl;
    for (int i = 0; i < 10; i++) {
        cerr << char(i + 'A');
        for (int j = 0; j < 10; j++) {
            if (a->me[i][j] >= -1 && a->me[i][j] < 10)
                cerr << ' ';
            if (a->me[i][j] == -1)
                cerr << '.';
            else
                cerr << int(a->me[i][j]);
        }
        cerr << "  ";
        for (int j = 0; j < 10; j++) {

            if (a->enemy[i][j] >= 0)
                cerr << ' ';
            cerr << int(a->enemy[i][j]);
        }
        cerr << endl;
    }/*
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < 10; i++)
            cerr << (a->move(i, k, true)) << ' ';
        cerr << endl;
    }*/
    cerr << endl;
}

int main() {
    srand(0);
    player A;
    string cur, ccur;
    for (int i = 0; i < 10; i++) {
        cin >> ccur;
        cur += ccur + ' ';
    }
    A.init(cur);
    cin >> cur;
    if (cur == "0") {
        cout << A.move("X") << endl;
        out(A.a);
    }
    while(cin >> cur) {
        cerr << "IN: " << cur << endl;
        cur = A.move(cur);
        cerr << "OUT: " << cur << endl;
        out(A.a);
        cout << cur << endl;
        if (A.a->me_total == 0 || A.a->enemy_total == 0)
            return 0;
    }
    return 0;
}

