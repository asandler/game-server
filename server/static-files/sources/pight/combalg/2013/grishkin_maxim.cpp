#include <iostream>
#include <fstream>
#include <string>
#include <queue>

//#define DEBUG

using namespace std;

const int inf = 2000000000;
const int MAX_DEPTH = 7;

enum Object {None, Food, MyO, EnemyO};
const int h = 30;
const int w = 40;
Object Map[h][w];

enum Angle {F, R, L = -1};

Angle ctoa(char c) {
	switch (c) {
	case ('L'):
		return L;
	case ('R'):
		return R;
	case ('F'):
		return F;
	}
}

char atoc(Angle a) {
	switch (a) {
	case (F):
		return 'F';
	case (L):
		return 'L';
	case (R):
		return 'R';
	}
}

enum Direction {Up, Right, Down, Left};

Direction directionByVector(int dx, int dy) {
	if (dx == -1) {
		return Up;
	}
	else if (dx == 1) {
		return Down;
	}
	else if (dy == -1) {
		return Left;
	}
	else return Right;
}

void rotate(Direction & dir, Angle & a) {
	dir = (Direction)(((int)dir + (int)a + 4) % 4);
}

Direction left(Direction dir) {
	return (Direction)(((int)dir + 3) % 4);
}
Direction right(Direction dir) {
	return (Direction)(((int)dir + 1) % 4);
}

Angle angle(Direction d1, Direction d2) {
	int a = ((int)d2 - (int)d1 + 4) % 4;
	if (a == 3) a = -1;
	return (Angle)a;
}

struct Position {
	int x, y;
	Position (): x(0), y(0) {}
	Position (int ix, int iy): x(ix), y(iy) {}
	//Position (Position & p): x(p.x), y(p.y) {}
	bool good() {
		return x >= 0 && y >= 0 && x < h && y < w;
	}
	bool empty() {
		return Map[x][y] == None;
	}
	bool possibleToMove() {
		//cout << x << " " << y << ": " << (good() && (Map[x][y] == None || Map[x][y] == Food)) << endl;
		return good() && (Map[x][y] == None || Map[x][y] == Food);
	}
	bool operator != (Position p) {
		return x != p.x || y != p.y;
	}
};
/*bool operator == (Position p1, Position p2) {
	return p1.x == p2.x && p1.y == p2.y;
}
bool operator != (Position p1, Position p2) {
	return p1.x != p2.x || p1.y != p2.y;
}*/

Position pmove(Position pos, Direction dir) {
	switch (dir) {
	case (Up):
		pos.x--;
		break;
	case (Down):
		pos.x++;
		break;
	case (Left):
		pos.y--;
		break;
	case (Right):
		pos.y++;
		break;
	}
	return pos;
}

struct Element {
	Position pos;
	bool withFood;
	Element *prv, *nxt;
	Element (): withFood(0), prv(0), nxt(0) {
		pos = Position();
	}
	Element (Position p): pos(p), withFood(0), prv(0), nxt(0) {}

	void getFood() {
		withFood = 1;
	}
	bool giveFood() {
		bool ind = false;
		if (withFood) {
			if (nxt) nxt->getFood();
			else ind = true;
			withFood = 0;
		}
		return ind;
	}
};

bool go(Element *elem, Position pos) {
	elem->pos = pos;
	return pos.good();
}

struct Snake {
	Element *head;
	Element *tail;
	Direction dir;
	Object type;
	int size;

	Snake (Position p, Direction d) {
		head = new Element(p);
		tail = head;
		dir = d;
		type = MyO;
		size = 1;
	}

	void add(Position p) {
		Element *newElem = new Element(p);
		newElem->prv = tail;
		tail->nxt = newElem;
		tail = tail->nxt;
		size++;
	}

