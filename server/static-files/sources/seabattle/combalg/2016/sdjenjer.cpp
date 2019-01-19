// SeaBattle_Master.cpp: определяет точку входа для консольного приложения.
//
// SeaBattle.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
//#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;

#define fin cin
#define type(x, y) static_cast<t>(x)

/*/ifstream fin("firststep.in");//*/

const string NO_MOVE = "0X ";
const string CONVERT = "ABCDEFGHIJ";
const string DIRECTION = "EWNS";
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

struct Ship
{
	int secs, x1, y1, x2, y2, dx, dy, hurted;
	vector <bool> block;
	bool is_hurt, dead;

	Ship(int _secs = 4, int _x1 = 0, int _y1 = 0, int _x2 = 0, int _y2 = 0):
	secs(_secs), x1(min(_x1, _x2)), y1(min(_y1, _y2)), x2(max(_x1, _x2)), y2(max(_y1, _y2)), hurted(0), is_hurt(false), dead(false)
	{
		block.clear();
		block.resize(_secs, true);
		dx = (_x1 == _x2 ? 0 : 1);
		dy = (_y1 == _y2 ? 0 : 1);
	}

	bool operator ==(Ship sh)
	{
		return x1 == sh.x1 && y1 == sh.y1 && x2 == sh.x2 && y2 == sh.y2;
	}
};

const int LEN = 10;
const int N = 10;

vector <Ship> myship, eneship;
vector <vector <bool> > my_mine, enemine;
vector <int> shufships, shuf;
bool player;
int p, alive = N;
stack <pair <int, int> > fire, potent, randpot;

/*
MY_MINE ON MY FLOOR, ENEMINE --- WHERE I SHOT
*/

bool have(Ship sh, int x, int y)
{
	return (sh.x1 <= x && x <= sh.x2 && sh.y1 <= y && y <= sh.y2);
}

int numer_ship(int x, int y)
{
	for (int i = 0; i < N; i++)
	{
		if (have(myship[i], x, y))
			return i;
	}
	return -1;
}

int numer_block(Ship sh, int x, int y)
{
	return x - sh.x1 + y - sh.y1;
}

bool there_is_myship(int x, int y)
{
	return (numer_ship(x, y) != -1);
}

bool there_is_myblock(int x, int y)
{
	if (!there_is_myship(x, y))
		return false;
	return myship[numer_ship(x, y)].block[numer_block(myship[numer_ship(x, y)], x, y)];
}

void attack(int sh, int x, int y)
{
	int bl = numer_block(myship[sh], x, y);
	myship[sh].block[bl] = false;
	myship[sh].is_hurt = true;
	myship[sh].hurted++;
	if (myship[sh].hurted == myship[sh].secs)
	{
		myship[sh].dead = true;
		alive--;
	}
}

bool point_in_bound(int x)
{
	return 0 <= x && x < LEN;
}

bool in_bound(int x, int y)
{
	return point_in_bound(x) && point_in_bound(y);
}

bool in_bound(int x1, int y1, int x2, int y2)
{
	return point_in_bound(x1) && point_in_bound(y1) && point_in_bound(x2) && point_in_bound(y2);
}

bool on_mine(Ship sh)
{
	for (int i = 0; i < sh.secs; i++)
	{
		if (my_mine[sh.x1 + i * sh.dx][sh.y1 + i * sh.dy])
			return true;
	}
	return false;
}

Ship sail_on(Ship sh, int dx, int dy)
{
	Ship boat = sh;
	boat.x1 += dx;
	boat.x2 += dx;
	boat.y1 += dy;
	boat.y2 += dy;
	return boat;
}

bool neighbour(int x1, int y1, int x2, int y2)
{
	return (abs(x1 - x2) + abs(y1 - y2)) <= 1;
}

bool neighbour_corner(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) == 1 && abs(y1 - y2) == 1;
}

