#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdio>
#include <ctime>

class table{
    struct st{
        int live, dead;
    };
    std::vector<std::vector<int> > t;
    //[x][y]
    std::vector<std::pair<int, int> > validMoves[2], accesiblePoints[2];
    std::vector<std::pair<int, int> > dXY;
    std::vector<std::vector<bool> > used;
    void buildMovesPL(int pl, std::vector<std::vector<bool> >& used);
    void addAccessible(int x, int y, int pl, std::vector<std::vector<bool> >& used, bool);
    bool isValidPos(int x, int y);
    public:
        st players[2];
        static const int empt = 0;
        size_t size();
        std::vector<int>& operator[] (int pos);
        std::vector<std::pair<int, int> >& getValidMoves(int pl);
        std::vector<std::pair<int, int> >& getAccesiblePoints(int pl);
        void buildValidMoves();
        int nextPlayer(int pl);
        bool isValidMove(std::pair<int, int> move, int player);
        void move(std::vector<std::pair<int, int> > currMv, int);
        table();
};


std::vector<std::pair<int, int> >& table::getValidMoves(int pl){
    return validMoves[pl];
}

size_t table::size(){
    return t.size();
}

bool table::isValidPos(int x, int y){
    return (0 <= x &&  x < 10 && 0 <= y && y < 10);
}

std::vector<std::pair<int, int> >& table::getAccesiblePoints(int pl){
    return accesiblePoints[pl];
}

void table::addAccessible(int x, int y, int pl, std::vector<std::vector<bool> >& used, bool wasD){
    if(used[x][y] || t[x][y] == players[pl].dead || (t[x][y] == empt && wasD)) return;
    used[x][y] = true;
    if(t[x][y] == players[nextPlayer(pl)].live || t[x][y] == empt){
        validMoves[pl].push_back(std::make_pair(x, y));
        return;
    }
    accesiblePoints[pl].push_back(std::make_pair(x, y));
    for(int i = 0; i < (int)dXY.size(); ++i)
        if(isValidPos(x + dXY[i].first, y + dXY[i].second))
            addAccessible(x + dXY[i].first, y + dXY[i].second, pl, used, wasD || t[x][y] == players[nextPlayer(pl)].dead);
}

void table::buildMovesPL(int pl, std::vector<std::vector<bool> >& used){
    validMoves[pl].clear();
    accesiblePoints[pl].clear();
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if(t[i][j] == players[pl].live)
                addAccessible(i, j, pl, used, false);
}

void table::buildValidMoves(){
    for(int pl = 0; pl < 2; ++pl){
        for(int i = 0; i < 10; ++i)
            for(int j = 0; j < 10; ++j)
                used[i][j] = 0;
        buildMovesPL(pl, used);
    }
    return;
}

bool table::isValidMove(std::pair<int, int> move, int player){
    if(t[move.first][move.second] == empt)
        return true;
    if(t[move.first][move.second] == players[nextPlayer(player)].live)
        return true;
    return false;
}

int table::nextPlayer(int pl){
    return (pl + 1) % 2;
}

void table::move(std::vector<std::pair<int, int> > currMv, int player){
    for(size_t i = 0; i < currMv.size(); ++i){
        assert(isValidMove(currMv[i], player));
        if(t[currMv[i].first][currMv[i].second] == players[nextPlayer(player)].live)
            t[currMv[i].first][currMv[i].second] = players[nextPlayer(player)].dead;
        else
            t[currMv[i].first][currMv[i].second] = players[player].live;
    }
    return;
}

std::vector<int>& table::operator[] (int pos)
{
    assert(0 <= pos && pos <= (int)t.size());
    return t[pos];
}

table::table(){
    t.resize(10);

    for(int i = 0; i < 10; ++i)
        t[i].resize(10, 0);
    used.resize(10);
    for(int i = 0; i < 10; ++i)
        used[i].resize(10, false);
    players[0].dead = -2;
    players[0].live = -1;
    players[1].dead = 2;
    players[1].live = 1;
    for(int i = -1; i < 2; ++i)
        for(int j = -1; j < 2; ++j)
            if(i != 0 || j != 0)
                dXY.push_back(std::make_pair(i, j));
}

using std::vector;
using std::pair;
using std::max;
using std::make_pair;
using std::min;
using std::cout;
using std::endl;
using std::cerr;
const int SafeFarm = -1, War = -2;
const int INF = 1e5;
table tb;
int st = SafeFarm;
int pl, it = 0;

