#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <queue>

#define long long long
#define sz(A) (int(A.size()))

using namespace std;

struct pt
{
    int x, y;

    pt(): x(0), y(0) {}

    pt(int x, int y): x(x), y(y) {}

    void rotate(char c)
    {
        if (c == 'L')
        {
            int cx = x;
            x = -y;
            y = cx;
        }
        else if (c == 'R')
        {
            int cx = x;
            x = y;
            y = -cx;
        }
        else
            assert(c == 'F');
    }

    friend bool operator ==(pt A, pt B)
    {
        return A.x == B.x && A.y == B.y;
    }

    friend bool operator !=(pt A, pt B)
    {
        return !(A == B);
    }
};

vector <vector <int> > snake_table;
vector <vector <int> > table;
const char moves[3] = {'L', 'R', 'F'};
const int INF = 100000;
int random_const;

struct snake
{
    vector <pt> pos;
    vector <int> protocol;
    pt now;
    int n, m;
    int ptr;
    int side;

    snake() {}

    snake(int n, int m, int side)
    {
        this->n = n;
        this->m = m;
        this->ptr = 0;
        this->side = side;

        if (side == 1)
        {
            for (int i = 0; i < 4; i++)
                this->pos.push_back(pt(i, 0)), snake_table[i][0] = side;
            this->now = pt(1, 0);
        }
        else
        {
            for (int i = 0; i < 4; i++)
                this->pos.push_back(pt(n - 1 - i, m - 1)), snake_table[n - 1 - i][m - 1] = side;
            this->now = pt(-1, 0);
        }
    }

    bool move_forward(char c)
    {
        pt now_copy = now;
        now.rotate(c);
        int newx = pos.back().x + now.x;
        int newy = pos.back().y + now.y;

        if (newx >= 0 && newx < n && newy >= 0 && newy < m && snake_table[newx][newy] == 0)
        {
            pos.push_back(pt(newx, newy));
            snake_table[newx][newy] = side;

            if (table[pos[ptr].x][pos[ptr].y] == 0)
                protocol.push_back(0), snake_table[pos[ptr].x][pos[ptr].y] = 0, ptr++;
            else
                protocol.push_back(1), table[pos[ptr].x][pos[ptr].y] = 0;
            return 1;
        }
        else
        {
            now = now_copy;
            return 0;
        }
    }

    void move_back()
    {
        snake_table[pos.back().x][pos.back().y] = 0;
        pos.pop_back();

        if (protocol.back() == 0)
            ptr--, snake_table[pos[ptr].x][pos[ptr].y] = side;
        else
            table[pos[ptr].x][pos[ptr].y] = 1;
        protocol.pop_back();
        pt A = pos.back();
        pt B = pos[sz(pos) - 2];
        now = pt(A.x - B.x, A.y - B.y);
    }
};

void print(vector <vector <int> > & A)
{
    for (int i = 0; i < sz(A); i++)
    {
        for (int j = 0; j < sz(A[i]); j++)
            cerr << A[i][j] << ' ';
        cerr << '\n';
    }
}

char random_move(snake & A)
{
    for (int i = 0; i < 3; i++)
    {
        char c = moves[i];

        if (A.move_forward(c))
        {
            if (A.move_forward('L') || A.move_forward('R') || A.move_forward('F'))
            {
                A.move_back();
                return c;
            }
            A.move_back();
        }
    }
    return ':' + '-' + '(';
}

int disc_zone(int i0, int j0)
{
    int n = sz(table);
    int m = sz(table[0]);
    vector <vector <int> > mark(n, vector <int>(m));
    queue <pt> Q;
    Q.push(pt(i0, j0));
    int ans = 0;
    mark[i0][j0] = 1;

    while (!Q.empty())
    {
        int x = Q.front().x;
        int y = Q.front().y;
        Q.pop();
        pt coor(1, 0);

        for (int i = 0; i < 4; i++)
        {
            int newx = x + coor.x;
            int newy = y + coor.y;

            if (newx >= 0 && newx < n && newy >= 0 && newy < m && !mark[newx][newy] && snake_table[newx][newy] == 0)
            {
                mark[newx][newy] = 1;
                Q.push(pt(newx, newy));
                ans += 1;
                
                if (ans > 50)
                    return ans;
            }
            coor.rotate('L');
        }
    }
    return ans;
}

