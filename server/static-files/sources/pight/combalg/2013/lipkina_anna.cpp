#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <deque>
#include <utility>
#include <string>
#include <cstdlib>

using namespace std;

const int high = 30; // height of the field
const int width = 40; // width if the field
const int MAX = 1543; // magic const, my school number:) use for invisible tail

char field[high][width]; //field of a battle
int dist[high][width];
vector<char> greed;

vector<char> enemy_movement; // enemy's turns, from cin
vector<char> my_movement; // my turns, to cout
int enemy_leng; // length of enemy's python
int my_leng; // length if my python
int my_invisible, enemy_invisible; // first index of my/enemy's invisible tail
int eat; // count eat from the field
char convert[256][256]; // from last - to we must go - to program language

int abs(int a)
{
    if (a < 0) return -a;
    return a;
}

struct point
{
    int x, y, time_shine;
    point(){}
    point(int a, int b, int g)
    {
        x = a;
        y = b;
        time_shine = g;
    }
    bool operator == (point p)
    {
        return x == p.x && y == p.y;
    }
};

vector<point> enemy_python; // coordinates of enemy's python
vector<point> my_python; //coordinates of my python
char type_enemy, type_enemy_head; // type of char of enemy's body/head
char type_my, type_my_head; //type of char of my body/head
char my_last; //last direction of my python
char enemy_last; //last direction of enemy's python
point parent[high][width];

void move_enemy(char c)
{
    char temp = enemy_movement.back();
    if (temp == 'D' && c == 'F') enemy_movement.push_back('D');
    else if (temp == 'D' && c == 'R') enemy_movement.push_back('L');
    else if (temp == 'D' && c == 'L') enemy_movement.push_back('R');
    else if (temp == 'H' && c == 'F') enemy_movement.push_back('H');
    else if (temp == 'H' && c == 'R') enemy_movement.push_back('R');
    else if (temp == 'H' && c == 'L') enemy_movement.push_back('L');
    else if (temp == 'R' && c == 'F') enemy_movement.push_back('R');
    else if (temp == 'R' && c == 'L') enemy_movement.push_back('H');
    else if (temp == 'R' && c == 'R') enemy_movement.push_back('D');
    else if (temp == 'L' && c == 'F') enemy_movement.push_back('L');
    else if (temp == 'L' && c == 'R') enemy_movement.push_back('H');
    else if (temp == 'L' && c == 'L') enemy_movement.push_back('D');
    enemy_invisible = MAX;
    point enemy_tail;
    int idx = 0;
    for (int i = enemy_python.size() - 1; i > 0; --i)
    {
        if (enemy_python[i].time_shine > 0)
        {
            --enemy_python[i].time_shine;
            if (enemy_python[i].time_shine == 0)
            {
                if (i > idx)
                {
                    enemy_tail = enemy_python[i];
                    idx = i;
                }
                enemy_python[i] = enemy_python[i - 1];
            }
            enemy_invisible = min(enemy_invisible, i);
            continue;
        }
        if (i > idx)
        {
            enemy_tail = enemy_python[i];
            idx = i;
        }
        enemy_python[i] = enemy_python[i - 1];
    }
    if ((enemy_last == 'H' && c == 'L') || (enemy_last == 'D' && c == 'R') || (enemy_last == 'L' && c == 'F'))
        --enemy_python[0].y;
    else if ((enemy_last == 'D' && c == 'L') || (enemy_last == 'H' && c == 'R') || (enemy_last == 'R' && c == 'F'))
        ++enemy_python[0].y;
    else if ((enemy_last == 'R' && c == 'L') || (enemy_last == 'L' && c == 'R') || (enemy_last == 'H' && c == 'F'))
        --enemy_python[0].x;
    else if ((enemy_last == 'L' && c == 'L') || (enemy_last == 'R' && c == 'R') || (enemy_last == 'D' && c == 'F'))
        ++enemy_python[0].x;
    enemy_last = enemy_movement.back();
    field[enemy_tail.x][enemy_tail.y] = '0';
    if (enemy_invisible < MAX && enemy_python[enemy_invisible].time_shine == 1)
    {
        field[enemy_python[enemy_invisible].x][enemy_python[enemy_invisible].y] = type_enemy;
        ++enemy_leng;
    }
    if (field[enemy_python[0].x][enemy_python[0].y] == '1')
    {
        enemy_python.push_back(point(enemy_python[0].x, enemy_python[0].y, enemy_python.size() + 1));
        --eat;
    }
    field[enemy_python[0].x][enemy_python[0].y] = type_enemy_head;
    field[enemy_python[1].x][enemy_python[1].y] = type_enemy;
}

