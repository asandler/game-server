#include <iostream>
#include <vector>

using namespace std;

enum cell {
    voit,
    wite,
    blak
};

class Part {
public:
    vector<vector<cell>> d;
    int v;
    Part(): d() {}
    Part(vector<vector<char>> q) {
        d.resize(q.size(), vector<cell>(q.size()));
        for (int i = 0; i < q.size(); ++i) {
            for (int j = 0; j < q[i].size(); ++j) {
                if (q[i][j] == '.') {
                    d[i][j] = voit;
                    v++;
                } else if (q[i][j] == 'B') {
                    d[i][j] = blak;
                } else {
                    d[i][j] = wite;
                }
            }
        }
    }

    vector<int> move(int x, int y, cell q) {
        vector<int> ans;
        if (d[x][y] != voit) {
            return ans;
        }
        for (int l = 1; (x - l >= 0) && (y < d.size()); ++l) {
            if (d[x - l][y] == q) {
                if (l != 1) {
                    ans.push_back(0);
                }
                break;
            }
            if (d[x - l][y] == voit) {
                break;
            }
        }
        for (int l = 1; (x - l >= 0) && (y + l < d.size()); ++l) {
            if (d[x - l][y + l] == q) {
                if (l != 1) {
                    ans.push_back(1);
                }
                break;
            }
            if (d[x - l][y + l] == voit) {
                break;
            }
        }
        for (int l = 1; (x >= 0) && (y + l < d.size()); ++l) {
            if (d[x][y + l] == q) {
                if (l != 1) {
                    ans.push_back(2);
                }
                break;
            }
            if (d[x][y + l] == voit) {
                break;
            }
        }
        for (int l = 1; (x + l < d.size()) && (y + l < d.size()); ++l) {
            if (d[x + l][y + l] == q) {
                if (l != 1) {
                    ans.push_back(3);
                }
                break;
            }
            if (d[x + l][y + l] == voit) {
                break;
            }
        }
        for (int l = 1; (x + l < d.size()) && (y < d.size()); ++l) {
            if (d[x + l][y] == q) {
                if (l != 1) {
                    ans.push_back(4);
                }
                break;
            }
            if (d[x + l][y] == voit) {
                break;
            }
        }
        for (int l = 1; (x + l < d.size()) && (y - l >= 0); ++l) {
            if (d[x + l][y - l] == q) {
                if (l != 1) {
                    ans.push_back(5);
                }
                break;
            }
            if (d[x + l][y - l] == voit) {
                break;
            }
        }
        for (int l = 1; (x < d.size()) && (y - l >= 0); ++l) {
            if (d[x][y - l] == q) {
                if (l != 1) {
                    ans.push_back(6);
                }
                break;
            }
            if (d[x][y - l] == voit) {
                break;
            }
        }
        for (int l = 1; (x - l >= 0) && (y - l >= 0); ++l) {
            if (d[x - l][y - l] == q) {
                if (l != 1) {
                    ans.push_back(7);
                }
                break;
            }
            if (d[x - l][y - l] == voit) {
                break;
            }
        }
        return ans;
    }

    Part fill(int x, int y, Part p, int vec, cell q) {
        if (vec == 0) {
            int l = 0;
            while (p.d[x - l][y] != q) {
                p.d[x - l][y] = q;
                l++;
            }
        }
        if (vec == 1) {
            int l = 0;
            while (p.d[x - l][y + l] != q) {
                p.d[x - l][y + l] = q;
                l++;
            }
        }
        if (vec == 2) {
            int l = 0;
            while (p.d[x][y + l] != q) {
                p.d[x][y + l] = q;
                l++;
            }
        }
        if (vec == 3) {
            int l = 0;
            while (p.d[x + l][y + l] != q) {
                p.d[x + l][y + l] = q;
                l++;
            }
        }
        if (vec == 4) {
            int l = 0;
            while (p.d[x + l][y] != q) {
                p.d[x + l][y] = q;
                l++;
            }
        }
        if (vec == 5) {
            int l = 0;
            while (p.d[x + l][y - l] != q) {
                p.d[x + l][y - l] = q;
                l++;
            }
        }
        if (vec == 6) {
            int l = 0;
            while (p.d[x][y - l] != q) {
                p.d[x][y - l] = q;
                l++;
            }
        }
        if (vec == 7) {
            int l = 0;
            while (p.d[x - l][y - l] != q) {
                p.d[x - l][y - l] = q;
                l++;
            }
        }
        return p;
    }