int func(snake & A)
{
    int ans = INF;

    if (disc_zone(A.pos.back().x, A.pos.back().y) < 50)
        return INF * 2;

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (table[i][j] == 1)
                ans = min(ans, abs(i - A.pos.back().x) + abs(j - A.pos.back().y));

            if (abs(i - A.pos.back().x) + abs(j - A.pos.back().y) < 6 && snake_table[i][j] == 3 - A.side)
                return INF;
        }
    }
    return ans;
}

int on_food(snake & A, vector <vector <int> > & dist, vector <vector <double> > & alt_dist)
{
    return -func(A) * random_const + int(-dist[A.pos.back().x][A.pos.back().y] * alt_dist[A.pos.back().x][A.pos.back().y]);
}

pair <int, char> dfs(snake & A, snake & B, int player, int d, vector <vector <int> > & dist, vector <vector <double> > & alt_dist)
{
    if (d == 0)
        return make_pair(on_food(A, dist, alt_dist) - on_food(B, dist, alt_dist), '-');
    else if (player == 1)
    {
        int res = -INF;
        char resc = '-';

        for (int i = 0; i < 3; i++)
        {
            char c = moves[i];

            if (!A.move_forward(c))
                continue;
            int now = dfs(A, B, 2, d - 1, dist, alt_dist).first;

            if (res < now)
                res = now, resc = moves[i];
            A.move_back();
        }

        if (res == -INF)
            return make_pair(-INF, '-');
        else
            return make_pair(max(res, on_food(A, dist, alt_dist) - on_food(B, dist, alt_dist)), resc);
    }
    else
    {
        int res = INF;
        char resc = '-';

        for (int i = 0; i < 3; i++)
        {
            char c = moves[i];

            if (!B.move_forward(c))
                continue;
            int now = dfs(A, B, 1, d - 1, dist, alt_dist).first;

            if (res > now)
                res = now, resc = moves[i];
            B.move_back();
        }

        if (res == INF)
            return make_pair(INF, '-');
        else
            return make_pair(min(res, on_food(A, dist, alt_dist) - on_food(B, dist, alt_dist)), resc);
    }
}

char move(snake & A, snake & B, int player)
{
    /*int res = INF;
    char resc = '-';

    if (player == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (!A.move_forward(moves[i]))
                continue;
            
            if (res > func(A))
                res = func(A), resc = moves[i];
            A.move_back();
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            if (!B.move_forward(moves[i]))
                continue;

            if (res > func(B))
                res = func(B), resc = moves[i];
            B.move_back();
        }
    }

    if (resc == '-')
        return random_move(player == 1 ? A : B);
    else
        return resc;*/
    int n = sz(table);
    int m = sz(table[0]);
    vector <vector <int> > dist(n, vector <int>(m, INF));
    vector <vector <double> > alt_dist(n, vector <double>(m, INF));
    int counter = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int k = 0; k < n; k++)
                for (int l = 0; l < m; l++)
                    if (table[k][l] == 1)
                        dist[i][j] = min(dist[i][j], abs(i - k) + abs(j - l)), alt_dist[i][j] += abs(i - k) + abs(j - l), counter++;
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            alt_dist[i][j] /= counter;
    char xx = dfs(A, B, player, 5, dist, alt_dist).second;

    if (xx == '-')
        return random_move(player == 1 ? A : B);
    else
        return xx;
}

int main()
{
    ios::sync_with_stdio(0);
    
    random_const = rand() % 100;
    const int n = 30, m = 40;
    int num;
    cin >> num;

    table.resize(n, vector <int>(m));
    snake_table.resize(n, vector <int>(m));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            char c;
            cin >> c;
            table[i][j] = int(c - '0');
        }
    }
    snake A(n, m, 1);
    snake B(n, m, 2);

    if (num == 1)
    {
        char xx = move(A, B, 1);
        A.move_forward(xx);
        cout << xx << '\n';

        while (true)
        {
            char c;
            cin >> c;
            B.move_forward(c);
            xx = move(A, B, 1);
            cout << xx << '\n';
            A.move_forward(xx);
//            print(snake_table);
//            cerr << clock() * 1000 / CLOCKS_PER_SEC << '\n';
        }
    }
    else
    {
        while (true)
        {
            char c;
            cin >> c;
            A.move_forward(c);
            char xx = move(A, B, 2);
            cout << xx << '\n';
            B.move_forward(xx);
//            print(snake_table);
//            cerr << clock() * 1000 / CLOCKS_PER_SEC << '\n';
        }
    }
    return 0;
}
