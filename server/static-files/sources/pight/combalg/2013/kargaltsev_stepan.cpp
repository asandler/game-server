#include <cstdio>
#include <cstdlib>
#include <vector>
#include <utility>
#include <queue>
#include <climits>
#include <iostream>

#define DEPTH 8

using namespace std;

const int H = 30;
const int W = 40;

char changeDirection[256][256];
char antiChange[256][256];
void initChangeDirection()
{
    changeDirection['U']['U'] = 'F';
    changeDirection['U']['L'] = 'L';
    changeDirection['U']['R'] = 'R';
    changeDirection['D']['L'] = 'R';
    changeDirection['D']['R'] = 'L';
    changeDirection['D']['D'] = 'F';
    changeDirection['L']['L'] = 'F';
    changeDirection['L']['D'] = 'L';
    changeDirection['L']['U'] = 'R';
    changeDirection['R']['R'] = 'F';
    changeDirection['R']['D'] = 'R';
    changeDirection['R']['U'] = 'L';
}

void initAntiChange()
{
    antiChange['U']['F'] = 'U';
    antiChange['U']['L'] = 'L';
    antiChange['U']['R'] = 'R';
    antiChange['D']['F'] = 'D';
    antiChange['D']['L'] = 'R';
    antiChange['D']['R'] = 'L';
    antiChange['L']['F'] = 'L';
    antiChange['L']['L'] = 'D';
    antiChange['L']['R'] = 'U';
    antiChange['R']['F'] = 'R';
    antiChange['R']['L'] = 'U';
    antiChange['R']['R'] = 'D';
}

char myCurrentDirection;
char hisCurrentDirection;
int whoAmI;

short Map[H][W];

struct coords
{
        short int x;
        short int y;
        coords(int x, int y) : x(x), y(y) {}
        bool operator==(const coords a) const
        {
            return (x == a.x && y == a.y);
        }
        bool operator!=(const coords a) const
        {
            return (x != a.x || y != a.y);
        }
};

void printMap()
{
    printf("--------\n");
    for (int i = 0; i < H; ++i)
    {
        printf("|");
        for (int j = 0; j < W; ++j)
        {
            printf("%hd", Map[i][j]);
        }
        printf("|\n");
    }
    printf("--------\n");
}

struct Snake
{
    deque <coords> body;
    char currentDirection;
    int myNumber;
    Snake(){}
    Snake(int whoAmI)
    {
        myNumber = whoAmI;
        if (whoAmI == 2)
        {
            body.push_back(coords(W - 1, H - 1));
            body.push_back(coords(W - 1, H - 2));
            body.push_back(coords(W - 1, H - 3));
            body.push_back(coords(W - 1, H - 4));
            currentDirection = 'U';
        }
        else
        {
            body.push_back(coords(0, 0));
            body.push_back(coords(0, 1));
            body.push_back(coords(0, 2));
            body.push_back(coords(0, 3));
            currentDirection = 'D';
        }
    }
    void makeMove(char toWhere, bool changeMyDir = 0)
    {
        //printf("! %d\n", myNumber);
        //printMap();
        if (changeMyDir)            
            toWhere = antiChange[currentDirection][toWhere];
        currentDirection = toWhere;
        coords curHeadPos = body.back();
        if (Map[body.front().y][body.front().x] != 2 * myNumber)
        {
            Map[body.front().y][body.front().x] = 2 * myNumber;
        }
        else
        {
            Map[body.front().y][body.front().x] = 0;
            body.pop_front();
        }
        if (toWhere == 'U')
            body.push_back(coords(curHeadPos.x, curHeadPos.y - 1));
        else if (toWhere == 'D')
            body.push_back(coords(curHeadPos.x, curHeadPos.y + 1));
        else if (toWhere == 'L')
            body.push_back(coords(curHeadPos.x - 1, curHeadPos.y));
        else
            body.push_back(coords(curHeadPos.x + 1, curHeadPos.y));
        Map[body.back().y][body.back().x] += 2 * myNumber;
    }
    void moveBack(char oldDir, coords oldTailCoords)
    {
        //printf("!!! %d\n", myNumber);
        //printMap();
        currentDirection = oldDir;
        Map[body.back().y][body.back().x] -= 2 * myNumber;
        body.pop_back();
        if (body.front() != oldTailCoords)
        {
            body.push_front(oldTailCoords);
            Map[oldTailCoords.y][oldTailCoords.x] += 2 * myNumber;
        }
        else
        {
            Map[oldTailCoords.y][oldTailCoords.x] += 1;
        }
    }
};

