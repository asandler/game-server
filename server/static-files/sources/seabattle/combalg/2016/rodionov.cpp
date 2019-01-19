#include <bits/stdc++.h>

using namespace std;

ifstream in("seabatle.in");

#define mp make_pair

int n = 10;

struct deck
{
    char x;
    int y;
    int id;
    bool alive;
    deck()
    {
        x = 'A' - 1;
        y = -1;
        id = -1;
        alive = 0;
    }
    deck(char a, int b, int i)
    {
        x = a;
        y = b;
        id = i;
        alive = 1;
    }
};

int main()
{
    srand(time(NULL));
    char x1, x2, ans = 'M';
    int y1, y2, k = 0, decks = 20, turn;
    vector<vector<int>> a(10, vector<int>(10, -1));
    map<pair<int, char>, pair<int, bool>> b;
    vector <int> c = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
    for (int i = 0; i < n; ++i)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2 || y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        for (int j = x1; j <= x2; ++j){
            a[j - 'A'][y1] = i;
            b[mp(y1, j)] = mp(i, 1);
        }
        for (int j = y1; j <= y2; ++j){
            a[x1 - 'A'][j] = i;
            b[mp(j, x1)] = mp(i, 1);
        }
    }
//    cout << a['H' - 'A'][9] << endl;
//    for (int i = 0; i < 10; ++i)
//    {
//        for (int j = 0; j < 10; ++j)
//        {
//            if (a[i][j] == -1)
//                cout << '~' << ' ';
//            else
//                cout << a[i][j] << ' ';
//        }
//        cout << endl;
//    }
    cin >> turn;
    if (turn == 0)
        cout << (int)(rand() % 10) << 'X' << ' ' << x1 << y1 << (char)('A' + (rand() % 10)) << rand() % 10 << endl;
    string s;
    for (int I = 0; I < 100 && decks > 0; ++I)
    {
        cin >> s;
        cout << (int)(rand() % 10) << 'X' << ' ';
        if(s.size() > 1)
        {
            x1 = s[0];
            y1 = s[1] - '0';
            x2 = s[2];
            y2 = s[3] - '0';
            turn = 1;
            if (a[x2 - 'A'][y2] != -1 && b[mp(y2, x2)].second != 0)
            {
                decks --;
                c[a[x2 - 'A'][y2]]--;
                b[mp(y2, x2)].second = 0;
                if (c[a[x2 - 'A'][y2]] == 0)
                    cout << 'K' << endl;
                else
                    cout << 'H' << endl;
            }
            else
                turn = 0;
        }
        if (s.size() == 1 && (s[0] == 'K' || s[0] == 'H')){
            for (auto i:b)
            {
                if (i.second.second == 1){
                    cout << i.first.second << i.first.first << (char)('A' + (rand() % 10)) << rand() % 10 << endl;
                    break;
                }
            }
        }
        else if (turn == 0 && decks > 0)
        {
            for (auto i:b)
            {
                if (i.second.second == 1){
                    cout << i.first.second << i.first.first << s[0] << s[1] << endl;
                    break;
                }
            }
        }
    }
}
