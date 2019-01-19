#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <array>
#include <bitset>

using namespace std;

typedef array<string, 3> field_t;

bool cwins(const field_t& field, char c)
{
    for (int i = 0; i < 3; i++)
    {
        if (field[i][0] == c && field[i][1] == c && field[i][2] == c)
            return true;
        if (field[0][i] == c && field[1][i] == c && field[2][i] == c)
            return true;
    }
    if (field[0][0] == c && field[1][1] == c && field[2][2] == c)
        return true;
    if (field[0][2] == c && field[1][1] == c && field[2][0] == c)
        return true;
    return false;
}

char revert_turn(char c)
{
    return c == 'X' ? 'O' : 'X';
}

bool draw(field_t field)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (field[i][j] == '.')
                return false;
    return true;
}

int analyse_position(field_t& field, char turn)
{
    if (cwins(field, revert_turn(turn)))
        return 0;
    if (cwins(field, turn))
        return 1;
    if (draw(field))
        return 2;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (field[i][j] == '.')
            {
                field[i][j] = turn;
                if (!analyse_position(field, revert_turn(turn)))
                {
                    field[i][j] = '.';
                    return 1;
                }
                field[i][j] = '.';
            }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (field[i][j] == '.')
            {
                field[i][j] = turn;
                if (analyse_position(field, revert_turn(turn)) == 2)
                {
                    field[i][j] = '.';
                    return 2;
                }
                field[i][j] = '.';
            }
    return 0;
}

int make_turn(int a, int b)
{
    return a * 3 + b;
}

pair<int, int> get_turn(int a)
{
    return make_pair(a / 3, a % 3);
}

bool end(field_t field)
{
    return cwins(field, 'X') || cwins(field, 'O') || draw(field);
}

int get_any_turn(field_t &field, char c)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (field[i][j] == '.')
            {
                field[i][j] = c;
                return make_turn(i, j);
            }
    return -1;
}

int main()
{
    field_t field;
    for (int i = 0; i < 3; i++)
        field[i] = "...";
    int _tr;
    cin >> _tr;
    char me = 'X';
    if (_tr == 2)
    {
        me = 'O';
        cin >> _tr;
        pair<int, int> p = get_turn(_tr);
        field[p.first][p.second] = 'X';
    }
    while (!end(field))
    {
        //My turn
        bool f = false;
        for (int i = 0; i < 3 && !f; i++)
            for (int j = 0; j < 3 && !f; j++)
                if (field[i][j] == '.')
                {
                    field[i][j] = me;
                    if (!analyse_position(field, revert_turn(me)))
                    {
              //          cout << "WIN!!!" << endl;
                        cout << make_turn(i, j) << endl;
                        f = true;
                        break;
                    }
                    field[i][j] = '.';
                }
        for (int i = 0; i < 3 && !f; i++)
            for (int j = 0; j < 3 && !f; j++)
                if (field[i][j] == '.')
                {
                    field[i][j] = me;
                    if (analyse_position(field, revert_turn(me)) == 2)
                    {
                        //cout << "DRAW!!!" << endl;
                        cout << make_turn(i, j) << endl;
                        f = true;
                        break;
                    }
                    field[i][j] = '.';
                }
        if (!f)
            cout << get_any_turn(field, me) << endl;
        //for (int i = 0; i < 3; i++)
            //cout << field[i] << endl;
        if (end(field))
            break;
        //His turn
        cin >> _tr;
        pair<int, int> p = get_turn(_tr);
        field[p.first][p.second] = revert_turn(me);
    }
    return 0;
}