bool neighbour(Ship sh1, Ship sh2)
{
	for (int i = 0; i < sh1.secs; i++)
	{
		for (int j = 0; j < sh2.secs; j++)
		{
			if (neighbour(sh1.x1 + i * sh1.dx, sh1.y1 + i * sh1.dy, sh2.x1 + j * sh2.dx, sh2.y1 + j * sh2.dy) ||
				neighbour_corner(sh1.x1 + i * sh1.dx, sh1.y1 + i * sh1.dy, sh2.x1 + j * sh2.dx, sh2.y1 + j * sh2.dy))
				return true;
		}
	}
	return false;
}

bool exist(Ship sh, int dx, int dy)
{
	Ship boat = sail_on(sh, dx, dy);
	if (!in_bound(boat.x1, boat.y1, boat.x2, boat.y2))
		return false;
	if (on_mine(boat))
		return false;
	for (int i = 0; i < N; i++)
	{
		if (myship[i] == sh)
			continue;
		if (neighbour(myship[i], boat))
			return false;
	}
	return true;
}

void shuffle_by2(vector <int> &v)
{
	if (rand() % 2 == 1)
		swap(v[0], v[1]);
	if (rand() % 2 == 1)
		swap(v[2], v[3]);
}

void perm_by2(vector <int> &v)
{
	swap(v[0], v[2]);
	swap(v[1], v[3]);
}

void shuffle(vector <int> &v)
{
	for (int i = v.size() - 1; i > 0; i--)
		swap(v[i], v[rand() % i]);
}

int classify(int x, int y)
{
	for (int i = 0; i < eneship.size(); i++)
	{
		if (have(eneship[i], x, y))
			return i;
		if (neighbour(eneship[i].x1, eneship[i].y1, x, y))
		{
			eneship[i] = Ship(eneship[i].secs + 1, x, y, eneship[i].x2, eneship[i].y2);
			return i;
		}
		if (neighbour(eneship[i].x2, eneship[i].y2, x, y))
		{
			eneship[i] = Ship(eneship[i].secs + 1, x, y, eneship[i].x1, eneship[i].y1);
			return i;
		}
	}
	eneship.push_back(Ship(1, x, y, x, y));
	return eneship.size() - 1;
}

int classify_not_create(int x, int y)
{
	for (int i = 0; i < eneship.size(); i++)
	{
		if (neighbour(eneship[i].x1, eneship[i].y1, x, y) || neighbour(eneship[i].x2, eneship[i].y2, x, y) || have(eneship[i], x, y))
			return i;
	}
	return -1;
}

void convex1(int x, int y)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (in_bound(x + i, y + j))
			{
				enemine[x + i][y + j] = true;
			}
		}
	}
}

void convex(Ship sh)
{
	for (int i = 0; i < sh.secs; i++)
	{
		convex1(sh.x1 + i * sh.dx, sh.y1 + i * sh.dy);
	}
}

bool exist_any(Ship sh)
{
	for (int i = 0; i < 4; i++)
	{
		if (exist(sh, dx[i], dy[i]))
			return true;
	}
	return false;
}

int position(Ship sh)
{
	int res = 0;
	for (int i = 0; i < 4; i++)
	{
		if (exist(sh, dx[i], dy[i]))
			res++;
	}
	return res;
}

void print_field()
{
	if (player)
		cerr << "Player 1, alive = ";
	else
		cerr << "Player 2, alive = ";
	cerr << alive << '\n';
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			if (there_is_myship(i, j))
			{
				if (there_is_myblock(i, j))
					cerr << numer_ship(i, j) << ' ';
				else
					cerr << "@ ";
			}
			else
			{
				if (my_mine[i][j])
					cerr << "* ";
				else
					cerr << "- ";
			}
		}
		cerr << '\n';
	}
	cerr << "\n------------------------------------------------\n";
}

int position()
{
	int res = 0;
	for (int i = 0; i < N; i++)
	{
		if (!myship[i].is_hurt)
			res += position(myship[i]);
	}
	return res;
}

int create_direction(int ind, bool must_go)
{
	Ship begship = myship[ind], reship = myship[ind];
	int cur = (must_go ? -1 : position()), res = 4;
	for (int p = 0; p < 4; p++)
	{
		if (exist(begship, dx[shuf[p]], dy[shuf[p]]))
		{
			myship[ind] = sail_on(begship, dx[shuf[p]], dy[shuf[p]]);
			int pos = position();
			if (pos > cur)
			{
				reship = myship[ind];
				cur = pos;
				res = p;
			}
		}
	}
	myship[ind] = reship;
	return res;
}