void move_my(char c)
{
    char temp = my_movement.back();
    if (temp == 'D' && c == 'F') my_movement.push_back('D');
    else if (temp == 'D' && c == 'R') my_movement.push_back('L');
    else if (temp == 'D' && c == 'L') my_movement.push_back('R');
    else if (temp == 'H' && c == 'F') my_movement.push_back('H');
    else if (temp == 'H' && c == 'R') my_movement.push_back('R');
    else if (temp == 'H' && c == 'L') my_movement.push_back('L');
    else if (temp == 'R' && c == 'F') my_movement.push_back('R');
    else if (temp == 'R' && c == 'L') my_movement.push_back('H');
    else if (temp == 'R' && c == 'R') my_movement.push_back('D');
    else if (temp == 'L' && c == 'F') my_movement.push_back('L');
    else if (temp == 'L' && c == 'R') my_movement.push_back('H');
    else if (temp == 'L' && c == 'L') my_movement.push_back('D');
    my_invisible = MAX;
    point my_tail;
    int idx = 0;
    for (int i = my_python.size() - 1; i > 0; --i)
    {
        if (my_python[i].time_shine > 0)
        {
            --my_python[i].time_shine;
            if (my_python[i].time_shine == 0)
            {
                if (i > idx)
                {
                    my_tail = my_python[i];
                    idx = i;
                }
                my_python[i] = my_python[i - 1];
            }
            my_invisible = min(my_invisible, i);
            continue;
        }
        if (i > idx)
        {
            my_tail = my_python[i];
            idx = i;
        }
        my_python[i] = my_python[i - 1];
    }
    if ((my_last == 'H' && c == 'L') || (my_last == 'D' && c == 'R') || (my_last == 'L' && c == 'F'))
        --my_python[0].y;
    else if ((my_last == 'D' && c == 'L') || (my_last == 'H' && c == 'R') || (my_last == 'R' && c == 'F'))
        ++my_python[0].y;
    else if ((my_last == 'R' && c == 'L') || (my_last == 'L' && c == 'R') || (my_last == 'H' && c == 'F'))
        --my_python[0].x;
    else if ((my_last == 'L' && c == 'L') || (my_last == 'R' && c == 'R') || (my_last == 'D' && c == 'F'))
        ++my_python[0].x;
    my_last = my_movement.back();
    field[my_tail.x][my_tail.y] = '0';
    if (my_invisible < MAX && my_python[my_invisible].time_shine == 1)
    {
        field[my_python[my_invisible].x][my_python[my_invisible].y] = type_my;
        ++my_leng;
    }
    if (field[my_python[0].x][my_python[0].y] == '1')
    {
        my_python.push_back(point(my_python[0].x, my_python[0].y, my_python.size() + 1));
        --eat;
    }
    field[my_python[0].x][my_python[0].y] = type_my_head;
    field[my_python[1].x][my_python[1].y] = type_my;
    cout << c << endl;
}

struct direction
{
    int L, F, R;
    direction(){}
    direction(int a, int b, int c)
    {
        L = a;
        F = b;
        R = c;
    }
};