Snake me, he;

coords myTail(bool his = 0)
{
    return (his ? he.body.front() : me.body.front());
}

pair <pair <short, int>, char> analysePosition(bool his = 0)
{
    int currentNumber = (his ? he.myNumber : me.myNumber);
    coords start = (his ? he.body.back() : me.body.back());
    vector <vector <int> > dist(H, vector <int>(W, INT_MAX - 1));
    vector <vector <coords> > parent(H, vector <coords>(W, coords(-1, -1)));
    //printf("%d %d\n", start.x, start.y);
    dist[start.y][start.x] = 0;
    parent[start.y][start.x] = start;
    queue <coords> q;
    q.push(start);
    coords optimal(-1, -1);
    bool flag = 0;
    while(q.size())
    {
        coords cur = q.front();
        q.pop();
        if (Map[cur.y][cur.x] == 0 && dist[cur.y][cur.x] != INT_MAX - 1 && optimal.x == -1)
        {
            optimal = cur;
            flag = 1;
        }
        if (Map[cur.y][cur.x] == 1 && (flag || optimal.x == -1 || dist[cur.y][cur.x] < dist[optimal.y][optimal.x]))   
        {
            optimal = cur;
            flag = 0;
        }
        if (!(optimal.x == -1 || flag || dist[cur.y][cur.x] <= dist[optimal.y][optimal.x]))
            continue;
        if (cur.y && (Map[cur.y - 1][cur.x] <= 1 || (coords(cur.y - 1, cur.x) == myTail(his) && Map[cur.y - 1][cur.x] == currentNumber * 2)) && dist[cur.y - 1][cur.x] > dist[cur.y][cur.x] + 1)
        {
            dist[cur.y - 1][cur.x] = dist[cur.y][cur.x] + 1;
            parent[cur.y - 1][cur.x] = cur;
            q.push(coords(cur.x, cur.y - 1));
        }
        if (cur.y < H - 1 && (Map[cur.y + 1][cur.x] <= 1 || (coords(cur.y + 1, cur.x) == myTail(his) && Map[cur.y + 1][cur.x] == currentNumber * 2)) && dist[cur.y + 1][cur.x] > dist[cur.y][cur.x] + 1)
        {
            dist[cur.y + 1][cur.x] = dist[cur.y][cur.x] + 1;
            parent[cur.y + 1][cur.x] = cur;
            q.push(coords(cur.x, cur.y + 1));
        }
        if (cur.x && (Map[cur.y][cur.x - 1] <= 1 || (coords(cur.y, cur.x - 1) == myTail(his) && Map[cur.y][cur.x - 1] == currentNumber * 2)) && dist[cur.y][cur.x - 1] > dist[cur.y][cur.x] + 1)
        {
            dist[cur.y][cur.x - 1] = dist[cur.y][cur.x] + 1;
            parent[cur.y][cur.x - 1] = cur;
            q.push(coords(cur.x - 1, cur.y));
        }
        if (cur.x < W - 1 && (Map[cur.y][cur.x + 1] <= 1 || (coords(cur.y, cur.x + 1) == myTail(his) && Map[cur.y][cur.x + 1] == currentNumber * 2)) && dist[cur.y][cur.x + 1] > dist[cur.y][cur.x] + 1)
        {
            dist[cur.y][cur.x + 1] = dist[cur.y][cur.x] + 1;
            parent[cur.y][cur.x + 1] = cur;
            q.push(coords(cur.x + 1, cur.y));
        }
    }
    short whereCanWeGo = !flag;
    //printf("Flag : %d, wcwg: %hd\n", int(flag), whereCanWeGo);
    if (optimal.x == -1)
        return make_pair(make_pair(-1, INT_MAX), 'U');
        //throw "Всё тлен:(\n";
    int answerDist = INT_MAX;
    if (optimal.x != -1)
        answerDist = dist[optimal.y][optimal.x];
    while (parent[optimal.y][optimal.x] != start)
    {
        optimal = parent[optimal.y][optimal.x];
    }
//     printf("WSIG : %d %d %d\n",(his ? he.myNumber: me.myNumber), optimal.x - start.x, optimal.y - start.y);
    if (optimal.x - start.x == 1)
        return make_pair(make_pair(whereCanWeGo, answerDist), 'R');
    if (optimal.x - start.x == -1)
        return make_pair(make_pair(whereCanWeGo, answerDist), 'L');
    if (optimal.y - start.y == 1)
        return make_pair(make_pair(whereCanWeGo, answerDist), 'D');
    if (optimal.y - start.y == -1)
        return make_pair(make_pair(whereCanWeGo, answerDist), 'U');
    return make_pair(make_pair(whereCanWeGo, answerDist), 'U');
}