	Position move(Angle a) {
		Map[tail->pos.x][tail->pos.y] = None;
		rotate(dir, a);
		Position p(tail->pos);
		go(tail, pmove(head->pos, dir));
		tail->nxt = head;
		head->prv = tail;
		head = tail;
		tail = tail->prv;
		tail->nxt = 0;
		head->prv = 0;
		if (head->withFood) {
			add(p);
			Map[p.x][p.y] = type;
		}
		head->withFood = (bool)Map[head->pos.x][head->pos.y];
		Map[head->pos.x][head->pos.y] = type;
		return p;
	}

	Position move(Direction newDir) {
		Map[tail->pos.x][tail->pos.y] = None;
		dir = newDir;
		Position p(tail->pos);
		go(tail, pmove(head->pos, dir));
		tail->nxt = head;
		head->prv = tail;
		head = tail;
		tail = tail->prv;
		tail->nxt = 0;
		head->prv = 0;
		if (head->withFood) {
			add(p);
			Map[p.x][p.y] = type;
		}
		head->withFood = (bool)Map[head->pos.x][head->pos.y];
		Map[head->pos.x][head->pos.y] = type;
		return p;
	}

	void moveBack(Position p) {
		Map[head->pos.x][head->pos.y] = None;
		if (head->withFood) {
			Map[head->pos.x][head->pos.y] = Food;
			head->withFood = 0;
		}
		if (Map[p.x][p.y] == type) {
			tail = tail->prv;
			delete tail->nxt;
			tail->nxt = 0;
			head->withFood = 1;
		}
		Map[p.x][p.y] = type;
		head = head->nxt;
		go(head->prv, p);
		tail->nxt = head->prv;
		tail->nxt->prv = tail;
		tail = tail->nxt;
		head->prv = 0;
		tail->nxt = 0;
		dir = directionByVector(head->pos.x - head->nxt->pos.x, head->pos.y - head->nxt->pos.y);
	}
};

Snake *Me, *Enemy;

void initSnakes(int turn) {
	Snake *s1, *s2;

	s1 = new Snake(Position(3, 0), Down);
	s1->add(Position(2, 0));
	s1->add(Position(1, 0));
	s1->add(Position(0, 0));

	s2 = new Snake(Position(h - 4, w - 1), Up);
	s2->add(Position(h - 3, w - 1));
	s2->add(Position(h - 2, w - 1));
	s2->add(Position(h - 1, w - 1));

	if (turn == 2) swap(s1, s2);
	Me = s1;
	Enemy = s2;
	Me->type = MyO;
	Enemy->type = EnemyO;
}

void initMap() {
#ifdef DEBUG
	ifstream mapFile("map.txt");
	string s;
	for (int x = 0; x < h; ++x) {
		mapFile >> s;
		for (int y = 0; y < w; ++y) {
			Map[x][y] = (Object)(s[y] - '0');
		}
	}
	mapFile.close();
#else
	string s;
	for (int x = 0; x < h; ++x) {
		cin >> s;
		for (int y = 0; y < w; ++y) {
			Map[x][y] = (Object)(s[y] - '0');
		}
	}
#endif
	Element *snake;
	for (snake = Me->head; snake; snake = snake->nxt) Map[snake->pos.x][snake->pos.y] = MyO;
	for (snake = Enemy->head; snake; snake = snake->nxt) Map[snake->pos.x][snake->pos.y] = EnemyO;
}