void readMove(vector<pair<int, int> > &mv){
    using std::cin;
    char x, y;
    for(int i = 0; i < 3; ++i){
        cin >> x >> y;
        mv.push_back(std::make_pair(x - '0', y - '0'));
    }
    return;
}

void printMove(vector<pair<int, int> > mv){
    //using std::cout;
    for(int i = 0; i < (int)mv.size(); ++i){
        cout << mv[i].first << mv[i].second;
        //fprintf(foutlog, "%d%d", mv[i].first, mv[i].second);
        //fprintf(stderr, "%d%d", mv[i].first, mv[i].second);
    }
   // fprintf(foutlog, "\n");
}

bool isValidPos(int x, int y){
    return (0 <= x && x < 10 && 0 <= y && y < 10);
}

void relaxDt(vector<vector<int> >& dt, int pl, int x, int y){
    dt[x][y] = 0;
    for(int i = -5; i <= 5; ++i)
        for(int j = -5; j <= 5; ++j)
            if(isValidPos(x + i, y + j))
                if(tb[x + i][y + j] != tb.players[tb.nextPlayer(pl)].dead)
                    dt[x + i][y + j] = min(dt[x + i][y + j], dt[x][y] + max(abs(i), abs(j)));
}

void buildDist(vector<vector<int> > &dt, int pl){
    //fprintf(foutlog, "bd");
    dt.clear();
    dt.resize(10);
    tb.buildValidMoves();
    for(int i = 0; i < 10; ++i)
        dt[i].resize(10, INF);
    vector<pair<int, int> > acsPt = tb.getAccesiblePoints(pl);
    for(int i = 0; i < (int) acsPt.size(); ++i)
        relaxDt(dt, pl, acsPt[i].first, acsPt[i].second);
}

inline bool eqRand(pair<int, int> p1, pair<int, int> p2){
    return (p1.first + p1.second == p2.first + p2.second && rand() % 2 == 0);
}

bool haveWeAtacked(vector<pair<int, int> >opponentMoves, int pl, pair<int, int> &p){
    bool ans = false; 
    for(int i = 0; i < (int) opponentMoves.size(); ++i)
        if(tb[opponentMoves[i].first][opponentMoves[i].second] == tb.players[pl].dead)
            ans = true;
        else if(tb[opponentMoves[i].first][opponentMoves[i].second] == tb.players[tb.nextPlayer(pl)].live)
            p = opponentMoves[i];
    return ans;
}

int getCntUndHit(int x, int y, vector<vector<bool> > &used, vector<vector<int> > &w, int pl){
    if(used[x][y] || tb[x][y] == tb.empt || tb[x][y] == tb.players[tb.nextPlayer(pl)].live || tb[x][y] == tb.players[tb.nextPlayer(pl)].dead) return 0;
    if(tb[x][y] == tb.players[pl].live) return w[x][y];
    used[x][y] = true;
    int ans = 0;
    for(int i = -1; i <= 2; ++i)
        for(int j = -1; j <= 2; ++j)
            if(i != j && isValidPos(x + i, y + j))
                ans += getCntUndHit(x + i, y + j, used, w, pl);
    return ans;
}


int addWeight(int x, int y, vector<vector<bool> > &used, int pl){
    if(used[x][y] || tb[x][y] == tb.empt || tb[x][y] == tb.players[pl].live || tb[x][y] == tb.players[pl].dead) return 0;
    if(tb[x][y] == tb.players[tb.nextPlayer(pl)].live) return 1; 
    used[x][y] = true;
    int ans = 0;
    for(int i = -1; i < 2; ++i){
       for(int j = -1; j < 2; ++j){
            if((i != 0 || j != 0) && isValidPos(x + i, y + j))
                ans += addWeight(x + i, y + j, used, pl);
        }
    }
    return ans;
}


template<class t>
void init(vector<vector<t> > &v, int x, int y, t val){
    v.resize(x);
    for(int i = 0; i < x; ++i)
        v[i].resize(y, val);
}

void relaxWeight(int x, int y, vector<vector<int> > &w, int pl){
    vector<vector<bool> > used;
    int add = 0;
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            if((i != 0 || j != 0) && isValidPos(x + i, y + j)){
                init(used, 10, 10, false);
                add += addWeight(x + i, y + j, used, pl);
            }
        }
    }
    w[x][y] += add;
}