int count_free_subj(point v)
{
    deque<point> q;
    q.push_back(v);
    point d;
    if (v.x < 0 || v.x > high - 1 || v.y < 0 || v.y > width - 1) return 0;
    if (!(field[v.x][v.y] == '0' || field[v.x][v.y] == '1')) return 0;
    for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            dist[i][j] = MAX;
            if (!(field[i][j] == '0' || field[i][j] == '1'))
            {
                dist[i][j] = 0;
            }
        }
    }
    /*for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (dist[i][j]) cout << 1;
            else cout << 0;
        }
        cout << endl;
    }*/
    dist[v.x][v.y] = 0;
    int ans = 0;
    while (!q.empty())
    {
        d = q.front();
        q.pop_front();
        ++ans;
        if (d.x > 0 && dist[d.x - 1][d.y] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x - 1, d.y, 0));
            dist[d.x - 1][d.y] = dist[d.x][d.y] + 1;
        }
        if (d.x < high - 1 && dist[d.x + 1][d.y] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x + 1, d.y, 0));
            dist[d.x + 1][d.y] = dist[d.x][d.y] + 1;
        }
        if (d.y > 0 && dist[d.x][d.y - 1] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x, d.y - 1, 0));
            dist[d.x][d.y - 1] = dist[d.x][d.y] + 1;
        }
        if (d.y < width - 1 && dist[d.x][d.y + 1] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x, d.y + 1, 0));
            dist[d.x][d.y + 1] = dist[d.x][d.y] + 1;
        }
    }
    int temp = 0;
    for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (field[i][j] == '0' || field[i][j] == '1') ++temp;
        }
    }
    //cout << temp << ' ' << ans << endl;
    if (ans >= temp / 3) return 1;
    return 0;
}

direction check_my_movement()
{
    direction ans(0, 0, 0);
    char to = my_last;
    point p = my_python[0];
    int ansH = count_free_subj(point(p.x - 1, p.y, 0));
    int ansD = count_free_subj(point(p.x + 1, p.y , 0));
    int ansR = count_free_subj(point(p.x, p.y + 1, 0));
    int ansL = count_free_subj(point(p.x, p.y - 1, 0));
    //cout << to << ' ' << ansH << ' ' << ansR << ' ' << ansD << ' ' << ansL << endl;
    int i = 0;
    while (i < my_python.size() && my_python[i].time_shine <= 1) ++i;
    point tail = my_python[i - 1];
    if (to == 'D')
    {
        if (ansD && p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_my && (point(p.x + 1, p.y, 0) == tail || point(p.x + 1, p.y, 1) == tail))))
        {
            ans.F = 1;
        }
        if (ansR && p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_my && (point(p.x, p.y + 1, 0) == tail || point(p.x, p.y + 1, 1) == tail))))
        {
            ans.L = 1;
        }
        if (ansL && p.y > 0 && (field[p.x][p.y - 1] == '0' || field[p.x][p.y - 1] == '1'
            || (field[p.x][p.y - 1] == type_my && (point(p.x, p.y - 1, 0) == tail || point(p.x, p.y - 1, 1) == tail))))
        {
            ans.R = 1;
        }
        return ans;
    }
    if (to == 'H')
    {
        if (ansH && p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_my && (point(p.x - 1, p.y, 0) == tail || point(p.x - 1, p.y, 1) == tail))))
        {
            ans.F = 1;
        }
        if (ansL && p.y > 0 && (field[p.x][p.y - 1] == '0' || field[p.x][p.y - 1] == '1'
            || (field[p.x][p.y - 1] == type_my && (point(p.x, p.y - 1, 0) == tail || point(p.x, p.y - 1, 1) == tail))))
        {
            ans.L = 1;
        }
        if (ansR && p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_my && (point(p.x, p.y + 1, 0) == tail || point(p.x, p.y + 1, 1) == tail))))
        {
            ans.R = 1;
        }
        return ans;
    }
    if (to == 'L')
    {
        if (ansL && p.y < width - 1 && (field[p.x][p.y - 1] == '0' || field[p.x][p.y - 1] == '1'
            || (field[p.x][p.y - 1] == type_my && (point(p.x, p.y - 1, 0) == tail || point(p.x, p.y - 1, 1) == tail))))
        {
            ans.F = 1;
        }
        if (ansH && p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_my && (point(p.x - 1, p.y, 0) == tail || point(p.x - 1, p.y, 1) == tail))))
        {
            ans.R = 1;
        }
        if (ansD && p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_my && (point(p.x + 1, p.y, 0) == tail || point(p.x + 1, p.y, 1) == tail))))
        {
            ans.L = 1;
        }
        return ans;
    }
    if (to == 'R')
    {
        if (ansR && p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_my && (point(p.x, p.y + 1, 0) == tail || point(p.x, p.y + 1, 1) == tail))))
        {
            ans.F = 1;
        }
        if (ansH && p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_my && (point(p.x - 1, p.y, 0) == tail || point(p.x - 1, p.y, 1) == tail))))
        {
            ans.L = 1;
        }
        if (ansD && p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_my && (point(p.x + 1, p.y, 0) == tail || point(p.x + 1, p.y, 1) == tail))))
        {
            ans.R = 1;
        }
        return ans;
    }
}

