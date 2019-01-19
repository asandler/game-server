#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<algorithm>
#include<fstream>
#include<vector>
#include<queue>
#define fieldW 40
#define fieldH 30
#define StdSnakeSize 4
#define AngryProb 0.4
/*
   W - Wall. End of field.
   F - Eated Food.
   S - Snake.
   1 - Food
   0 - Nothing, i hope.
   */
using namespace std;
int FCount = 0;
int turn;
int step = 0;
char field[fieldW + 10][fieldH + 10];
int stopFollow = 0;
int magicOneConst = 100;
int atackD = 0;
int t2;
struct Node {
    int x, y;
    Node() {
    }
    Node(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void move(int x, int y) {
        this->x += x;
        this->y += y;
    }
    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

Node tmp;
int tmpint;
char tmpchar;
int csafego;

double dRand() {
    return (long double)(rand() % 1000000) / 1000000;
}

int d[100][100];

struct Snake {
    vector<Node> body;
    int px;
    int py;
    Snake(int who) {
        if (who == 1) {
            px = 0;
            py = 1;
            body.resize(StdSnakeSize);
            for (int i = 1; i <= StdSnakeSize; ++i) {
                body[StdSnakeSize - i].set(1, i);
                field[1][i] = 'S';
            }
        }
        if (who == 2) {
            px = 0;
            py = -1;
            body.resize(StdSnakeSize);
            for (int i = 1; i <= StdSnakeSize; ++i) {
                body[i - 1].set(fieldW, fieldH - StdSnakeSize + i);
                field[fieldW][ fieldH - StdSnakeSize + i] = 'S';
            }
        }
    }
    char char_at_dir(char dir) {
        int x, y;
        if (dir == 'F') {
            x = this->px;
            y = this->py;
        }
        if (dir == 'L') {
            x = this->py;
            y = -this->px;
        }
        if (dir == 'R') {
            x = -this->py;
            y = this->px;
        }
        if (body[0].x + x > fieldW) return 'W';
        if (body[0].x + x < 1) return 'W';
        if (body[0].y + y > fieldH) return 'W';
        if (body[0].y + y < 1) return 'W';
        return field[body[0].x + x][body[0].y + y];
    }
    void go(char dir) {
        int x, y;
        if (dir == 'F') {
            x = this->px;
            y = this->py;
        }
        if (dir == 'L') {
            x = this->py;
            y = -this->px;
        }
        if (dir == 'R') {
            x = -this->py;
            y = this->px;
        }
        if (char_at_dir(dir) == '1') {
            field[body[0].x + x][body[0].y + y] = 'F';
            FCount--;
        } else
            field[body[0].x + x][body[0].y + y] = 'S';
        if (field[body[body.size() - 1].x][body[body.size() - 1].y] == 'F') {
            body.push_back(body[body.size() - 1]);
        }
        field[body[body.size() - 1].x][body[body.size() - 1].y] = '0';
        this->px = x;
        this->py = y;
        for (int i = (int)body.size() - 1; i > 0; --i)
            body[i] = body[i - 1];
        body[0].move(x, y);
    }
};
queue<int> q;
int dist(const Node &a, const Node &b) {
    return abs(b.x - a.x) + abs(b.y  - a.y);
}
Snake *mySnake, *enemySnake;
int myFoodDist;
int myFoodX, myFoodY;
int hisFoodDist;

int foodSum;
void bfs(int bx, int by) {
    myFoodDist = 1000000000;
    hisFoodDist = 1000000000;
    foodSum = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j)
            d[i][j] = -1;
    }
    d[bx][by] = 0;
    q.push(bx * 100 +  by);
    int cx, cy;
    while (!q.empty()) {
        cx = q.front() / 100;
        cy = q.front() % 100;
        q.pop();
        if (field[cx][cy] == '1')
            foodSum += d[cx][cy];
        if ((field[cx][cy] == '1')&&(dist(Node(cx, cy), mySnake->body[0]) < myFoodDist)) {
            myFoodDist = dist(Node(cx, cy), mySnake->body[0]);
            myFoodX = cx;
            myFoodY = cy;
        }
        if ((d[cx - 1][cy] == -1)&&((field[cx - 1][cy] == '0')||(field[cx - 1][cy] == '1'))) {
            d[cx - 1][cy] = d[cx][cy] + 1;
            q.push((cx - 1) * 100 + cy);
        }
        if ((d[cx + 1][cy] == -1)&&((field[cx + 1][cy] == '0')||(field[cx + 1][cy] == '1'))) {
            d[cx + 1][cy] = d[cx][cy] + 1;
            q.push((cx + 1) * 100 + cy);
        }
        if ((d[cx][cy - 1] == -1)&&((field[cx][cy - 1] == '0')||(field[cx][cy - 1] == '1'))) {
            d[cx][cy - 1] = d[cx][cy] + 1;
            q.push((cx) * 100 + cy - 1);
        }
        if ((d[cx][cy + 1] == -1)&&((field[cx][cy + 1] == '0')||(field[cx][cy + 1] == '1'))) {
            d[cx][cy + 1] = d[cx][cy] + 1;
            q.push((cx) * 100 + cy + 1);
        }
    }
    //foodSum = 0;
    if (myFoodDist == 1000000000) {myFoodX = enemySnake->body[0].x;myFoodY = enemySnake->body[0].y;}
}

