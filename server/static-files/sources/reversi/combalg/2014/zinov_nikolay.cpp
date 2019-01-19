#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>

using namespace std;

const int ITER = 30;
const int DEPTH = 2;

typedef pair<short, short> cord;
typedef pair<long long, long long> f;

bool black;
f field;
cord moves[64];
int mc = 0;

inline short atm(f field, long long mask)
{
    return (field.first & mask ? (field.second & mask ? 1 : 0) : 2);
}
inline short atb(f field, int bias)
{
    return atm(field, 1LL << bias);
}
inline short at(f field, int x, int y)
{
    return atb(field, 8*x + y);
}
inline void setm(f& field, long long mask, bool black)
{
    field.first |= mask;
    if (black)
        field.second |= mask;
    else
        field.second &= ~mask;
}
inline void setb(f& field, int bias, bool black)
{
    setm(field, 1LL << bias, black);
}
inline void set(f& field, int x, int y, bool black)
{
    setb(field, 8*x + y, black);
}
void make_move(f& field, bool black, short move_x, short move_y)
{
    for (short dx = -1; dx <= 1; dx++)
        for (short dy = -1; dy <= 1; dy++)
        {
            if (!dx && !dy)
                continue;
            short x = move_x, y = move_y;
            for (x+=dx,y+=dy;x >= 0 && y >= 0 && x < 8 && y < 8; x+=dx,y+=dy)
                if (at(field, x, y) != !black)
                    break;
            if (x >= 0 && y >= 0 && x < 8 && y < 8 && at(field, x, y) == black)
                for (x-=dx,y-=dy; x != move_x || y != move_y; x-=dx,y-=dy)
                    set(field, x, y, black);
        }
    set(field, move_x, move_y, black);
}
bool can_move(f field, bool black, short move_x, short move_y)
{
    for (short dx = -1; dx <= 1; dx++)
        for (short dy = -1; dy <= 1; dy++)
        {
            short x = move_x, y = move_y;
            if (!dx && !dy)
                continue;
            bool reverse = false;
            for (x+=dx,y+=dy;x >= 0 && y >= 0 && x < 8 && y < 8; x+=dx,y+=dy)
            {
                if (at(field, x, y) != !black)
                    break;
                reverse = true;
            }
            if (x >= 0 && y >= 0 && x < 8 && y < 8 && at(field, x, y) == black && reverse)
                return true;
        }
    return false;
}
inline void do_move(cord a)
{
    make_move(field, black, a.first, a.second);
    cout << (char)('a'+a.first) << a.second+1 << endl;
}
inline void skip()
{
    cout << "Skip" << endl;
}
inline void read_move()
{
    char x;
    short y;
    cin >> x;
    if (x == 'S')
    {
        cin.ignore(3);
    }
    else
    {
        cin >> y;
        make_move(field, !black, (short)(x-'a'), y-1);
    }
}
int winner(f field)
{
    int ans[2] = {0, 0};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            short cur = at(field, i, j);
            if (cur < 2)
                ans[cur]++;
        }
    }
    return ans[1] - ans[0];
}
short simulate(f field, bool black)
{
    bool passed = false;
    while (true)
    {
        mc = 0;
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                if (at(field, x, y) == 2 && can_move(field, black, x, y))
                {
                    moves[mc] = cord(x, y);
                    mc++;
                }
            }
        }
        if (mc > 0)
        {
            cord choice = moves[rand() % mc];
            make_move(field, black, choice.first, choice.second);
            passed = false;
        }
        else
        {
            if (passed)
            {
                return winner(field);
            }
            passed = true;
        }
        black = !black;
    }
}
int cost(f field, bool black)
{
    int count = 0;
    for (int i = 0; i < ITER; i++)
    {
        count += simulate(field, black);
    }
    return count;
}
int move_white(f& field, int a = -1000000, int b = 1000000, int depth = 0, bool passed=false);
int move_black(f& field, int a = -1000000, int b = 1000000, int depth = 0, bool passed=false)
{
    if (depth == DEPTH)
        return cost(field, true);
    int max_cost = -1000000;
    cord best_move(-1, -1);
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            if (at(field, x, y) == 2 && can_move(field, true, x, y))
            {
                f temp = field;
                make_move(temp, true, x, y);
                int cur = move_white(temp, a, b, depth+1);
                if (cur > max_cost)
                {
                    if (cur > b)
                        return 1000000;
                    max_cost = cur;
                    best_move = cord(x, y);
                    a = max(max_cost, a);
                }
            }
        }
    if (best_move.first == -1)
    {
        if (passed)
            max_cost = winner(field) * ITER;
        else
        {
            f temp = field;
            max_cost = move_white(temp, a, b, depth+1, true);
        }
    }
    if (depth == 0)
    {
        if (best_move.first == -1)
            skip();
        else
            do_move(best_move);
    }
    return max_cost;
}
int move_white(f& field, int a, int b, int depth, bool passed)
{
    if (depth == DEPTH)
        return cost(field, false);
    int max_cost = 1000000;
    cord best_move(-1, -1);
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            if (at(field, x, y) == 2 && can_move(field, false, x, y))
            {
                f temp = field;
                make_move(temp, false, x, y);
                int cur = move_black(temp, a, b, depth+1);
                if (cur < max_cost)
                {
                    if (cur < a)
                        return -1000000;
                    max_cost = cur;
                    best_move = cord(x, y);
                    b = min(max_cost, b);
                }
            }
        }
    if (best_move.first == -1)
    {
        if (passed)
            max_cost = winner(field) * ITER;
        else
        {
            f temp = field;
            max_cost = move_black(temp, a, b, depth+1, true);
        }
    }
    if (depth == 0)
    {
        if (best_move.first == -1)
            skip();
        else
            do_move(best_move);
    }
    return max_cost;
}
inline void init()
{
    srand(time(NULL));
    set(field, 3, 3, false);
    set(field, 4, 4, false);
    set(field, 4, 3, true);
    set(field, 3, 4, true);
}
int main()
{
    init();
    short first;
    cin >> first;
    black = first == 1;
    if (!black)
        read_move();
    while (true)
    {
        if (black)
            move_black(field);
        else
            move_white(field);
        read_move();
    }
    return 0;
}