direction posibility_enemy()
{
    direction ans(0, 0, 0);
    char to = enemy_last;
    point p = enemy_python[0];
    if (to == 'D')
    {
        if (p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_enemy && point(p.x + 1, p.y, 0) == enemy_python.back())))
        {
            ans.F = 1;
        }
        if (p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_enemy && point(p.x, p.y + 1, 0) == enemy_python.back())))
        {
            ans.L = 1;
        }
        if (p.y > 0 && (field[p.x][p.y - 1] == '0' || field[p.x][p.y - 1] == '1'
            || (field[p.x][p.y - 1] == type_enemy && point(p.x, p.y - 1, 0) == enemy_python.back())))
        {
            ans.R = 1;
        }
        return ans;
    }
    if (to == 'H')
    {
        if (p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_enemy && point(p.x - 1, p.y, 0) == enemy_python.back())))
        {
            ans.F = 1;
        }
        if (p.y > 0 && (field[p.x][p.y - 1] == '0' || field[p.x][p.y - 1] == '1'
            || (field[p.x][p.y - 1] == type_enemy && point(p.x, p.y - 1,0) == enemy_python.back())))
        {
            ans.L = 1;
        }
        if (p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_enemy && point(p.x, p.y + 1, 0) == enemy_python.back())))
        {
            ans.R = 1;
        }
        return ans;
    }
    if (to == 'L')
    {
        if (p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_enemy && point(p.x, p.y + 1, 0) == enemy_python.back())))
        {
            ans.F = 1;
        }
        if (p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_enemy && point(p.x - 1, p.y, 0) == enemy_python.back())))
        {
            ans.R = 1;
        }
        if (p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_enemy && point(p.x + 1, p.y, 0) == enemy_python.back())))
        {
            ans.L = 1;
        }
        return ans;
    }
    if (to == 'R')
    {
        if (p.y < width - 1 && (field[p.x][p.y + 1] == '0' || field[p.x][p.y + 1] == '1'
            || (field[p.x][p.y + 1] == type_enemy && point(p.x, p.y + 1, 0) == enemy_python.back())))
        {
            ans.F = 1;
        }
        if (p.x > 0 && (field[p.x - 1][p.y] == '0' || field[p.x - 1][p.y] == '1'
            || (field[p.x - 1][p.y] == type_enemy && point(p.x - 1, p.y, 0) == enemy_python.back())))
        {
            ans.L = 1;
        }
        if (p.x < high - 1 && (field[p.x + 1][p.y] == '0' || field[p.x + 1][p.y] == '1'
            || (field[p.x + 1][p.y] == type_enemy && point(p.x + 1, p.y, 0) == enemy_python.back())))
        {
            ans.R = 1;
        }
        return ans;
    }
}