void showMap() {
#ifndef DEBUG
	return;
#endif
	for (int x = 0; x < h; ++x) {
		for (int y = 0; y < w; ++y) {
			switch (Map[x][y]) {
			case (None):
				cout << '.';
				break;
			case (Food):
				cout << '#';
				break;
			case (MyO):
				if (Me->head->pos.x == x && Me->head->pos.y == y) cout << 'M';
				else cout << 'm';
				break;
			case (EnemyO):
				if (Enemy->head->pos.x == x && Enemy->head->pos.y == y) cout << 'E';
				else cout << 'e';
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}

int dist[h][w];
Direction dirs[h][w];
Position firstFood(Position start, int maxDist = inf) {
	int x, y, k;
	for (x = 0; x < h; ++x) {
		for (y = 0; y < w; ++y) {
			dist[x][y] = -inf;
		}
	}

	Element *elem;
	elem = Me->tail;
	for (k = 1; elem; elem = elem->prv) {
		dist[elem->pos.x][elem->pos.y] = -k;
		k++;
	}
	elem = Enemy->tail;
	for (k = 1; elem; elem = elem->prv) {
		dist[elem->pos.x][elem->pos.y] = -k;
		k++;
	}
	dist[start.x][start.y] = 0;

	Position cur, nxt;
	Direction dir;
	queue <Position> Q;
	Q.push(start);
	while (!Q.empty()) {
		cur = Q.front();
		//cout << "cur: " << cur.x << " " << cur.y << endl;
		Q.pop();
		for (k = 0; k < 4; ++k) {
			dir = (Direction)k;
			nxt = pmove(cur, dir);
			if (nxt.good() && dist[nxt.x][nxt.y] < 0 && (dist[nxt.x][nxt.y] == -inf || dist[nxt.x][nxt.y] + dist[cur.x][cur.y] + 1 >= 0)) {
				//cout << "change: " << nxt.x << " " << nxt.y << endl;
				dist[nxt.x][nxt.y] = dist[cur.x][cur.y] + 1;
				dirs[nxt.x][nxt.y] = (Direction)(k ^ 2);
				if (Map[nxt.x][nxt.y] == Food) return nxt;
				if (dist[nxt.x][nxt.y] < maxDist)
					Q.push(nxt);
			}
		}
	}
	return Position(-1, -1);
}

void fullBfs(Position start, int maxDist = inf) {
	int x, y, k;
	for (x = 0; x < h; ++x) {
		for (y = 0; y < w; ++y) {
			dist[x][y] = -inf;
		}
	}

	Element *elem;
	elem = Me->tail;
	for (k = 1; elem; elem = elem->prv) {
		dist[elem->pos.x][elem->pos.y] = -k;
		k++;
	}
	elem = Enemy->tail;
	for (k = 1; elem; elem = elem->prv) {
		dist[elem->pos.x][elem->pos.y] = -k;
		k++;
	}
	dist[start.x][start.y] = 0;

	Position cur, nxt;
	Direction dir;
	queue <Position> Q;
	Q.push(start);
	while (!Q.empty()) {
		cur = Q.front();
		//cout << "cur: " << cur.x << " " << cur.y << endl;
		Q.pop();
		for (k = 0; k < 4; ++k) {
			dir = (Direction)k;
			nxt = pmove(cur, dir);
			if (nxt.good() && dist[nxt.x][nxt.y] < 0 && (dist[nxt.x][nxt.y] == -inf || dist[nxt.x][nxt.y] + dist[cur.x][cur.y] + 1 >= 0)) {
				//cout << "change: " << nxt.x << " " << nxt.y << endl;
				dist[nxt.x][nxt.y] = dist[cur.x][cur.y] + 1;
				dirs[nxt.x][nxt.y] = (Direction)(k ^ 2);
				//if (Map[nxt.x][nxt.y] == Food) return nxt;
				if (dist[nxt.x][nxt.y] < maxDist)
					Q.push(nxt);
			}
		}
	}
	//return Position(-1, -1);
}

int countPossible(Position start) {
	fullBfs(start);
	int cnt = 0;
	for (int x = 0; x < h; ++x) {
		for (int y = 0; y < w; ++y) {
			if (dist[x][y] >= 0) cnt ++;
		}
	}
	return cnt;
}

Direction getDirection(Position start, Position target) {
	Position cur = target;
	Direction dir = dirs[cur.x][cur.y];
	while (pmove(cur, dir) != start) {
		cur = pmove(cur, dir);
		dir = dirs[cur.x][cur.y];
	}
	//cout << "getDirection: " << (((int)dir) ^ 2) << endl;
	return (Direction)((int)dir ^ 2);
}

Angle greedy(Snake *s) {
	Position food = firstFood(s->head->pos);
	if (food.good())
		return angle(s->dir, getDirection(s->head->pos, food));
	else {
		Position pos;
		Direction dir, best_dir;
		int cnt, best_cnt = -1;
		for (int k = 0; k < 4; ++k) {
			dir = (Direction)k;
			if (pmove(s->head->pos, dir).possibleToMove()) {
				pos = s->move(dir);
				cnt = countPossible(s->head->pos);
				s->moveBack(pos);
			}
			else cnt = -2;
			if (cnt >= best_cnt) {
				best_dir = dir;
				best_cnt = cnt;
			}
		}
		return angle(s->dir, best_dir);
	}
}

double profit(Snake *s) {
	Position food = firstFood(s->head->pos, 20);
	if (!food.good()) return -inf;
	return ((s->head->withFood)?0:dist[food.x][food.y]);
}

double function() {
	return -profit(Me);
	//return -profit(Me) + profit(Enemy);
}

//int debugCNT = 0;

Direction minimax(int &total, int depth = MAX_DEPTH, int turn = 1) { // 1 - Me (max), 2 - Enemy (min)
	//debugCNT ++;

	//for (int i = 0; i < debugCNT; ++i) cout << "  ";
	//cout << "MINIMAX (" << total << " " << depth << " " << ((turn==1)?"max":"min") << ")" << endl;

	if (depth <= 0) {
		//for (int i = 0; i < debugCNT; ++i) cout << "  ";
		//cout << "END: " << total << endl;
		//debugCNT --;
		return Up;
	}
	int tmp, functionValue = function();
	Direction best_dir = Up, dir;
	Position pos;
	for (int k = 0; k < 4; ++k) {
		dir = (Direction)k;
		tmp = functionValue;
		if (turn == 1) {
			if (!pmove(Me->head->pos, dir).possibleToMove()) continue;
			pos = Me->move(dir);
		}
		else {
			if (!pmove(Enemy->head->pos, dir).possibleToMove()) continue;
			pos = Enemy->move(dir);
		}

		minimax(tmp, depth - 1, turn ^ 3);

		if (turn == 1) {
			if (total < tmp) {
				total = tmp;
				best_dir = dir;
			}
			Me->moveBack(pos);
		}
		else {
			if (total > tmp) {
				total = tmp;
				best_dir = dir;
			}
			Enemy->moveBack(pos);
		}
	}

	//for (int i = 0; i < debugCNT; ++i) cout << "  ";
	//cout << "END: " << total << " " << (int)best_dir << endl;

	//debugCNT --;
	return best_dir;
}

char angleTurn;

void enemyTurn() {
#ifdef DEBUG
	cout << "Enemy turn: ";
	cin >> angleTurn;
	cout << endl;
#else
	cin >> angleTurn;
#endif
	Enemy->move(ctoa(angleTurn));
}

void myTurn() {
	/*cout << "My turn: ";
	cin >> angleTurn;
	cout << endl;
	if (angleTurn == 'B') {
		int x, y;
		cin >> x >> y;
		Me->moveBack(Position(x, y));
		return;
	}
	Me->move(ctoa(angleTurn));
	Position food = firstFood(Me->head->pos);
	if (!food.good()) {
		//ololo
	}
	Direction dir = getDirection(Me->head->pos, food);*/
	int tmp = -inf;
	Direction dir = minimax(tmp);
	Angle a;
	if (!pmove(Me->head->pos, dir).possibleToMove() || tmp == -inf) a = greedy(Me);	
	else a = angle(Me->dir, dir);
	Me->move(a);
	cout << string(1, atoc(a)) << endl;
}

int main() {
	int turn;
#ifdef DEBUG
	cout << "Enter turn: ";
	cin >> turn;
	cout << endl;
#else
	cin >> turn;
#endif
	initSnakes(turn);
	initMap();

	//showMap();
	while (true) {
		if (turn == 1) myTurn();
		else enemyTurn();
		//showMap();
		turn ^= 3;
	}
	return 0;
}