
// Battle of pythons
// Release: version 3.0 (experimental)
// Author: Евгений Щербаков

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

const long INF = 1000000000;
const long MAX_DEPTH = 3;

long step[2][4] = {{0, 1, 0, -1}, {-1, 0, 1, 0}};

inline long analys(long, long, long, long);
inline bool value(long, long);
long minimax(long, long, char&, long, long, long);
long find_food(long, long);

class field
{
public:
    bool mat[30][40];
    vector <long> food;
};

field f;
long q;

class snake
{
public:
    friend long analys();
    friend bool value();
    vector <long> v;
    long len;
    long direct;
    void snakec(long num)
    {
        v.assign(4, 0);
        len = 3;
        if(num == 1)
        {
            direct = 2;
            v[0] = 120;
            v[1] = 80;
            v[2] = 40;
            v[3] = 0;
        }
        else
        {
            direct = 0;
            v[0] = 1079;
            v[1] = 1119;
            v[2] = 1159;
            v[3] = 1199;
        }
    }
    void get_turn(char c)
    {
        if(c == 'R')
        {
            direct = ((direct + 1) % 4);
        }
        if(c == 'L')
        {
            bool in = false;
            if(direct == 0)
            {
                direct = 3;
                in = true;
            }
            else
            {
                if(!in)
                {
                    direct = ((direct - 1) % 4);
                }
            }
        }
        for(long i = len; i > 0; i--)
        {
            v[i] = v[i - 1];
        }
        v[0] += (step[1][direct] * 40 + step[0][direct]);
        for(long i = 0; i < food_pos.size(); i++)
        {
            ++food_pos[i];
        }
        if((food_pos.size() > 0) && (food_pos[0] == len))
        {
            long ind = false;
            food_pos.erase(food_pos.begin());
            if((v[len] % 40) == (v[len - 1] % 40))
            {
                if(v[len] > v[len - 1])
                {
                    v.push_back(v[len] + 40);
                    ind = true;
                }
                else
                {
                    if(!ind)
                    {
                        v.push_back(v[len] - 40);
                        ind = true;
                    }
                }
            }
            else
            {
                if(v[len] > v[len - 1])
                {
                    if(!ind)
                    {
                        v.push_back(v[len] + 1);
                        ind = true;
                    }
                }
                else
                {
                    if(!ind)
                    {
                        v.push_back(v[len] - 1);
                        ind = true;
                    }
                }
            }
            ++len;
        }
        if(f.mat[v[0] / 40][v[0] % 40])
        {
            f.mat[v[0] / 40][v[0] % 40] = false;
            food_pos.push_back(0);
        }
    }
private:
    vector <long> food_pos;
};

class unit: public snake
{
public:
    void do_turn(long c2)
    {
        char a = 'F';
        long ans;
        ans = minimax(MAX_DEPTH, 1, a, v[0], c2, direct);
        get_turn(a);
        printf("%c\n", a);
    }
};

snake enemy;
unit u;
char ct;

int main()
{

    long coun = 0;
    char turn, cur;

    scanf("%li\n", &q);

    for(long i = 0; i < 30; i++)
    {
        for(long k = 0; k < 40; k++)
        {
            scanf("%c", &cur);
            f.mat[i][k] = ((cur == '0') ? false : true);
            if(f.mat[i][k])
            {
                f.food.push_back(i * 40 + k);
            }
        }
        scanf("\n");
    }

    enemy.snakec(3 - q);
    u.snakec(q);

    while(true)
    {
        if((q == 2) && (coun == 0))
        {
            scanf("%c\n", &turn);
            enemy.get_turn(turn);
        }
        u.do_turn(enemy.v[0]);
        scanf("%c\n", &turn);
        enemy.get_turn(turn);
        ++coun;
    }

    return 0;
}

inline bool value(long x, long y)
{
    long ind = false;
    for(long i = 0; i < enemy.len; i++)
    {
        if((x == (enemy.v[i] % 40)) && (y == (enemy.v[i] / 40)))
        {
            ind = true;
            break;
        }
    }
    if(!ind)
    {
        for(long i = 1; i < u.len; i++)
        {
            if((x == (u.v[i] % 40)) && (y == (u.v[i] / 40)))
            {
                ind = true;
                break;
            }
        }
    }
    if((x < 0) || (y < 0) || (x > 39) || (y > 29) || (ind))
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline long analys(long first_l, long second_l, long food1, long food2)
{
    long res = 1000000 * first_l - 1000000 * second_l + (100 - food1) * 1000 - (100 - food2) * 1000;
    return res;
}

long minimax(long depth, long turn, char &res, long co1, long co2, long dir)
{
    long lenf_1 = find_food(co1 % 40, co1 / 40), lenf_2 = find_food(co2 % 40, co2 / 40), co;
    if(depth <= 0)
    {
        return analys(u.len, enemy.len, lenf_1, lenf_2);
    }
    long score = ((turn == 1) ? -INF : INF);
    for(long i = 0; i < 3; i++)
    {
        long mov = (dir + i - 1);
        if(mov < 0)
        {
            mov = 3;
        }
        mov %= 4;
        co = (turn == 1 ? co1 : co2);
        if(!value(co % 40 + step[0][mov], co / 40 + step[1][mov]))
        {
            long prev = score;
            score = ((turn == 1) ? max(score, minimax(depth - 1, 3 - turn, ct, (co1 + step[1][mov] * 40 + step[0][mov]), co2, mov)) : min(score, minimax(depth - 1, 3 - turn, ct, co1, (co2 + step[1][mov] * 40 + step[0][mov]), mov)));
            if((prev != score))
            {
                switch(i)
                {
                case 0:
                {
                    res = 'L';
                    break;
                }
                case 1:
                {
                    res = 'F';
                    break;
                }
                case 2:
                {
                    res = 'R';
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
    }
    return score;
}

long find_food(long x, long y)
{
    long len = INF;
    for(long i = 0; i < f.food.size(); i++)
    {
        long curx = (f.food[i] % 40), cury = (f.food[i] / 40);
        if((len > (labs(curx - x) + labs(cury - y))) && (f.mat[cury][curx]))
        {
            len = (labs(curx - x) + labs(cury - y)) ;
        }
    }
    return len;
}