bool IAmDangerous = false;
bool used[50][50];

void clearUsed() {
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; ++j)
            used[i][j] = false;
    }
}

int dfs(int x, int y) {
    int c = 0;
    used[x][y] = true;
    if ((field[x + 1][y] == '0') || (field[x + 1][y] == '1')) {
        if (!used[x + 1][y])
            return dfs(x + 1, y) + 1;
    }
    if ((field[x - 1][y] == '0') || (field[x - 1][y] == '1')) {
        if (!used[x - 1][y])
            return dfs(x - 1, y) + 1;
    }
    if ((field[x][y - 1] == '0') || (field[x][y - 1] == '1')) {
        if (!used[x][y - 1])
            return dfs(x, y - 1) + 1;
    }
    if ((field[x][y + 1] == '0') || (field[x][y + 1] == '1')) {
        if (!used[x][y + 1])
            return dfs(x, y + 1) + 1;
    }
    return 1;
}

void makeWalls() {
    for (int i = 0; i < fieldW; ++i) {
        field[i][0] = 'W';
        field[i][fieldH + 1] = 'W';
    }
    for (int i = 0; i < fieldH; ++i) {
        field[0][i] = 'W';
        field[fieldW + 1][i] = 'W';
    }

}
Node tmpNode;
int tmpInt;
bool nearlyGo();
int t1;
int enfoodSum;
char ch;
bool normalStep() {
    char curGo = 'N';
    tmpNode = mySnake->body[0];
    bfs(tmpNode.x, tmpNode.y);
    tmpInt = 1000000000;
    int x;
    int y;
    if ((mySnake->char_at_dir('F') == '1')||(mySnake->char_at_dir('F') == '0')) {
        x = mySnake->px;
        y = mySnake->py;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        
        if (t1 >= mySnake->body.size())
            if (d[myFoodX][myFoodY] * magicOneConst + foodSum <= tmpInt) {// - rand() % (magicOneConst * 1)) {
                curGo = 'F'; 
                tmpInt = d[myFoodX][myFoodY] * magicOneConst + foodSum;
            }
    }
    if ((mySnake->char_at_dir('L') == '1')||(mySnake->char_at_dir('L') == '0')) {
        x = mySnake->py;
        y = -mySnake->px;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        if (t1 >= mySnake->body.size())
            if (d[myFoodX][myFoodY] * magicOneConst + foodSum <= tmpInt) {// - rand() % (magicOneConst * 1)) {
                curGo = 'L';
                tmpInt = d[myFoodX][myFoodY] * magicOneConst + foodSum;
            }
    }
    if ((mySnake->char_at_dir('R') == '1')||(mySnake->char_at_dir('R') == '0')) {
        x = -mySnake->py;
        y = mySnake->px;
        clearUsed();
        bfs(tmpNode.x + x, tmpNode.y + y);
        t1 = dfs(tmpNode.x + x, tmpNode.y + y) ;
        if (t1 >= mySnake->body.size())
            if (d[myFoodX][myFoodY] * magicOneConst + foodSum <= tmpInt) { // - rand() % (magicOneConst * 1)) {
                curGo = 'R';
                tmpInt = d[myFoodX][myFoodY] * magicOneConst + foodSum;
            }
    }
    if (curGo == 'N') {
        return false;
    }
    mySnake->go(curGo);
    cout << curGo << endl;
    return true; 

}
int enFoodX, enFoodY;
bool agressiveStep() {
    if (dRand() < AngryProb) {
        stopFollow = 20;
        return false;
    }
    char curGo = 'N';
    tmpNode = mySnake->body[0];
    tmpInt = 1000000000;
    int x;
    int y;
    if ((mySnake->char_at_dir('F') == '1')||(mySnake->char_at_dir('F') == '0')) {
        x = mySnake->px;
        y = mySnake->py;
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y);
        enfoodSum = foodSum;
        enFoodX = myFoodX;
        enFoodY = myFoodY;
        t2 = dfs(enemySnake->body[0].x, enemySnake->body[0].y);
        if (t2 > enemySnake->body.size() / 2) t2 = 10000;
        cerr << enfoodSum << endl;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        if (field[tmpNode.x + x][tmpNode.y + y] == '1')
            enfoodSum = 100000;
        if (t1 >= mySnake->body.size())
            if (d[enFoodX][enFoodY] * 30 + d[myFoodX][myFoodY] * 30 + foodSum  - enfoodSum * 2 + t2 <= tmpInt) {// - rand() % (magicOneConst * 1)) {
                curGo = 'F'; 
                tmpInt = d[enFoodX][enFoodY] * 30 + d[myFoodX][myFoodY] * 30 + foodSum - enfoodSum * 2 + t2;
            } 
    }
    if ((mySnake->char_at_dir('L') == '1')||(mySnake->char_at_dir('L') == '0')) {
        x = mySnake->py;
        y = -mySnake->px;
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y); 
        t2 = dfs(enemySnake->body[0].x, enemySnake->body[0].y);
        if (t2 > enemySnake->body.size() / 2) t2 = 10000;
        enfoodSum = foodSum;
        enFoodX = myFoodX;
        enFoodY = myFoodY;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        if (field[tmpNode.x + x][tmpNode.y + y] == '1')
            enfoodSum = 100000;
        if (t1 >= mySnake->body.size())
        if (d[enFoodX][enFoodY] * 30 + d[myFoodX][myFoodY] * 30 + foodSum - enfoodSum * 2 + t2 <= tmpInt) {// - rand() % (magicOneConst * 1)) {
            curGo = 'L';
            tmpInt = d[enFoodX][enFoodY] * 30 + d[myFoodX][myFoodY] * 30 + foodSum - enfoodSum * 2 + t2;
        }
    }
    if ((mySnake->char_at_dir('R') == '1')||(mySnake->char_at_dir('R') == '0')) {
        x = -mySnake->py;
        y = mySnake->px;
        clearUsed();
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y);
        enFoodX = myFoodX;
        enFoodY = myFoodY;
        enfoodSum = foodSum;
        t2 = dfs(enemySnake->body[0].x, enemySnake->body[0].y);
        if (t2 > enemySnake->body.size() / 2) t2 = 10000;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        t1 = dfs(tmpNode.x + x, tmpNode.y + y) ;
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        if (field[tmpNode.x + x][tmpNode.y + y] == '1')
            enfoodSum = 100000;
        if (t1 >= mySnake->body.size())
            if (d[myFoodX][myFoodY] * 30 + d[enFoodX][enFoodY] * 30 + foodSum - enfoodSum * 2 + t2  <= tmpInt) { // - rand() % (magicOneConst * 1)) {
                curGo = 'R';
                tmpInt = d[myFoodX][myFoodY] * 30 + d[enFoodX][enFoodY] * 30 + foodSum - enfoodSum * 2 + t2;
            }
    }
    if (curGo == 'N') {
        return false;
    }
    mySnake->go(curGo);
    cout << curGo << endl;
    return true; 

}