    void fillt(int x, int y, int vec, cell q) {
        if (vec == 0) {
            int l = 0;
            while (this->d[x - l][y] != q) {
                this->d[x - l][y] = q;
                l++;
            }
        }
        if (vec == 1) {
            int l = 0;
            while (this->d[x - l][y + l] != q) {
                this->d[x - l][y + l] = q;
                l++;
            }
        }
        if (vec == 2) {
            int l = 0;
            while (this->d[x][y + l] != q) {
                this->d[x][y + l] = q;
                l++;
            }
        }
        if (vec == 3) {
            int l = 0;
            while (this->d[x + l][y + l] != q) {
                this->d[x + l][y + l] = q;
                l++;
            }
        }
        if (vec == 4) {
            int l = 0;
            while (this->d[x + l][y] != q) {
                this->d[x + l][y] = q;
                l++;
            }
        }
        if (vec == 5) {
            int l = 0;
            while (this->d[x + l][y - l] != q) {
                this->d[x + l][y - l] = q;
                l++;
            }
        }
        if (vec == 6) {
            int l = 0;
            while (this->d[x][y - l] != q) {
                this->d[x][y - l] = q;
                l++;
            }
        }
        if (vec == 7) {
            int l = 0;
            while (this->d[x - l][y - l] != q) {
                this->d[x - l][y - l] = q;
                l++;
            }
        }
    }

    vector<pair<Part, pair<int, int>>> generate_particles(cell q) {
        vector<pair<Part, pair<int, int>>> ans;
        for (int i = 0; i < d.size(); ++i) {
            for (int j = 0; j < d.size(); ++j) {
                auto p = this->move(i, j, q);
                if (p.size() == 0) {
                    continue;
                }
                for (auto w : p) {
                    ans.push_back(make_pair(fill(i, j, *this, w, q), make_pair(i, j)));
                }
            }
        }
        return ans;
    }

    void idti(int x, int y, cell q) {
        vector<int> mv = this->move(x, y, q);
        this->d[x][y] = q;
        for (auto p : mv) {
            this->fillt(x, y, p, q);
        }
    }

    void print() {
        for (auto raw : this->d) {
            for (auto p : raw) {
                cerr << p << " ";
            }
            cerr << endl;
        }
    }
};

cell r(cell q) {
    if (q == blak) {
        return wite;
    }
    return blak;
}

int heuristic(Part p, cell q) {
    int score = 0;
    int angle = 10;
    int bar = 5;
    int cell = 1;
    if (p.d[0][0] == q) {
        score += angle * (p.d[0][0] == q);
        score += angle * (p.d[1][0] == q);
        score += angle * (p.d[0][1] == q);
        score += angle * (p.d[1][1] == q);
        score -= angle * (p.d[0][0] == r(q));
        score -= angle * (p.d[1][0] == r(q));
        score -= angle * (p.d[0][1] == r(q));
        score -= angle * (p.d[1][1] == r(q));
    } else {
        score += angle * (p.d[0][0] == q);
        score -= angle * (p.d[1][0] == q);
        score -= angle * (p.d[0][1] == q);
        score -= angle * (p.d[1][1] == q);
        score -= angle * (p.d[0][0] == r(q));
        score += angle * (p.d[1][0] == r(q));
        score += angle * (p.d[0][1] == r(q));
        score += angle * (p.d[1][1] == r(q));
    }
    if (p.d[0][7] == q) {
        score += angle * (p.d[0][7] == q);
        score += angle * (p.d[1][7] == q);
        score += angle * (p.d[0][6] == q);
        score += angle * (p.d[1][6] == q);
        score -= angle * (p.d[0][7] == r(q));
        score -= angle * (p.d[1][7] == r(q));
        score -= angle * (p.d[0][6] == r(q));
        score -= angle * (p.d[1][6] == r(q));
    } else {
        score += angle * (p.d[0][7] == q);
        score -= angle * (p.d[1][7] == q);
        score -= angle * (p.d[0][6] == q);
        score -= angle * (p.d[1][6] == q);
        score -= angle * (p.d[0][7] == r(q));
        score += angle * (p.d[1][7] == r(q));
        score += angle * (p.d[0][6] == r(q));
        score += angle * (p.d[1][6] == r(q));
    }
    if (p.d[7][0] == q) {
        score += angle * (p.d[7][0] == q);
        score += angle * (p.d[6][0] == q);
        score += angle * (p.d[7][1] == q);
        score += angle * (p.d[6][1] == q);
        score -= angle * (p.d[7][0] == r(q));
        score -= angle * (p.d[6][0] == r(q));
        score -= angle * (p.d[7][1] == r(q));
        score -= angle * (p.d[6][1] == r(q));
    } else {
        score += angle * (p.d[7][0] == q);
        score -= angle * (p.d[6][0] == q);
        score -= angle * (p.d[7][1] == q);
        score -= angle * (p.d[6][1] == q);
        score -= angle * (p.d[7][0] == r(q));
        score += angle * (p.d[6][0] == r(q));
        score += angle * (p.d[7][1] == r(q));
        score += angle * (p.d[6][1] == r(q));
    }
    if (p.d[7][7] == q) {
        score += angle * (p.d[7][7] == q);
        score += angle * (p.d[6][7] == q);
        score += angle * (p.d[7][6] == q);
        score += angle * (p.d[6][6] == q);
        score -= angle * (p.d[7][7] == r(q));
        score -= angle * (p.d[6][7] == r(q));
        score -= angle * (p.d[7][6] == r(q));
        score -= angle * (p.d[6][6] == r(q));
    } else {
        score += angle * (p.d[7][7] == q);
        score -= angle * (p.d[6][7] == q);
        score -= angle * (p.d[7][6] == q);
        score -= angle * (p.d[6][6] == q);
        score -= angle * (p.d[7][7] == r(q));
        score += angle * (p.d[6][7] == r(q));
        score += angle * (p.d[7][6] == r(q));
        score += angle * (p.d[6][6] == r(q));
    }
    for (int i = 0; i < p.d.size(); ++i) {
        for (int j = 0; j < p.d[i].size(); ++j) {
            if ((i <= 1 || i >= 6) && (j <= 1 || j >= 6)) {
                continue;
            } else if (i == 0 || i == 7 || j == 0 || j == 7) {
                score += bar * (p.d[i][j] == q);
                score -= bar * (p.d[i][j] == r(q));
            } else if (i == 1 || i == 6 || j == 1 || j == 6) {
                score -= bar * (p.d[i][j] == q);
                score += bar * (p.d[i][j] == r(q));
            } else {
                score += cell * (p.d[i][j] == q);
                score -= cell * (p.d[i][j] == r(q));
            }
        }
    }
    return score;
}

