#include <iostream>
#include <vector>
#define f first
#define s second

using namespace std;

int field[64];

void print_field()
{
    cout << "  ";
    for (int j = 0; j < 8; ++j)
    {
        cout << j + 1;
    }
    cout << endl;
    for (int i = 0; i < 8; ++i)
    {
        cout << (char)(i + 97) << ' ';
        for (int j = 0; j < 8; ++j)
        {
            cout << field[i * 8 + j];
        }
        cout << endl;
    }
}

int a1to0(char a, int t)
{
    return (a - 97) * 8 + t - 1;
}

pair<char, int> _0toa1(int i)
{
    return make_pair((char)(i / 8 + 97), i % 8 + 1);
}

vector< pair<int, int> > poss_(int col)
{
    vector< pair<int, int> > ans;
    bool poss = false;
    int encou = 0;
    int j;
    for (int i = 0; i < 64; ++i)
    {
        if (field[i] == 0)
        {
            poss = false;

            encou = 0;
            j = i + 1;
            while (j % 8 != 0 && field[j] == 3 - col) //Пока клетка не ушла за поле вправо и цвет в ней противоположный
            {
                j++;
                encou++;
            }
            if (j % 8 != 0 && field[j] == col && encou > 0)
            {
                ans.push_back(make_pair(i, encou));
                poss = true;
            }

            encou = 0;
            j = i - 1;
            while (j % 8 != 7 && field[j] == 3 - col)
            {
                j--;
                encou++;
            }
            if (j % 8 != 7 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i - 8;
            while (j >= 0 && field[j] == 3 - col)
            {
                j -= 8;
                encou++;
            }
            if (j >= 0 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i + 8;
            while (j < 64 && field[j] == 3 - col)
            {
                j += 8;
                encou++;
            }
            if (j < 64 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i - 9;
            while (j >= 0 && j % 8 != 7 && field[j] == 3 - col)
            {
                j -= 9;
                encou++;
            }
            if (j >= 0 && j % 8 != 7 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i - 7;
            while (j >= 0 && j % 8 != 0 && field[j] == 3 - col)
            {
                j -= 7;
                encou++;
            }
            if (j >= 0 && j % 8 != 0 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i + 7;
            while (j < 64 && j % 8 != 7 && field[j] == 3 - col)
            {
                j += 7;
                encou++;
            }
            if (j < 64 && j % 8 != 7 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }

            encou = 0;
            j = i + 9;
            while (j < 64 && j % 8 != 0 && field[j] == 3 - col)
            {
                j += 9;
                encou++;
            }
            if (j < 64 && j % 8 != 0 && field[j] == col && encou > 0)
            {
                if (poss)
                {
                    ans[ans.size() - 1].s += encou;
                }
                else
                {
                    ans.push_back(make_pair(i, encou));
                    poss = true;
                }
            }
        }
    }
    return ans;
}

void f5field(int i, int col)
{
    field[i] = col;
    int j;

    int encou = 0;
    j = i + 1;
    while (j % 8 != 0 && field[j] == 3 - col) //Пока клетка не ушла за поле вправо и цвет в ней противоположный
    {
        encou++;
        j++;
    }
    if (j % 8 != 0 && field[j] == col && encou > 0)
    {
        while (j != i + 1)
        {
            j--;
            field[j] = col;
        }
    }

    encou = 0;
    j = i - 1;
    while (j % 8 != 7 && field[j] == 3 - col)
    {
        encou++;
        j--;
    }
    if (j % 8 != 7 && field[j] == col && encou > 0)
    {
        while (j != i - 1)
        {
            j++;
            field[j] = col;
        }
    }

    encou = 0;
    j = i - 8;
    while (j >= 0 && field[j] == 3 - col)
    {
        encou++;
        j -= 8;
    }
    if (j >= 0 && field[j] == col && encou > 0)
    {
        while (j != i - 8)
        {
            j += 8;
            field[j] = col;
        }
    }

    encou = 0;
    j = i + 8;
    while (j < 64 && field[j] == 3 - col)
    {
        encou++;
        j += 8;
    }
    if (j < 64 && field[j] == col && encou > 0)
    {
        while (j != i + 8)
        {
            j -= 8;
            field[j] = col;
        }
    }

    encou = 0;
    j = i - 9;
    while (j >= 0 && j % 8 != 7 && field[j] == 3 - col)
    {
        encou++;
        j -= 9;
    }
    if (j >= 0 && j % 8 != 7 && field[j] == col && encou > 0)
    {
        while (j != i - 9)
        {
            j += 9;
            field[j] = col;
        }
    }

    encou = 0;
    j = i - 7;
    while (j >= 0 && j % 8 != 0 && field[j] == 3 - col)
    {
        encou++;
        j -= 7;
    }
    if (j >= 0 && j % 8 != 0 && field[j] == col && encou > 0)
    {
        while (j != i - 7)
        {
            j += 7;
            field[j] = col;
        }
    }

    encou = 0;
    j = i + 7;
    while (j < 64 && j % 8 != 7 && field[j] == 3 - col)
    {
        encou++;
        j += 7;
    }
    if (j < 64 && j % 8 != 7 && field[j] == col && encou > 0)
    {
        while (j != i + 7)
        {
            j -= 7;
            field[j] = col;
        }
    }

    encou = 0;
    j = i + 9;
    while (j < 64 && j % 8 != 0 && field[j] == 3 - col)
    {
        encou++;
        j += 9;
    }
    if (j < 64 && j % 8 != 0 && field[j] == col && encou > 0)
    {
        while (j != i + 9)
        {
            j -= 9;
            field[j] = col;
        }
    }
}

int main()
{
    int col;
    field[27] = 2;
    field[28] = 1;
    field[35] = 1;
    field[36] = 2;
    char a;
    int t;
    vector< pair<int, int> > posss;
    int maxx = 0;
    int maxi = 0;
    pair<char, int> tempa1;


    cin >> col;
    if (col == 1)
    {
        tempa1 = _0toa1(26);
        cout << tempa1.f << tempa1.s << endl;
        f5field(26, 1);
        //print_field();
    }
    while (true)
    {
        cin >> a;
        if (a != 'S')
        {
            cin >> t;
            f5field(a1to0(a, t), 3 - col);
        }
        else
        {
            cin >> a >> a >> a;
        }
        posss = poss_(col);
        /*
        for (int z = 0; z < posss.size(); ++z)
        {
            tempa1 = _0toa1(posss[z].f);
            cout << tempa1.f << tempa1.s << " ";
        }
        cout << endl;
        */
        if (posss.size() == 0)
        {
            cout << "Skip" << endl;
            //print_field();
        }
        else
        {
            maxx = 0;
            for (int i = 0; i < posss.size(); ++i)
            {
                if (maxx < posss[i].s)
                {
                    maxx = posss[i].s;
                    maxi = posss[i].f;
                }
            }
            f5field(maxi, col);
            tempa1 = _0toa1(maxi);
            cout << tempa1.f << tempa1.s << endl;
            //print_field();
        }
    }
    return 0;
}

