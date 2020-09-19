#include<bits/stdc++.h>

using namespace std;

const int n = 30;
const int m = 40;
const int maxDepth = 14;
const int maxCycle = 90;
const int maxTimer = 1e5;
const int bigConst = 1e9;
const long long inf = 2e18;

vector<char> way = {'F', 'L', 'R'};
vector<deque<pair<int, int>>> snakes = {
{
    {3, 0},
    {2, 0},
    {1, 0},
    {0, 0}
},
{
    {26, 39},
    {27, 39},
    {28, 39},
    {29, 39}
}};
vector<vector<bool>> used(n, vector<bool> (m, false));
vector<vector<int>> pole(n, vector<int> (m));
vector<pair<int, int>> doFalse;
map<char, int> valueFirstTurn;
vector<deque<int>> grow(2);
vector<int> timer = {0, 0};
vector<int> foodW(2, 0);
vector<int> food(2, 0);
bool onlyGrow = false;
int foodLater;
int player;
bool flag;
int cnt;

vector<int> doAction(int x, int y, int turn)
{
    if (snakes[turn].front().first + x < 0 ||
        snakes[turn].front().first + x > 29 ||
        snakes[turn].front().second + y < 0 ||
        snakes[turn].front().second + y > 39)
        return {-1, -1, -1};

    int pos = pole[snakes[turn].front().first + x][snakes[turn].front().second + y];

    if (onlyGrow)
    {
        if (pos != -1)
        {
            snakes[turn].push_front({snakes[turn].front().first + x,
                                     snakes[turn].front().second + y});
            pole[snakes[turn].front().first][snakes[turn].front().second] = -1;
        }
        return {pos, -1, -1};
    }

    if (snakes[turn].front().first + x == snakes[turn].back().first &&
        snakes[turn].front().second + y == snakes[turn].back().second &&
        (!grow[turn].size() || grow[turn].front() != timer[turn]))
    {
        snakes[turn].push_front({snakes[turn].front().first + x,
                                 snakes[turn].front().second + y});
        int taleX = snakes[turn].back().first;
        int taleY = snakes[turn].back().second;
        snakes[turn].pop_back();
        return {0, taleX, taleY};
    }

    if (pos == -1)
        return {-1, -1, -1};

    snakes[turn].push_front({snakes[turn].front().first + x,
                             snakes[turn].front().second + y});

    pole[snakes[turn].front().first][snakes[turn].front().second] = -1;

    int taleX = -1;
    int taleY = -1;
    int growF = -1;
    if (!grow[turn].size() || grow[turn].front() != timer[turn])
    {
        pole[snakes[turn].back().first][snakes[turn].back().second] = 0;
        taleX = snakes[turn].back().first;
        taleY = snakes[turn].back().second;
        snakes[turn].pop_back();
    }
    else
    {
        growF = timer[turn];
        grow[turn].pop_front();
    }

    if (pos == 1)
    {
        grow[turn].push_back(timer[turn] + snakes[turn].size());
        food[turn]++;
        foodW[turn] += maxTimer - timer[turn];
    }

    if (taleX != -1)
        return {pos, taleX, taleY};
    else
        return {pos, -1, growF};
}

void undoAction(int turn, int val,
                 pair<int, int> taleOrGrow)
{
    pole[snakes[turn].front().first][snakes[turn].front().second] = val;
    snakes[turn].pop_front();

    if (onlyGrow)
        return;

    if (val == 1)
    {
        grow[turn].pop_back();
        food[turn]--;
        foodW[turn] -= (maxTimer - timer[turn]);
    }

    if (taleOrGrow.first == -1)
    {
        grow[turn].push_front(taleOrGrow.second);
    }
    else
    {
        snakes[turn].push_back(taleOrGrow);
        pole[taleOrGrow.first][taleOrGrow.second] = -1;
    }
}

void inCycle()
{
    if (cnt == maxCycle)
        return;

    int dirX = (snakes[player].front().first -
                 snakes[player][1].first);
    int dirY = (snakes[player].front().second -
                 snakes[player][1].second);

    for (auto elem : way)
    {
        int x, y;
        if (elem == 'F')
        {
            x = dirX;
            y = dirY;
        }
        else if (elem == 'L')
        {
            x = -dirY;
            y = dirX;
        }
        else
        {
            x = dirY;
            y = -dirX;
        }
        auto ans = doAction(x, y, player);
        if (ans[0] == -1)
            continue;

        if (!used[snakes[player].front().first][snakes[player].front().second])
        {
            used[snakes[player].front().first][snakes[player].front().second] = true;
            cnt++;
            inCycle();
            doFalse.push_back({snakes[player].front().first, snakes[player].front().second});
        }
        undoAction(player, ans[0], {ans[1], ans[2]});
        if (cnt == maxCycle)
            break;
    }
}

