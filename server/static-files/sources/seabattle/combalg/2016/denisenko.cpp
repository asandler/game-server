#include<iostream>
#include<iomanip>
#include<sstream>
#include<cstring>
#include<cmath>
#include<fstream>
#include<vector>
#include<climits>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<fstream>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<char, int> pci;
int leng = 10;
int mov[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
int filling[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
int num[4] = {4, 3, 2, 1};
vector <vector<int>> used(leng, vector<int>(leng, 0));
vector<vector<int>> our_world(leng, vector<int>(leng, 0));

mt19937 rnd(179);

struct ship
{
    pii start, fin;
    ship()
    {
        start.first = 0;
        start.second = 0;
        fin.first = 0;
        fin.second = 0;
    }
    ship(pii a, pii b)
    {
        start = a;
        fin = b;
    }
};

bool check(pii start, int siz, pii type)
{
    for (int i = 0; i <= siz; i++)
    {
        if (start.first >= 0 && start.first < leng && start.second >= 0 && start.second < leng && used[start.first][start.second] != 0)
        {
            return false;
        }
        if (start.first < 0 || start.first >= leng || start.second < 0 || start.second >= leng)
        {
            return false;
        }
        start.first += type.first;
        start.second += type.second;
    }
    return true;
}

bool is_good = false;

void fil(pii start, pii type, int siz)
{
    for (int i = 0; i <= siz; i++)
    {
        used[start.first][start.second] = 2;
        for (int j = 0; j < 8; j++)
        {
            int newx = start.first + filling[j][0];
            int newy = start.second + filling[j][1];
            if (newx >= 0 && newx < leng && newy >= 0 && newy < leng && used[newx][newy] == 0)
            {
                used[newx][newy] = 1;
            }
        }
        start.first += type.first;
        start.second += type.second;
    }
}

vector <ship> newn()
{
    vector <ship> pre;
    for (int i = 0; i < leng; i++)
    {
        for (int j = 0; j < leng; j++)
        {
            used[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < num[i]; j++)
        {
            pii start, type;
            start.first = rand() % 10;
            start.second = rand() % 10;
            int ptr = rand() % 4;
            type.first = mov[ptr][0];
            type.second = mov[ptr][1];
            if (check(start, i, type))
            {
                fil(start, type, i);
                ship neww;
                neww.start = start;
                start.first += type.first * (i);
                start.second += type.second * (i);
                neww.fin = start;
                pre.push_back(neww);
            }
        }
    }
    if (pre.size() == 10)
    {
        is_good = true;
    }
    return pre;
}

bool good(int a, int b)
{
    return (a >= 0 && a < leng && b >= 0 & b < leng);
}

void K_or_H(int x, int y)
{
    used[x][y] = 3;
    int newx = x - 1, newy = y, cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        newx = x;
        newy = y;
        while (good(newx, newy) && used[newx][newy] == 3)
        {
            newx += mov[i][0];
            newy += mov[i][1];
        }
        if (good(newx, newy) && used[newx][newy] == 2)
        {
            cout << 'H' << endl;
            return;
        }
    }
    cout << 'K' << endl;
}

pii next(int x, int y)
{
    int newx = x, newy = y, cnt = 0, maxi = 0;;
    pii ans;
    int type;
    for (int i = 0; i < 4; i++)
    {
        newx = x;
        newy = y;
        cnt = 0;
        while(good(newx, newy) && our_world[newx][newy] == 2)
        {
            cnt++;
            newx += mov[i][0];
            newy += mov[i][1];
        }
        if (cnt > maxi)
        {
            if (good(newx, newy) && our_world[newx][newy] == 0)
            {
                ans.first = newx;
                ans.second = newy;
            }
        }
    }
    return ans;
}

bool not_around(int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        int newx = filling[i][0] + x, newy = filling[i][1] + y;
        if (good(newx, newy) && our_world[newx][newy] != 0)
        {
            return false;
        }
    }
    return true;
}

int type(int x1, int y1, int x2, int y2)
{
    if (x1 == x2)
    {
        if (y1 > y2)
        {
            return 1;
        }
        return 0;
    }
    if (x1 > x2)
    {
        return 3;
    }
    return 2;
}

int sizing(int x1, int y1, int x2, int y2)
{
    if (x1 == x2)
    {
        return abs(y1 - y2) + 1;
    }
    return abs(x1 - x2) + 1;
}

pii rand_shoot()
{
    pii ans(0, 0);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (our_world[i][j] == 0)
            {
                ans.first = i;
                ans.second = j;
                return ans;
            }
        }
    }
    return ans;
}

void shooting_from()
{
    for (int i = 0; i < leng; i++)
    {
        for (int j = 0; j < leng; j++)
        {
            if (used[i][j] == 2)
            {
                cout << char(i + 'A') << j;
                return;
            }
        }
    }
}

int main()
{
    ofstream out;
    ifstream in;
    in.open("ansn.in");
    vector <ship> ans;
    int cnt = 0;
    char x1, x2;
    int y1, y2;
    for (int i = 0; i < 10; i++)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        int start_x = x1 - 'A', start_y = y1, fin_x = x2 - 'A', fin_y = y2;
        pii typer;
        typer.first = mov[type(start_x, start_y, fin_x, fin_y)][0];
        typer.second = mov[type(start_x, start_y, fin_x, fin_y)][1];
        pii start(start_x, start_y);
        fil(start, typer, sizing(start_x, start_y, fin_x, fin_y) - 1);
    }
    int is_first;
    cin >> is_first;
    bool now = true;
    if (is_first != 0)
    {
        now = false;
    }
    bool is_rand = true;
    int xx, yy;
    string ptr;
    while(true)
    {
        if(now)
        {
            cout << 0 << 'X' << ' ';
            shooting_from();
            bool get = false;
            pii newn = rand_shoot();
            if(!is_rand)
            {
                our_world[xx][yy] = 1;
                cout << char(xx + 'A') << yy << endl;
            }
            else
            {
                our_world[newn.first][newn.second] = 1;
                cout << char(newn.first + 'A') << newn.second << endl;
            }
            cin >> ptr;
            if (ptr.size() > 1)
            {
                now = false;
                is_rand = true;
            }
            else
            {
                if (is_rand)
                {
                    our_world[newn.first][newn.second] = 2;
                }
                else
                {
                    our_world[xx][yy] = 2;
                }
                if (ptr[0] == 'H')
                {
                    if (!is_rand)
                    {
                        newn.first = xx;
                        newn.second = yy;
                    }
                    pii ptr = next(newn.first, newn.second);
                    xx = ptr.first;
                    yy = ptr.second;
                    is_rand = false;
                }
                else
                {
                    is_rand = true;
                }

            }
        }
        else
        {
            char x;
            int y;
            if (ptr.empty())
            {
                cin >> ptr;
            }
            x = ptr[2];
            y = ptr[3] - '0';
            ptr.clear();
            ptr.clear();
            if (used[x - 'A'][y] == 2)
            {
                cout << 0 << 'X' << ' ';
                K_or_H(x - 'A', y);
                now = false;
            }
            else
            {
                now = true;
            }
        }
    }
    return 0;
}