#define result pair< pair<int, short>, pair<int, char> > //how many we eat, what situation will be, 

bool mycomp(result a, result b)
{
    if (a.first.first == b.first.first)
    {
        if (a.first.second == b.first.second)
        {
            if (a.second.first == b.second.first)
                return false;
            return a.second.first < b.second.first;
        }
        return a.first.second < b.first.second;
    }
    return a < b;
}

void changeAlphaBeta(result &what, result to, char dir, bool his)
{
    if ((his && (mycomp(to, what) || to.first.second == -1)) || (!his && mycomp(what, to) && to.first.second != -1))
    {
        //printf("!!!!!!!!! %d %c\n", int(his), dir);
        what = to;
        what.second.second = dir;
//         printf("iAH: %c\n", dir);
    }
}

void printResult(result a)
{
    printf("! %d %hd %d %c\n", a.first.first, a.first.second, a.second.first, a.second.second);
}

result miniMax(int depth, result alpha, result beta, int eaten, bool his = 0)
{
    if (!depth)
    {
        pair <pair <short, int>, char> t = analysePosition();
        //printf("t.f.f: %d\n", t.first.first); 
        if (eaten == 1)
        {
            //printf("Co: %d %d\n", me.body.back().x, me.body.back().y);
        }
        return make_pair(make_pair(eaten, t.first.first), make_pair(-t.first.second, t.second));
    }
    int currentNumber = (his ? he.myNumber : me.myNumber);
    Snake *curSnake = (his ? &he : &me);
    coords oldTailCoords = curSnake->body.front();
    char oldDir = curSnake->currentDirection;
    bool flag = 1;
    result t;
    coords cur = curSnake->body.back();
    if (cur.y < H - 1 && flag && (Map[cur.y + 1][cur.x] <= 1 || (coords(cur.y + 1, cur.x) == myTail(his) && Map[cur.y + 1][cur.x] == currentNumber * 2)))
    {
        curSnake->makeMove('D');
        //printf("%d %d %d\n", depth, curSnake->body.back().x, curSnake->body.back().y);
//         printf("%d 0\n", depth);
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            ++eaten;
        t = miniMax(depth - 1, alpha, beta, eaten, !his);
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            --eaten;
        //if (depth == 6)
            //printf("D: %d, T.f.f: %d, %hd\n", depth, t.first.first, t.first.second);
        changeAlphaBeta((his ? beta : alpha), t, 'D', his);
        curSnake->moveBack(oldDir, oldTailCoords); //What to do here?
        if (beta <= alpha)
            flag = 0;
    }
    if (cur.y && flag && (Map[cur.y - 1][cur.x] <= 1 || (coords(cur.y - 1, cur.x) == myTail(his) && Map[cur.y - 1][cur.x] == currentNumber * 2)))
    {
        curSnake->makeMove('U');\
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            ++eaten;
        t = miniMax(depth - 1, alpha, beta, eaten, !his);
        changeAlphaBeta((his ? beta : alpha), t, 'U', his);
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            --eaten;
        curSnake->moveBack(oldDir, oldTailCoords); //What to do here?
        if (beta <= alpha)
            flag = 0;
    }
    if (cur.x < W - 1 && flag && (Map[cur.y][cur.x + 1] <= 1 || (coords(cur.y, cur.x + 1) == myTail(his) && Map[cur.y][cur.x + 1] == currentNumber * 2)))
    {
        curSnake->makeMove('R');
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            ++eaten;
        //if (depth == 6)
          //  printf("%d\n", eaten);
        t = miniMax(depth - 1, alpha, beta, eaten, !his);
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            --eaten;
        //if (depth == 6)
        //{
          //  printf("%d %d\n", curSnake->body.back().y, curSnake->body.back().x);
            //printf("SA: %d, %hd, %d, %c\n", alpha.first.first, alpha.first.second, alpha.second.first, alpha.second.second);
            //printf("ST: %d, %hd, %d, %c\n", t.first.first, t.first.second, t.second.first, t.second.second);
        //}
        changeAlphaBeta((his ? beta : alpha), t, 'R', his);
        //if (depth == 6)
            //printf("C: %c\n", alpha.second.second);
        curSnake->moveBack(oldDir, oldTailCoords); //What to do here?
        if (beta <= alpha)
            flag = 0;
    }
    if (cur.x && flag && (Map[cur.y][cur.x - 1] <= 1 || (coords(cur.y, cur.x - 1) == myTail(his) && Map[cur.y][cur.x - 1] == currentNumber * 2)))
    {
        curSnake->makeMove('L');
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            ++eaten;
        t = miniMax(depth - 1, alpha, beta, eaten, !his);
        if (!his && Map[curSnake->body.back().y][curSnake->body.back().x] % 2)
            --eaten;
        changeAlphaBeta((his ? beta : alpha), t, 'L', his);
        //if (depth == 6)
          //  printf("C: %c\n", alpha.second.second);
        curSnake->moveBack(oldDir, oldTailCoords); //What to do here?
        if (beta <= alpha)
            flag = 0;
    }
    return (his ? beta : alpha); 
}

