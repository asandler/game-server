#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <climits>

#define DETH 2
#define OUT 0
#define WHO gam

#define COEFF 2.75

using namespace std;

struct win
{
    int st, sh;
};

struct par
{
    int x, y;
    float w;
};

void sto(vector<vector<int> > pole, int gam)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(pole[0][0] != 0)
            {

            }
        }
    }
}

void out(vector<vector<int> > &pole, win &count, int gam);
void step(vector<vector<int> > &pole, int x, int y, int gam, win &count);
void find_can(vector<vector<int> > &pole, vector<par> &can, int gam);
void start(vector<vector<int> > &pole, int &gam, win &count);
void in(vector<vector<int> > &pole, int &gam, win &count);
int is_in(vector<par> &can, par c);

int nyan(vector<vector<int> > &pole, vector<par> &can, int gam)
{
    float b = -1;
    for(int k = 0; k < can.size(); k++)
    {
        //int a = sto(pole, can[k], gam);
        if(b < can[k].w && b < 4)
        {
            b = can[k].w;
        }
    }
    b += COEFF * can.size();
    return b;
}

int eng2(vector<vector<int> > &pole, int gam, int deth, win count)
{
    vector<par> can;
    vector<vector<int> > fake;
    find_can(pole, can, gam);
    if(deth == 0)
    {
        return nyan(pole, can, gam);
    }
    float ini = INT_MIN;
    for(int i = 0; i < can.size(); i++)
    {
        fake = pole;
        step(fake, can[i].x, can[i].y, gam, count);
        float prom = - eng2(fake, -gam, deth - 1, count);
        ini = max(ini, prom);
    }
    if(ini == INT_MIN)
    {
        ini = - eng2(pole, -gam, deth - 1, count);
    }
    return ini;
}

void eng1(vector<vector<int> > &pole, int gam, win &count)
{
    vector<par> can;
    vector<vector<int> > fake;
    find_can(pole, can, gam);
    int a = -1;
    float ini = INT_MIN;
    for(int i = 0; i < can.size(); i++)
    {
        win countf = count;
        fake = pole;
        step(fake, can[i].x, can[i].y, gam, countf);
        float prom = - eng2(fake, -gam, DETH, countf);
        if(ini <= prom)
        {
            a = i;
            ini = prom;
        }
    }
    if(a == -1)
    {
        cout << "Skip" << endl;
    }
    else
    {
        step(pole, can[a].x, can[a].y, gam, count);
        cout << (char)('a' + can[a].y) << 1 + can[a].x << endl;
    }
}

int main()
{
    int gam;
    win count;
    vector<vector<int> > pole;
    start(pole, gam, count);
    while(true)
    {
        if(OUT)
        {
            out(pole, count, gam);
        }
        if(WHO == 1)
        {
            eng1(pole, gam, count);
        }
        else
        {
            in(pole, gam, count);
        }
        gam = -gam;
        if(OUT && WHO == 1)
        {
            system("cls");
        }
    }
}

void start(vector<vector<int> > &pole, int &gam, win &count)
{
    string s;
    count.sh = 2;
    count.st = 2;
    pole.resize(8);
    for(int i = 0; i < 8; i++)
    {
        pole[i].resize(8, 0);
    }
    pole[3][3] = -1;
    pole[3][4] = 1;
    pole[4][3] = 1;
    pole[4][4] = -1;
    gam = 1;
    cin >> s;
    if(s == "2")
    {
        if(OUT)
        {
            out(pole, count, gam);
        }
        cin >> s;
        pole[s[0] - 'a'][s[1] - '1'] = -1;
        pole[3][3] = 1;
        pole[3][4] = -1;
        pole[4][3] = -1;
        pole[4][4] = 1;
        step(pole, s[1] - '1', s[0] - 'a', -1, count);
        if(OUT)
        {
            system("cls");
        }
    }
}

void in(vector<vector<int> > &pole, int &gam, win &count)
{
    string s;
    cin >> s;
    if(s != "Skip")
    {
        pole[s[0] - 'a'][s[1] - '1'] = gam;
        step(pole, s[1] - '1', s[0] - 'a', gam, count);
    }
    if(OUT && WHO == gam)
    {
        system("cls");
    }
}