pair<long long, char> alphaBeta(int depth, int turn, long long a,
                                long long b, char c)
{
    int currPlayer = (player + 1 - (turn + 1) / 2) % 2;
    if (!depth)
    {
        cnt = 0;
        onlyGrow = true;
        inCycle();
        onlyGrow = false;
        for (auto elem : doFalse)
            used[elem.first][elem.second] = false;
        for (int i = 0; i < cnt; i++)
            doFalse.pop_back();
        if (cnt == maxCycle)
        {
            if (food[player] > foodLater)
                flag = false;
            return {(food[player] - food[1 - player]) * 1LL * bigConst +
                     foodW[player] - foodW[1 - player], c};
        }
        else
        {
            return {-inf, c};
        }
    }
    long long value = -inf;
    int dirX = (snakes[currPlayer].front().first -
                 snakes[currPlayer][1].first);
    int dirY = (snakes[currPlayer].front().second -
                 snakes[currPlayer][1].second);
    for (auto elem : way)
    {
        int x;
        int y;
        if (elem == 'F')
        {
            x = dirX;
            y = dirY;
        }
        else if (elem == 'L')
        {
            x = -dirY;
            y = dirX;
        }
        else
        {
            x = dirY;
            y = -dirX;
        }
        auto ans = doAction(x, y, currPlayer);
        if (ans[0] == -1)
        {
            if (maxDepth == depth)
                valueFirstTurn[elem] = -inf;
            continue;
        }

        timer[currPlayer]++;
        auto new_value = alphaBeta(depth - 1, -turn, -b, -a, c);

        if (maxDepth == depth)
            valueFirstTurn[elem] = new_value.first;

        if (-new_value.first > value)
        {
            value = -new_value.first;
            c = elem;
        }
        timer[currPlayer]--;
        undoAction(currPlayer, ans[0], {ans[1], ans[2]});
        a = max(a, value);
        if (a >= b)
            break;
    }
    return {value, c};
}

char foodSearch()
{
    vector<vector<pair<char, int>>> dist(n, vector<pair<char, int>> (m, {'S', bigConst}));
    dist[snakes[player].front().first][snakes[player].front().second] = {'X', 0};

    queue<pair<int, int>> q;
    q.push({snakes[player].front().first, snakes[player].front().second});

    vector<vector<pair<int, int>>> prev(n, vector<pair<int, int>> (m));
    int dirX = (snakes[player].front().first -
                 snakes[player][1].first);
    int dirY = (snakes[player].front().second -
                 snakes[player][1].second);
    prev[snakes[player].front().first][snakes[player].front().second] = {dirX, dirY};

    int ansX = -1;
    int ansY = -1;
    int currLevel = -1;
    while (q.size())
    {
        auto elem = q.front();
        q.pop();

        int x = elem.first;
        int y = elem.second;
        if (dist[x][y].second > currLevel)
        {
            currLevel++;
            if (currLevel < snakes[player].size())
            {
                int distFromEnd = snakes[player].size() - 1 - currLevel;
                pole[snakes[player][distFromEnd].first][snakes[player][distFromEnd].second] = 0;
            }
        }

        for (auto elem : way)
        {
            dirX = prev[x][y].first;
            dirY = prev[x][y].second;
            if (elem == 'L')
            {
                dirX = dirX + dirY;
                dirY = dirX - dirY;
                dirX = dirY - dirX;
            }
            else if (elem == 'R')
            {
                dirY = dirX + dirY;
                dirX = dirY - dirX;
                dirY = dirX - dirY;
            }

            if (x + dirX < 0 ||
                x + dirX > 29 ||
                y + dirY < 0 ||
                y + dirY > 39)
                continue;

            if (pole[x + dirX][y + dirY] != -1 &&
                dist[x + dirX][y + dirY].first == 'S')
            {
                if (dist[x][y].first == 'X')
                    dist[x + dirX][y + dirY] = {elem, dist[x][y].second + 1};
                else
                    dist[x + dirX][y + dirY] = {dist[x][y].first, dist[x][y].second + 1};
                prev[x + dirX][y + dirY] = {dirX, dirY};
                q.push({x + dirX, y + dirY});
            }

            if (pole[x + dirX][y + dirY] == 1)
                return dist[x + dirX][y + dirY].first;
        }
    }
    return 'S';
}

int main()
{
    cin >> player;
    player--;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            char c;
            cin >> c;
            pole[i][j] = c - '0';
        }
    for (int i = 0; i < 4; i++)
        pole[i][0] = -1;
    for (int i = 0; i < 4; i++)
        pole[n - 1 - i][m - 1] = -1;

    int currPlayer = 0;
    while (true)
    {
        int dirX = (snakes[currPlayer].front().first -
                    snakes[currPlayer][1].first);
        int dirY = (snakes[currPlayer].front().second -
                    snakes[currPlayer][1].second);

        char c;
        if (currPlayer == player)
        {
            flag = true;
            foodLater = food[player];
            auto ans = alphaBeta(maxDepth, 1, -inf, inf, 'F');
            c = ans.second;
            if (flag)
            {
                char c2 = foodSearch();
                for (auto elem : snakes[player])
                    pole[elem.first][elem.second] = -1;
                if (c2 != 'S' && valueFirstTurn[c2] != -inf)
                    c = c2;
            }
            cout << c << endl;
        }
        else
        {
            cin >> c;
        }
        int x;
        int y;
        if (c == 'F')
        {
            x = dirX;
            y = dirY;
        }
        else if (c == 'L')
        {
            x = -dirY;
            y = dirX;
        }
        else
        {
            x = dirY;
            y = -dirX;
        }
        doAction(x, y, currPlayer);
        timer[currPlayer]++;
        currPlayer = (currPlayer + 1) % 2;
    }

    return 0;
}