char randTurn()
{
   int k = rand() % 3;
   return (k == 0 ? 'L' : k == 1 ? 'F' : 'R');
}


char charScanf()
{
    char c;
    do
        scanf("%c", &c);
    while (c != 'F' && c != 'L' && c != 'R');
    return c;
}

int main()
{
    initChangeDirection();
    initAntiChange();
    scanf("%d", &whoAmI);
    me = Snake(whoAmI);
    he = Snake(3 - whoAmI);
    char c;
    for (int i = 0; i < H; ++i)
    {
        for (int j = 0; j < W; ++j)
        {
            do
                scanf("%c", &c);
            while(c != '0' && c != '1');
            Map[i][j] = c - '0';
        }
    }
    Map[3][0] = Map[2][0] = Map[1][0] = (Map[0][0] += (whoAmI == 2 ? 4 : 2));
    Map[H - 4][W - 1] = Map[H - 3][W - 1] = Map[H - 2][W - 1] = (Map[H - 1][W - 1] += (whoAmI == 1 ? 4 : 2));
    //printMap();
    if (whoAmI == 2)
    {
        he.makeMove(charScanf(), 1);
    }
    while (true)
    {
        char c = miniMax(DEPTH, make_pair(make_pair(0, -1), make_pair(INT_MAX, 'U')), make_pair(make_pair(1000, 1), make_pair(INT_MIN, 'U')), 0).second.second;
        printf("%c\n", changeDirection[me.currentDirection][c]);
        cout.flush();
        me.makeMove(c);
        he.makeMove(charScanf(), 1);
        //printMap();
    }
    return 0;
}