void buildWeight(vector<vector<int> > &w, int pl){
    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < 10; ++j){
            relaxWeight(i, j, w, pl);
        }
    }
}

double getAttackC(vector<vector<int> > &dt, vector<vector<int> > &weight, pair<int, int> p1, int pl){
    double ans = 0;
    ans += 2*(3 - dt[p1.first][p1.second]);
    double tp = 0;
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j)
            tp += (isValidPos(p1.first + i, p1.second + j) && tb[p1.first + i][p1.second + j] == tb.players[tb.nextPlayer(pl)].live);
    ans -= tp;
    tp = 0;
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j)
            tp += (isValidPos(p1.first + i, p1.second + j) && tb[p1.first + i][p1.second + j] == tb.empt);
    ans += tp;
    tp = 0;
    vector<vector<bool> > used;
    init(used, 10, 10, false);
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j)
            if(isValidPos(p1.first + i, p1.second + j))
                tp += getCntUndHit(p1.first + i, p1.second + j, used, weight, pl);
            //tp += (isValidPos(p1.first + i, p1.second + j) && tb[p1.first + i][p1.second + j] == tb.players[tb.nextPlayer(pl)].dead);
    ans += tp;
    return ans;
}

bool isBetterForAttack(vector<vector<int> > &dt, vector<vector<int> > &w, pair<int, int> p1, pair<int, int> p2){
    if(p1.first == -1 && p1.second == -1) return true;
    double c1 = getAttackC(dt, w, p1, pl), c2 = getAttackC(dt, w, p2, pl);
    return (c1 < c2);
}

int dist(pair<int, int> p1, pair<int, int> p2){
    return (max(abs(p1.first - p2.first), abs(p1.second - p2.second)));
} 

pair<int, int> getOptimal(vector<vector<int> > &dt, vector<pair<int, int> > &opponentMoves, vector<pair<int, int> >& validMoves, int pl, int &st){
    switch(st){
    case(SafeFarm):{
        pair<int, int> p(-1, - 1);
        int cm = 0;
        for(int i = 0; i < (int)validMoves.size(); ++i){
            if(dt[validMoves[i].first][validMoves[i].second] > 2) ++cm;
            if(pl == 0 && (p.first + p.second < validMoves[i].first + validMoves[i].second || eqRand(p, validMoves[i])) && dt[validMoves[i].first][validMoves[i].second] > 2){
                p = validMoves[i];
            }
            if(pl == 1 && (p.first + p.second > validMoves[i].first + validMoves[i].second || p.first == -1 || eqRand(p, validMoves[i])) && dt[validMoves[i].first][validMoves[i].second] > 2){
                p = validMoves[i];
            }
        }
        //cerr << cm << endl;
        if(cm >= 3 && p != make_pair(-1, -1))
            return p;
        else
        {
            st = War;
            return getOptimal(dt, opponentMoves, validMoves, pl, st);
        }
    }
    case(War):{
        //fprintf(foutlog, "WAR\n");
        vector<pair<int, int> > mv = tb.getValidMoves(pl);
        vector<vector<int> > myDt;
        buildDist(myDt, pl);
        tb.buildValidMoves();
        vector<vector<int> > weight;
        init(weight, 10, 10, 1);
        buildWeight(weight, pl);
        pair<int, int> p(-1, -1);
        vector<vector<bool> > oppAcs;
        oppAcs.resize(10);        
        for(int i = 0; i < (int)tb.size(); ++i)
            for(int j = 0; j < (int)tb[i].size(); ++j)
                if(tb[i][j] == tb.players[tb.nextPlayer(pl)].live && isBetterForAttack(myDt, weight, p, make_pair(i, j)))
                    p = make_pair(i, j);
        if(p.first != -1){
            pair<int, int> ans = mv[0];                
            for(int i = 0; i < (int)mv.size(); ++i)
                if(dist(mv[i], p) < dist(ans, p))
                    ans = mv[i];
            return ans;
        }
        return validMoves[rand() % validMoves.size()];
    }
    default:{
        assert(false);
    }
    }
}