bool fearStep() {
    if (dRand() < AngryProb) {
        stopFollow = 10;
        return false;
    }
    char curGo = 'N';
    tmpNode = mySnake->body[0];
    tmpInt = 1000000000;
    int x;
    int y;
    if ((mySnake->char_at_dir('F') == '1')||(mySnake->char_at_dir('F') == '0')) {
        x = mySnake->px;
        y = mySnake->py;
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y);
        enfoodSum = foodSum;
        cerr << enfoodSum << endl;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        
        if (t1 * 10 + d[myFoodX][myFoodY] >= tmpInt) {// - rand() % (magicOneConst * 1)) {
            curGo = 'F'; 
            tmpInt = t1 * 10 + d[myFoodX][myFoodY];
        }
    }
    if ((mySnake->char_at_dir('L') == '1')||(mySnake->char_at_dir('L') == '0')) {
        x = mySnake->py;
        y = -mySnake->px;
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y);
        enfoodSum = foodSum;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        clearUsed();
        t1 = dfs(tmpNode.x + x, tmpNode.y + y);
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        if (t1 * 10 + d[myFoodX][myFoodY] >= tmpInt) {// - rand() % (magicOneConst * 1)) {
            curGo = 'L';
            tmpInt = t1 * 10 + d[myFoodX][myFoodY];
        }
    }
    if ((mySnake->char_at_dir('R') == '1')||(mySnake->char_at_dir('R') == '0')) {
        x = -mySnake->py;
        y = mySnake->px;
        clearUsed();
        ch = field[tmpNode.x + x][tmpNode.y + y];
        field[tmpNode.x + x][tmpNode.y + y] = 'S';
        bfs(enemySnake->body[0].x, enemySnake->body[0].y);
        enfoodSum = foodSum;
        field[tmpNode.x + x][tmpNode.y + y] = ch;
        bfs(tmpNode.x + x, tmpNode.y + y);
        t1 = dfs(tmpNode.x + x, tmpNode.y + y) ;
        myFoodX = enemySnake->body[0].x + enemySnake->px;
        myFoodY = enemySnake->body[0].y + enemySnake->py;
        if (t1 * 2>= mySnake->body.size())
            if (t1 * 10 + d[myFoodX][myFoodY] >= tmpInt) { // - rand() % (magicOneConst * 1)) {
                curGo = 'R';
                tmpInt = t1 * 10 + d[myFoodX][myFoodY];
            }
    }
    if (curGo == 'N') {
        return false;
    }
    mySnake->go(curGo);
    cout << curGo << endl;
    return true; 

}