void out(vector<vector<int> > &pole, win &count, int gam)
{
        cout << "# = " << count.sh << '\n';
        cout << "* = " << count.st << '\n';
        if(gam == -1)
        {
            cout << "You are gamer #" << '\n' << '\n';
        }
        else
        {
            cout << "You are gamer *" << '\n' << '\n';
        }
        cout << " 12345678" << '\n';
        for(int i = 0; i < 8; i++)
        {
            cout << (char)('a' + i);
            for(int j = 0; j < 8; j++)
            {
                if(pole[i][j] == -1)
                {
                    cout << '#';
                }
                else if(pole[i][j] == 1)
                {
                    cout << '*';
                }
                else
                {
                    cout << '.';
                }
            }
            cout << (char)('a' + i) << '\n';
        }
        cout << " 12345678" << '\n';
}

void step(vector<vector<int> > &pole, int x, int y, int gam, win &count)
{
    int sum = 0;
    pole[y][x] = gam;
    if(x > 1 && pole[y][x - 1] == -gam)//íŕëĺâî
    {
        int i;
        for(i = x - 1; i > 0 && pole[y][i] == -gam; i--);
        if(pole[y][i] == gam)
        {
            for(int i = x - 1; i > 0 && pole[y][i] == -gam; pole[y][i] = gam, i--, sum++);
        }
    }
    if(x > 1 && y > 1 && pole[y - 1][x - 1] == -gam)//ââĺđő íŕëĺâî
    {
        int i, j;
        for(i = y - 1, j = x - 1; i > 0 && j > 0 && pole[i][j] == -gam; i--, j--);
        if(pole[i][j] == gam)
        {
            for(i = y - 1, j = x - 1; i > 0 && j > 0 && pole[i][j] == -gam; pole[i][j] = gam, i--, j--, sum++);
        }
    }
    if(y > 1 && pole[y - 1][x] == -gam)//ââĺđő
    {
        int i;
        for(i = y - 1; i > 0 && pole[i][x] == -gam; i--);
        if(pole[i][x] == gam)
        {
            for(int i = y - 1; i > 0 && pole[i][x] == -gam; pole[i][x] = gam, i--, sum++);
        }
    }
    if(x < 7 && y > 1 && pole[y - 1][x + 1] == -gam)//ââĺđő íŕďđŕâî
    {
        int i, j;
        for(i = y - 1, j = x + 1; i > 0 && j < 7 && pole[i][j] == -gam; i--, j++);
        if(pole[i][j] == gam)
        {
            for(i = y - 1, j = x + 1; i > 0 && j < 7 && pole[i][j] == -gam; pole[i][j] = gam, i--, j++, sum++);
        }
    }
    if(x < 7 && pole[y][x + 1] == -gam)//íŕďđŕâî
    {
        int i;
        for(i = x + 1; i < 7 && pole[y][i] == -gam; i++);
        if(pole[y][i] == gam)
        {
            for(int i = x + 1; i < 7 && pole[y][i] == -gam; pole[y][i] = gam, i++, sum++);
        }
    }
    if(x < 7 && y < 7 && pole[y + 1][x + 1] == -gam)//âíčç íŕďđŕâî
    {
        int i, j;
        for(i = y + 1, j = x + 1; i < 7 && j < 7 && pole[i][j] == -gam; i++, j++);
        if(pole[i][j] == gam)
        {
            for(i = y + 1, j = x + 1; i < 7 && j < 7 && pole[i][j] == -gam; pole[i][j] = gam, i++, j++, sum++);
        }
    }
    if(y < 7 && pole[y + 1][x] == -gam)//âíčç
    {
        int i;
        for(i = y + 1; i < 7 && pole[i][x] == -gam; i++);
        if(pole[i][x] == gam)
        {
            for(int i = y + 1; i < 7 && pole[i][x] == -gam; pole[i][x] = gam, i++, sum++);
        }
    }
    if(x > 1 && y < 7 && pole[y + 1][x - 1] == -gam)//âíčç íŕëĺâî
    {
        int i, j;
        for(i = y + 1, j = x - 1; i < 7 && j > 0 && pole[i][j] == -gam; i++, j--);
        if(pole[i][j] == gam)
        {
            for(i = y + 1, j = x - 1; i < 7 && j > 0 && pole[i][j] == -gam; pole[i][j] = gam, i++, j--, sum++);
        }
    }
    if(gam == -1)
    {
        count.sh += sum + 1;
        count.st -= sum;
    }
    else
    {
        count.st += sum + 1;
        count.sh -= sum;
    }
}