void print_field()
{
    for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cout << field[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void build_convert()
{
    convert['H' - 0]['H' - 0] = 'F';
    convert['H' - 0]['R' - 0] = 'R';
    convert['H' - 0]['L' - 0] = 'L';
    convert['D' - 0]['R' - 0] = 'L';
    convert['D' - 0]['L' - 0] = 'R';
    convert['D' - 0]['D' - 0] = 'F';
    convert['L' - 0]['L' - 0] = 'F';
    convert['L' - 0]['H' - 0] = 'R';
    convert['L' - 0]['D' - 0] = 'L';
    convert['R' - 0]['R' - 0] = 'F';
    convert['R' - 0]['H' - 0] = 'L';
    convert['R' - 0]['D' - 0] = 'R';
}

point min_dist_to_eat(point v)
{
    deque<point> q;
    q.push_back(v);
    point d;
    for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            dist[i][j] = MAX;
            if (!(field[i][j] == '0' || field[i][j] == '1'))
            {
                dist[i][j] = 0;
            }
        }
    }
    dist[v.x][v.y] = 0;
    parent[v.x][v.y] = v;
    while (!q.empty())
    {
        d = q.front();
        q.pop_front();
        if (field[d.x][d.y] == '1')
        {
            return d;
        }
        if (d.x > 0 && dist[d.x - 1][d.y] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x - 1, d.y, 0));
            dist[d.x - 1][d.y] = dist[d.x][d.y] + 1;
            parent[d.x - 1][d.y] = d;
        }
        if (d.x < high - 1 && dist[d.x + 1][d.y] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x + 1, d.y, 0));
            dist[d.x + 1][d.y] = dist[d.x][d.y] + 1;
            parent[d.x + 1][d.y] = d;
        }
        if (d.y > 0 && dist[d.x][d.y - 1] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x, d.y - 1, 0));
            dist[d.x][d.y - 1] = dist[d.x][d.y] + 1;
            parent[d.x][d.y - 1] = d;
        }
        if (d.y < width - 1 && dist[d.x][d.y + 1] > dist[d.x][d.y] + 1)
        {
            q.push_back(point(d.x, d.y + 1, 0));
            dist[d.x][d.y + 1] = dist[d.x][d.y] + 1;
            parent[d.x][d.y + 1] = d;
        }
    }
    return point(-1, -1, 0);
}

void strategy_greedy()
{
    point to = min_dist_to_eat(my_python[0]);
    point temp;
    greed.clear();
    while (!(parent[to.x][to.y] == to))
    {
        temp = parent[to.x][to.y];
        if (temp.x > to.x) greed.push_back('D');
        else if (temp.x < to.x) greed.push_back('H');
        else if (temp.y < to.y) greed.push_back('L');
        else if (temp.y > to.y) greed.push_back('R');
        to = parent[to.x][to.y];
    }
    if (greed.back() == 'H') greed.back() = 'D';
    else if (greed.back() == 'L') greed.back() = 'R';
    else if (greed.back() == 'R') greed.back() = 'L';
    else if (greed.back() == 'D') greed.back() = 'H';
    char cur = convert[my_last][greed.back()];
    direction d = check_my_movement();
    if (cur == 'F' && d.F) move_my(cur);
    else if (cur == 'L' && d.L) move_my(cur);
    else if (cur == 'R' && d.R) move_my(cur);
    else if (d.F) move_my('F');
    else if (d.L) move_my('L');
    else move_my('R');
    //print_field();
}

void strategy_random()
{
    direction d = check_my_movement();
    if (d.F) move_my('F');
    else if (d.L) move_my('L');
    else if (d.R) move_my('R');
    //print_field();
}