void checkForChangeSt(vector<vector<int> > &dt, int player, int &st){
    switch(st){
        case(SafeFarm):{
            for(int i = 0; i < (int)tb.size(); ++i)
                for(int j = 0; j < (int)tb[i].size(); ++j)
                    if(tb[i][j] == tb.players[player].live && dt[i][j] <= 2){
                        st = War;
                        return;
                    }
        }
    }
}

void correctExit(){
    cout << std::endl;
    //std::cerr << std::endl << std::endl;
    exit(0); 
}

void makeMove(vector<pair<int, int> > &myMove, vector<pair<int, int> > &opponentMove, int player, int &st, int cnt = 3){
    myMove.clear(); 
    vector<vector<int> > dt;
    buildDist(dt, tb.nextPlayer(pl));
    checkForChangeSt(dt, player, st);
    switch(st){
        case(SafeFarm):{
            vector<pair<int, int> > validMoves, tmp;
            for(int i = 0; i < cnt; ++i){
                tb.buildValidMoves();
                tmp.clear();
                validMoves = tb.getValidMoves(pl);
                if(validMoves.size() == 0) correctExit();
                buildDist(dt, tb.nextPlayer(pl));
                pair<int, int> p = getOptimal(dt, opponentMove, validMoves, pl, st);
                tmp.push_back(p);
                myMove.push_back(p);
                tb.move(tmp, pl);
                printMove(tmp);
            }
            break;
        }
        case(War):{
            vector<pair<int, int> > validMoves, tmp;
            for(int i = 0; i < cnt; ++i){
                tb.buildValidMoves();
                tmp.clear();
                validMoves = tb.getValidMoves(pl);
                if(validMoves.size() == 0) correctExit();
                buildDist(dt, tb.nextPlayer(pl));
                pair<int, int> p = getOptimal(dt, opponentMove, validMoves, pl, st);
                tmp.push_back(p);
                myMove.push_back(p);
                tb.move(tmp, pl);
                printMove(tmp);
            }
            break;
        }
        default:{
            tb.buildValidMoves();
            vector<pair<int, int> > validMoves = tb.getValidMoves(player), tmp;
            if(validMoves.size() == 0) exit(0);
            random_shuffle(validMoves.begin(), validMoves.end());
            for(int i = 0, cp = 0; i < cnt; ++i, ++cp){
                myMove.push_back(validMoves[cp]);
                tmp.clear();
                tmp.push_back(validMoves[cp]);
                tb.move(tmp, player);
                printMove(tmp);
                myMove.clear();
                validMoves = tb.getValidMoves(pl);
                if(validMoves.size() == 0) exit(0);
                random_shuffle(validMoves.begin(), validMoves.end());
            }
        }
    }
    std::cout << std::endl;
}

void startPlayByFirst(){
    vector<pair<int, int> > myMove, opponentMove;
    myMove.push_back(make_pair(0, 0));
    printMove(myMove);
    tb.move(myMove, pl);
    tb.buildValidMoves();
    myMove.clear();
    makeMove(myMove, opponentMove, 0, st, 2);
    cout.flush();
    return;
}

void startPlayBySecond(){
    vector<pair<int, int> > myMove, opponentMove;
    readMove(opponentMove);
    tb.move(opponentMove, tb.nextPlayer(pl));
    tb.buildValidMoves();
    myMove.clear();
    opponentMove.clear();
    ++it;
    myMove.push_back(std::make_pair(9, 9));
    printMove(myMove);
    tb.move(myMove, pl);
    myMove.clear();
    makeMove(myMove, opponentMove, pl, st, 2);
    cout.flush();
    return;
}

void play(){
    vector<pair<int, int> > myMove, opponentMove;
    //fprintf(stderr, "\n");
    int time = 0;
    while(true){
        time = clock();
        opponentMove.clear();
        readMove(opponentMove);
        tb.move(opponentMove, tb.nextPlayer(pl));
        myMove.clear();
        ++it;
        makeMove(myMove, opponentMove, pl, st);
        tb.buildValidMoves();
        vector<pair<int, int> > mv = tb.getValidMoves(pl);
        time = clock() - time;
        //cerr << (double)time / CLOCKS_PER_SEC << endl;
    }
}

int main(){
    srand(179);
    using std::cin;
    cin >> pl;
    --pl;
    if (pl == 0){
        startPlayByFirst();
    }
    else{
        startPlayBySecond();
    }
    play();
    return 0;
}