int iswin(Part p) {
    int ans = 0;
    for (auto raw : p.d) {
        for (auto q : raw) {
            ans += (q == voit);
        }
    }
    return ans;
}

pair<int, pair<int, int>> alpbeto(Part p, int d, int alpha, int beta, bool f, cell q, int ans) {
    if (d == 0 || ans == 0) {
        return {heuristic(p, q), {0, 0}};
    }
    if (f) {
        int val = -1000000000;
        int i = 0;
        int j = 0;
        for (auto e : p.generate_particles(q)) {
            int w = alpbeto(e.first, d - 1, alpha, beta, false, q, ans - 1).first;
            if (w > val) {
                val = w;
                i = e.second.first;
                j = e.second.second;
            }
            alpha = max(alpha, val);
            if (alpha >= beta) {
                break;
            }
        }
        return {val, {i, j}};
    } else {
        int val = 1000000000;
        int i = 0;
        int j = 0;
        for (auto e : p.generate_particles(r(q))) {
            int w = alpbeto(e.first, d - 1, alpha, beta, true, q, ans - 1).first;
            if (w < val) {
                val = w;
                i = e.second.first;
                j = e.second.second;
            }
            beta = min(beta, val);
            if (alpha >= beta) {
                break;
            }
        }
        return {val, {i, j}};
    }
}

Part p;

int main() {
    vector<vector<char>> g(8, vector<char>(8, '.'));
    g[3][3] = 'W';
    g[4][4] = 'W';
    g[3][4] = 'B';
    g[4][3] = 'B';
    p = Part(g);
    int a;
    cin >> a;
    cell q;
    if (a == 1) {
        q = blak;
    } else {
        q = wite;
    }
    cell cur = blak;
    while (true) {
        if (cur != q) {
            char xc;
            int x, y;
            cin >> xc >> y;
            x = xc - 'a';
            y--;
            cerr << x << " " << y << "n" << endl;
            p.idti(x, y, cur);
        } else {
            auto res = alpbeto(p, 6, -1000000000, 1000000000, true, q, iswin(p));
            char d = res.second.first + 'a';
            cerr << d << res.second.second + 1 << endl;
            cout << d << res.second.second + 1 << endl;
            p.idti(res.second.first, res.second.second, cur);
        }
        cur = r(cur);
        p.print();
    }
}