bool strategy_block_catch()
{
    point cur_my = my_python[0];
    point cur_en = enemy_python[0];
    int leng = my_movement.size();
    if (leng <= 2) return 0;
    direction d = check_my_movement();
    if (my_movement[leng - 3] == 'D' && my_movement[leng - 2] == 'R' && my_movement[leng - 1] == 'R' && d.L && field[cur_my.x - 1][cur_my.y - 1] == type_enemy_head)
    {
        move_my('L');
        return 1;
    }
    else if (my_movement[leng - 3] == 'D' && my_movement[leng - 2] == 'L' && my_movement[leng - 1] == 'L' && d.R && field[cur_my.x - 1][cur_my.y + 1] == type_enemy_head)
    {
        move_my('R');
        return 1;
    }
    else if (my_movement[leng - 3] == 'H' && my_movement[leng - 2] == 'R' && my_movement[leng - 1] == 'R' && d.R && field[cur_my.x + 1][cur_my.y - 1] == type_enemy_head)
    {
        move_my('R');
        return 1;
    }
    else if (my_movement[leng - 3] == 'H' && my_movement[leng - 2] == 'L' && my_movement[leng - 1] == 'L' && d.L && field[cur_my.x + 1][cur_my.y + 1] == type_enemy_head)
    {
        move_my('L');
        return 1;
    }
    else if (my_movement[leng - 3] == 'R' && my_movement[leng - 2] == 'H' && my_movement[leng - 1] == 'H' && d.L && field[cur_my.x + 1][cur_my.y - 1] == type_enemy_head)
    {
        move_my('L');
        return 1;
    }
    else if (my_movement[leng - 3] == 'R' && my_movement[leng - 2] == 'D' && my_movement[leng - 1] == 'D' && d.R && field[cur_my.x - 1][cur_my.y - 1] == type_enemy_head)
    {
        move_my('R');
        return 1;
    }
    else if (my_movement[leng - 3] == 'L' && my_movement[leng - 2] == 'H' && my_movement[leng - 1] == 'H' && d.R && field[cur_my.x + 1][cur_my.y + 1] == type_enemy_head)
    {
        move_my('R');
        return 1;
    }
    else if (my_movement[leng - 3] == 'L' && my_movement[leng - 2] == 'D' && my_movement[leng - 1] == 'D' && d.L && field[cur_my.x - 1][cur_my.y + 1] == type_enemy_head)
    {
        move_my('L');
        return 1;
    }
    return 0;
}