int best_move(int ind)
{
	Ship begship = myship[ind];
	int cur = position();
	for (int p = 0; p < 4; p++)
	{
		if (exist(begship, dx[shuf[p]], dy[shuf[p]]))
		{
			myship[ind] = sail_on(begship, dx[shuf[p]], dy[shuf[p]]);
			cur = max(cur, position());
		}
	}
	myship[ind] = begship;
	return cur;
}

void random_move()
{
	shuffle(shufships);
	if (rand() % 2)
		perm_by2(shuf);
	shuffle_by2(shuf);
	int ind = -1, cur = position();
	for (int i = 0; i < N; i++)
	{
		if (myship[shufships[i]].is_hurt)
			continue;
		int pos = best_move(shufships[i]);
		if (pos > cur)
		{
			cur = pos;
			ind = shufships[i];
		}
	}
	if (ind == -1)
		cout << NO_MOVE;
	else
	{
		int p = create_direction(ind, false);
		if (p == 4)
			cout << NO_MOVE;
		else
			cout << ind << DIRECTION[shuf[p]] << ' ';
	}
}

bool comp(int a, int b)
{
	return position(myship[a]) < position(myship[b]);
}

void shot_from_ship(int i)
{
	if (myship[i].block[0])
	{
		cout << CONVERT[myship[i].x1] << myship[i].y1;
	}
	else
	{
		int j = myship[i].secs - 1;
		if (myship[i].block[j])
		{
			cout << CONVERT[myship[i].x1 + j * myship[i].dx] << myship[i].y1 + j * myship[i].dy;
		}
		else
		{
			for (int j = 1; j < myship[i].secs - 1; j++)
			{
				if (!myship[i].block[j])
					continue;
				cout << CONVERT[myship[i].x1 + j * myship[i].dx] << myship[i].y1 + j * myship[i].dy;
				break;
			}
		}
	}
}

void shot_from_ship()
{
	sort(shufships.rbegin(), shufships.rend(), comp);
	for (int i = 0; i < N; i++)
	{
		if (myship[shufships[i]].is_hurt)
			continue;
		shot_from_ship(shufships[i]);
		return;
	}
	shuffle(shufships);
	for (int i = 0; i < N; i++)
	{
		if (myship[shufships[i]].dead)
			continue;
		shot_from_ship(shufships[i]);
		return;
	}
}