int is_in(vector<par> &can, par c)
{
    int place = -1;
    for(int i = 0; i < can.size(); i++)
    {
        if(can[i].x == c.x && can[i].y == c.y)
        {
            place = i;
            break;
        }
    }
    return place;
}

void find_can(vector<vector<int> > &pole, vector<par> &can, int gam)
{
    for(int k = 0; k < 8; k++)
    {
        for(int g = 0; g < 8; g++)
        {
            if(pole[k][g] == gam)
            {
                if(g > 1 && pole[k][g - 1] == -gam)            //íŕëĺâî
                {
                    int i = g - 1;
                    for(; i > 0 && pole[k][i] == -gam; i--);
                    if(pole[k][i] == 0)
                    {
                        par c;
                        c.w = 2 * (g - 1 - i);
                        c.x = i;
                        c.y = k;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(g > 1 && k > 1 && pole[k - 1][g - 1] == -gam)//ââĺđő íŕëĺâî
                {
                    int i = k - 1;
                    int j = g - 1;
                    for(; i > 0 && j > 0 && pole[i][j] == -gam; i--, j--);
                    if(pole[i][j] == 0)
                    {
                        par c;
                        c.w = 2 * (k - 1 - i);
                        c.x = j;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(k > 1 && pole[k - 1][g] == -gam)          //ââĺđő
                {
                    int i = k - 1;
                    for(; i > 0 && pole[i][g] == -gam; i--);
                    if(pole[i][g] == 0)
                    {
                        par c;
                        c.w = 2 * (k - 1 - i);
                        c.x = g;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(k > 1 && g < 7 && pole[k - 1][g + 1] == -gam)//ââĺđő íŕďđŕâî
                {
                    int i = k - 1;
                    int j = g + 1;
                    for(; i > 0 && j < 7 && pole[i][j] == -gam; i--, j++);
                    if(pole[i][j] == 0)
                    {
                        par c;
                        c.w = 2 * (k - 1 - i);
                        c.x = j;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(g < 7 && pole[k][g + 1] == -gam)          //íŕďđâî
                {
                    int i = g + 1;
                    for(; i < 7 && pole[k][i] == -gam; i++);
                    if(pole[k][i] == 0)
                    {
                        par c;
                        c.w = 2 * (i - g - 1);
                        c.x = i;
                        c.y = k;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(k < 7 && g < 7 && pole[k + 1][g + 1] == -gam)//âíčç íŕďđŕâî
                {
                    int i = k + 1;
                    int j = g + 1;
                    for(; i < 7 && j < 7 && pole[i][j] == -gam; i++, j++);
                    if(pole[i][j] == 0)
                    {
                        par c;
                        c.w = 2 * (i - k - 1);
                        c.x = j;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(k < 7 && pole[k + 1][g] == -gam)             //âíčç
                {
                    int i = k + 1;
                    for(; i < 7 && pole[i][g] == -gam; i++);
                    if(pole[i][g] == 0)
                    {
                        par c;
                        c.w = 2 * (i - k - 1);
                        c.x = g;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
                if(k < 7 && g > 1 && pole[k + 1][g - 1] == -gam)//âíčç íŕëĺâî
                {
                    int i = k + 1;
                    int j = g - 1;
                    for(; i < 7 && j > 0 && pole[i][j] == -gam; i++, j--);
                    if(pole[i][j] == 0)
                    {
                        par c;
                        c.w = 2 * (i - k - 1);
                        c.x = j;
                        c.y = i;
                        int place = is_in(can, c);
                        if(place == -1)
                        {
                            can.push_back(c);
                        }
                        else
                        {
                            can[place].w += c.w;
                        }
                    }
                }
            }
        }
    }
}