bool strategy_block_in_angle()
{
    point cur_en = enemy_python[0], cur_my = my_python[0];
    direction d = check_my_movement();
    if (cur_en == point(0, 0, 0) && (cur_my == point(2, 0, 0) || cur_my == point(1, 1, 0) || cur_my == point(0, 2, 0)))
    {
        if (cur_my == point(2, 0, 0))
        {

            if (my_last == 'L' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'H' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
        else if (cur_my == point(0, 2, 0))
        {
            if (my_last == 'L' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'H' && d.L)
            {
                move_my('L');
                return 1;
            }
        }
        else
        {
            if (my_last == 'L' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'L' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'H' && d.L)
            {
                move_my('L');
                return 1;
            }
            else if (my_last == 'H' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
    }
    else if (cur_en == point(0, width - 1, 0) && (cur_my == point(0, width - 3, 0) || cur_my == point(1, width - 2, 0) || cur_my == point(2, width - 1, 0)))
    {
        if (cur_my == point(0, width - 3, 0))
        {

            if (my_last == 'R' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'H' && d.R)
            {
                move_my('R');
                return 1;
            }
        }
        else if (cur_my == point(2, width - 1, 0))
        {
            if (my_last == 'R' && d.L)
            {
                move_my('L');
                return 1;
            }
            else if (my_last == 'H' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
        else
        {
            if (my_last == 'R' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'R' && d.L)
            {
                move_my('L');
                return 1;
            }
            else if (my_last == 'H' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'H' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
    }
    else if (cur_en == point(high - 1, 0, 0) && (cur_my == point(high - 3, 0, 0) || cur_my == point(high - 2, 1, 0) || cur_my == point(high - 1, 2, 0)))
    {
        if (cur_my == point(high - 3, 0, 0))
        {

            if (my_last == 'D' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'L' && d.L)
            {
                move_my('L');
                return 1;
            }
        }
        else if (cur_my == point(high - 1, 2, 0))
        {
            if (my_last == 'D' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'L' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
        else
        {
            if (my_last == 'D' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'D' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'L' && d.L)
            {
                move_my('L');
                return 1;
            }
            else if (my_last == 'L' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
    }
    else if (cur_en == point(high - 1, width - 1, 0) && (cur_my == point(high - 1, width - 3, 0) || cur_my == point(high - 2, width - 2, 0) || cur_my == point(high - 3, width - 1, 0)))
    {
        if (cur_my == point(high - 1, width - 3, 0))
        {

            if (my_last == 'R' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'D' && d.L)
            {
                move_my('L');
                return 1;
            }
        }
        else if (cur_my == point(high - 3, width - 1, 0))
        {
            if (my_last == 'R' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'D' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
        else
        {
            if (my_last == 'R' && d.F)
            {
                move_my('F');
                return 1;
            }
            else if (my_last == 'R' && d.R)
            {
                move_my('R');
                return 1;
            }
            else if (my_last == 'D' && d.L)
            {
                move_my('L');
                return 1;
            }
            else if (my_last == 'D' && d.F)
            {
                move_my('F');
                return 1;
            }
        }
    }
    return 0;
}

void turn_enemy()
{
    char c;
    //string s;
    //if (!(cin >> c)) exit(0);
    cin >> c;
    //cin >> s >> s;
    //cin >> s >> s >> s;
    //cin >> s >> s >> s;
    //cin >> s >> s >> s;
    move_enemy(c);
    //cout << s[s.length() - 1] << endl;
    //print_field();
}

int main()
{
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    build_convert();
    int action;
    cin >> action;
    eat = 0;
    for (int i = 0; i < high; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cin >> field[i][j];
            if (field[i][j] == '1') ++eat;
        }
    }
    char c;
    string s;
    if (action == 2)
    {
        for (int i = 3; i >=0; --i)
        {
            field[i][0] = '*';
            enemy_python.push_back(point(i, 0, 0));
        }
        field[3][0] = '%';
        type_enemy = '*';
        type_enemy_head = '%';
        enemy_last = 'D';
        enemy_movement.push_back('D');
        for (int i = high - 4; i < high ; ++i)
        {
            field[i][width - 1] = '#';
            my_python.push_back(point(i, width - 1, 0));
        }
        field[high - 4][width - 1] = '&';
        type_my = '#';
        type_my_head = '&';
        my_last = 'H';
        my_movement.push_back('H');
        cin >> c;
        move_enemy(c);
    }
    else
    {
        for (int i = 3; i >= 0; --i)
        {
            field[i][0] = '*';
            my_python.push_back(point(i, 0, 0));
        }
        field[3][0] = '%';
        type_my = '*';
        type_my_head = '%';
        my_last = 'D';
        my_movement.push_back('D');
        for (int i = high - 4; i < high ; ++i)
        {
            field[i][width - 1] = '#';
            enemy_python.push_back(point(i, width - 1, 0));
        }
        field[high - 4][width - 1] = '&';
        type_enemy = '#';
        type_enemy_head = '&';
        enemy_last = 'H';
        enemy_movement.push_back('H');
    }
    enemy_leng = 4;
    my_leng = 4;
    //print_field();
    //cin >> s >> s >> s;
    while (eat > 0)
    {
        if (strategy_block_catch())
        {
            turn_enemy();
            continue;
        }
        if (strategy_block_in_angle())
        {
            turn_enemy();
            continue;
        }
        strategy_greedy();
        turn_enemy();
    }
    int i = 0;
    while (i < 500)
    {
        if (strategy_block_catch())
        {
            turn_enemy();
            ++i;
            continue;
        }
        if (strategy_block_in_angle())
        {
            turn_enemy();
            ++i;
            continue;
        }
        strategy_random();
        turn_enemy();
        ++i;
        //print_field();
    }
    return 0;
}