int main()
{
	myship.clear();
	eneship.clear();
	while (!fire.empty())
		fire.pop();
	randpot = fire;
	potent = fire;

	shufships.clear();
	shufships.resize(N);
	for (int i = 0; i < N; i++)
	{
		shufships[i] = i;
	}

	randpot.push(make_pair(7, 7));
	randpot.push(make_pair(2, 2));
	randpot.push(make_pair(7, 2));
	randpot.push(make_pair(2, 7));
	randpot.push(make_pair(6, 3));
	randpot.push(make_pair(3, 6));
	randpot.push(make_pair(3, 3));
	randpot.push(make_pair(6, 6));
	randpot.push(make_pair(5, 5));
	randpot.push(make_pair(4, 4));

	for (int i = 0; i < N; i++)
	{
		string s;
		fin >> s;
		int palub;
		if (i < 4)
			palub = 1;
		else if (i < 7)
			palub = 2;
		else if (i < 9)
			palub = 3;
		else
			palub = 4;
		myship.push_back(Ship(palub, s[0] - 'A', s[1] - '0', s[2] - 'A', s[3] - '0'));
	}

	my_mine.clear();
	my_mine.resize(N);
	for (int i = 0; i < N; i++)
	{
		my_mine[i].resize(N, false);
	}

	enemine = my_mine;

	fin >> p;

	player = (p == 0);

	srand(26345652);

	string hit = "H";

	if (player)
		cout << NO_MOVE;
	else
		cin >> hit;
	int xfire, yfire;
	shuf.clear();
	shuf.resize(4);
	for (int i = 0; i < 4; i++)
		shuf[i] = i;
	
	while (alive > 0)
	{
		while (player)
		{
			print_field();
			shot_from_ship();
			
			while (!potent.empty() && enemine[potent.top().first][potent.top().second])
				potent.pop();
			if (potent.empty())
			{
				while (!fire.empty() && enemine[fire.top().first][fire.top().second])
					fire.pop();
				if (fire.empty())
				{
					while (!randpot.empty() && enemine[randpot.top().first][randpot.top().second])
						randpot.pop();
					if (randpot.empty())
					{
						do
						{
							xfire = rand() % LEN;
							yfire = rand() % LEN;
						} while (enemine[xfire][yfire]);
					}
					else
					{
						xfire = randpot.top().first;
						yfire = randpot.top().second;
						randpot.pop();
					}
				}
				else
				{
					xfire = fire.top().first;
					yfire = fire.top().second;
					fire.pop();
				}
			}
			else
			{
				xfire = potent.top().first;
				yfire = potent.top().second;
				potent.pop();
			}
			cout << CONVERT[xfire] << yfire << '\n';
			enemine[xfire][yfire] = true;
				
			cin >> hit;
			if (hit.length() == 1)
			{
				random_move();
				int x = classify(xfire, yfire);
				if (hit == "K")
				{
					convex(eneship[x]);
				}
				else
				{
					if (eneship[x].secs == 1)
					{
						for (int p = 0; p < 4; p++)
						{
							if (in_bound(xfire + dx[p], yfire + dy[p]) && !enemine[xfire + dx[p]][yfire + dy[p]])
								potent.push(make_pair(xfire + dx[p], yfire + dy[p]));
						}
					}
					else
					{
						if (in_bound(eneship[x].x2 + eneship[x].dx, eneship[x].y2 + eneship[x].dy))
							potent.push(make_pair(eneship[x].x2 + eneship[x].dx, eneship[x].y2 + eneship[x].dy));
						if (in_bound(eneship[x].x1 - eneship[x].dx, eneship[x].y1 - eneship[x].dy))
							potent.push(make_pair(eneship[x].x1 - eneship[x].dx, eneship[x].y1 - eneship[x].dy));
					}
				}
				continue;
			}
			player = !player;
		}
		while (!player)
		{
			print_field();
			int x = hit[0] - 'A';
			int y = hit[1] - '0';
			int w = classify_not_create(x, y);
			if (w == -1)
				fire.push(make_pair(x, y));
			else
				potent.push(make_pair(x, y));
			x = hit[2] - 'A';
			y = hit[3] - '0';
			if (there_is_myblock(x, y))
			{
				int sh = numer_ship(x, y);

				if (myship[sh].dx == 1 && (shuf[0] == 0 || shuf[0] == 1))
					perm_by2(shuf);
				shuffle_by2(shuf);
				
				int p = 0;
				if (myship[sh].is_hurt)
					p = 4;
				else
				{
					my_mine[x][y] = true;
					p = create_direction(sh, true);
					if (p == 4)
					{
						my_mine[x][y] = false;
						p = create_direction(sh, false); //true
					}
				}

				if (p == 4)
				{
					attack(sh, x, y);
					my_mine[x][y] = true;

					random_move(); //						DONT FORGET
					//cout << NO_MOVE;

					if (myship[sh].dead)
						cout << "K\n";
					else
						cout << "H\n";
					cin >> hit;
					player = !player;
				}
				else
				{
					cout << sh << DIRECTION[shuf[p]] << ' ';
					if (there_is_myblock(x, y))
					{
						attack(sh, x, y);
						if (myship[sh].dead)
							cout << "K\n";
						else
							cout << "H\n";
						cin >> hit;
						player = !player;
					}
				}
			}
			else
			{
				my_mine[x][y] = true;
				cerr << "Am not shot\n";
				random_move();
			}
			my_mine[x][y] = true;
			player = !player;
		}
	}
	
	return 0;
}