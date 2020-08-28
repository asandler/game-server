#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int g[10][10];
vector <pair <int, int> > dir(9);

int is_good(int i, int j, int col)
{
    if (g[i][j] != -1)
    {
        return 0;
    }
    int ans = 0;
    for (int d = 1; d <= 8; ++d)
    {
        int i0 = i + dir[d].first;
        int j0 = j + dir[d].second;
        int L = 0;
        while (g[i0][j0] == (1 - col))
        {
            i0 += dir[d].first;
            j0 += dir[d].second;
            L++;
        }
        if ((L > 0) && (g[i0][j0] == col))
        {
            ans += L;
        }
    }
    return ans;
}

void change_color(int i, int j, int col)
{
    for (int d = 1; d <= 8; ++d)
    {
        int i0 = i + dir[d].first;
        int j0 = j + dir[d].second;
        int L = 0;
        while (g[i0][j0] == (1 - col))
        {
            i0 += dir[d].first;
            j0 += dir[d].second;
            L++;
        }
        if ((L > 0) && (g[i0][j0] == col))
        {
            int i0 = i + dir[d].first;
            int j0 = j + dir[d].second;
            while (g[i0][j0] == (1 - col))
            {
                g[i0][j0] = col;
                i0 += dir[d].first;
                j0 += dir[d].second;
            }
        }
    }
    g[i][j] = col;
}

int main()
{
    srand(179);
    dir[1] = make_pair(-1, 0);
    dir[2] = make_pair(-1, 1);
    dir[3] = make_pair(0, 1);
    dir[4] = make_pair(1, 1);
    dir[5] = make_pair(1, 0);
    dir[6] = make_pair(1, -1);
    dir[7] = make_pair(0, -1);
    dir[8] = make_pair(-1, -1);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            g[i][j] = -2;
        }
    }
    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
        {
            g[i][j] = -1;
        }
    }
    //0 - black; 1 - white; -1 - empty; -2 - bound
    g[4][5] = 0;
    g[5][4] = 0;
    g[4][4] = 1;
    g[5][5] = 1;
    int flag;
    cin >> flag;
    if (flag == 2)
    {
        char c;
        int a;
        cin >> c >> a;
        change_color((int)(c - 'a') + 1, a, 0);
    }
    int my_col = 1 - (flag % 2);
    while (true)
    {
        vector <pair<int, int> > can;
        vector <int> P;
        int summ = 0;
        for (int i = 1; i <= 8; ++i)
        {
            for (int j = 1; j <= 8; ++j)
            {
                int newv = is_good(i, j, my_col);
                if (newv > 0)
                {
                    can.push_back(make_pair(i, j));
                    P.push_back(newv);
                    summ += newv;
                }
            }
        }
        int cou = (int)can.size();
        if (cou == 0)
        {
            cout << "Skip" << endl;
        }
        else
        {
            int k = (rand() % summ) + 1;
            int summnow = 0;
            int it = 0;
            while (summnow < k)
            {
                summnow += P[it];
                it++;
            }
            it--;
            change_color(can[it].first, can[it].second, my_col);
            cout << (char)(can[it].first + 'a' - 1) << can[it].second << endl;
        }
        char c;
        int a;
        cin >> c;
        if (c != 's')
        {
            cin >> a;
            change_color((int)(c - 'a') + 1, a, 1 - my_col);
        }
        else
        {
            cin >> c;
            cin >> c;
            cin >> c;
        }
    }
    return 0;
}