int main(void) {
    srand(time(NULL));
    atackD = 1;
    cin >> turn;
    for (int i = 1; i <= fieldH; ++i)
        for (int j = 1; j <= fieldW; ++j) {
            cin >> field[j][i];
            if (field[j][i] == '1')
                FCount++;
            //field[j][i] = '0';
        }
    mySnake = new Snake(turn);
    enemySnake = new Snake(2 - (turn + 1) % 2);
    if (turn == 1) {
        cout << "F" << endl;
        mySnake->go('F');
        step++;
    }
    char ch;
    while (true) {
        step++;
        cin >> ch;
        if (step < 50) atackD = 0;
        if (step >= 50) atackD = 1;
        if (step >= 100) atackD = 2;
        if (step >= 150) atackD = 3;
        if (step >= 200) atackD = 10000;
        //atackD = 1000000;
        //cerr << atackD << endl;
        cerr << step << endl;
        enemySnake->go(ch);
        bfs(mySnake->body[0].x, mySnake->body[0].y);
        if (FCount <= 0) {
            if (mySnake->body.size() > enemySnake->body.size()) {
                if (fearStep())
                    continue;
                cerr << "feARRRR" << endl;
            }
            else
            if (agressiveStep())
                continue;
        } else
        if (d[enemySnake->body[0].x][enemySnake->body[0].y] != -1)
            if (stopFollow <= 0)
                if (d[enemySnake->body[0].x + enemySnake->px][enemySnake->body[0].y + enemySnake->py] <= atackD * d[myFoodX][myFoodY])
                    if (agressiveStep())
                        continue;
        
        stopFollow--;
        if (normalStep())
            continue;
        if ((mySnake->char_at_dir('L') == '0') || (mySnake->char_at_dir('L') == '1')) {
            mySnake->go('L');
            cout << "L" << endl; } else
        if ((mySnake->char_at_dir('F') == '0') || (mySnake->char_at_dir('F') == '1')) {
            mySnake->go('F');
            cout << "F" << endl; } else
        if ((mySnake->char_at_dir('R') == '0') || (mySnake->char_at_dir('R') == '1')) {
            mySnake->go('R');
            cout << "R" << endl; 
        } else {
            cout << "F" << endl;
            cout << "U" << endl;
            cout << "C" << endl;
            cout << "K" << endl; 
            cout << "!" << endl;
        }
    }

}
