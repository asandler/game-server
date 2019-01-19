// Reversy.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
//#include <vector>

using namespace std;

const int Size = 64;
const int Side = 8;
const string s = "abcdefgh";
const int CostCorn = 15;//10;//8;
const int CostSide = 7;//6;//5;//3;
const int INF = 1e+5;
const int Depth = 4;//3;

const int Rnd[] = {387652636, 67894236, 9984652, 788653532};

int player, cnt = 0;
unsigned long long mine = 0, its = 0;
//vector <vector <bool> > mine, its;

bool bit(unsigned long long x, int i)
{
	return ((x & (1ll << i)) >> i);
}

void make_bit(unsigned long long &x, int i)
{
	//x |= (1ll << i);
	x = (x | (1ll << i));
}

void del_bit(unsigned long long &x, int i)
{
	//x &= (~(1ll << i));
	x = (x & (~(1ll << i)));
}

bool corner(int x, int y)
{
	return ((x == 0 || x == 7) && (y == 0 || y == 7));
}

bool side(int x, int y)
{
	return (x == 0 || x == 7 || y == 0 || y == 7);
}

int fun(unsigned long long my, unsigned long long yo)
{
	int res = 0;
	
	for (int x = 0; x < Side; x++)
	{
		for (int y = 0; y < Side; y++)
		{
			int i = x + y * 8;
			res += bit(my, i) * (corner(x, y) ? CostCorn : (side(x, y) ? CostSide : 1));
			res -= bit(yo, i) * (corner(x, y) ? CostCorn : (side(x, y) ? CostSide : 1));
		}
	}
	
	return res;
}

bool in_bounds(int x, int y)
{
	return (x >= 0 && x < Side && y >= 0 && y < Side);
}

void plusxy(int &x, int &y, int dx, int dy)
{
	x += dx;
	y += dy;
}

void minusxy(int &x, int &y, int dx, int dy)
{
	x -= dx;
	y -= dy;
}

bool reverse(unsigned long long &my, unsigned long long &yo, int i)
{
	bool ok = false;
	make_bit(my, i);
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (dx == 0 && dy == 0)
				continue;
			int y = i / 8 + dy;
			int x = i % 8 + dx;
			if (!in_bounds(x, y))
				continue;
			bool gap = false;
			for (; in_bounds(x, y); plusxy(x, y, dx, dy))
			{
				int pos = x + y * 8;
				if (bit(yo, pos))
				{
					gap = true;
					continue;
				}
				if (bit(my, pos) && gap)
				{
					ok = true;
					minusxy(x, y, dx, dy);
					for (; x + y * 8 != i; minusxy(x, y, dx, dy))
					{
						pos = x + y * 8;
						make_bit(my, pos);
						del_bit(yo, pos);
					}
					break;
				}
				else
					break;
			}
		}
	}
	return ok;
}

int minimax(unsigned long long my, unsigned long long yo, int depth, int color)
{
	if (((my | yo) == (1ll << Size) - 1) || (depth == 0))
		return fun(my, yo);
	//int ans = INF;//-INF * color;
	int ans = INF * color;
	for (int i = 0; i < Size; i++)
	{
		if (bit(my, i) || bit(yo, i))
			continue;
		unsigned long long nxt = my;
		unsigned long long oth = yo;

		if (!reverse(nxt, oth, i))
			continue;

		if (color == -1)
			ans = max(ans, minimax(nxt, oth, depth - 1, -color));
		else
			ans = min(ans, minimax(nxt, oth, depth - 1, -color));
		//ans = min(ans, minimax(oth, nxt, depth - 1, -color));

	}
	return ans;
}

void move()
{
	int mx = -INF, ind = -1;
	for (int i = 0; i < Size; i++)
	{
		if (bit(mine, i) || bit(its, i))
			continue;
		unsigned long long nxt_me = mine;
		unsigned long long other = its;
		if (!reverse(nxt_me, other, i))
			continue;
		int nxt = minimax(other, nxt_me, Depth, -1);
		if (mx < nxt || mx == nxt && rand() % 2 == 1)
		{
			mx = nxt;
			ind = i;
		}
	}
	if (ind == -1)
	{
		cout << "Skip\n";
		return;
	}
	reverse(mine, its, ind);
	cout << s[ind / 8] << (ind % 8) + 1 << '\n';
}

int main()
{
	srand(Rnd[rand() % 4]);
	
	cin >> player;

	//mine = 5760090436131618815;
	//its  = 2310360096116310016;

	//mine = 3399988157142350624;
	//its  = 860943697032;

	//move();

	if (player == 1)
	{
		make_bit(its, 27);
		make_bit(mine, 28);
		make_bit(its, 36);
		make_bit(mine, 35);
		move();
	}
	else
	{
		make_bit(mine, 27);
		make_bit(its, 28);
		make_bit(mine, 36);
		make_bit(its, 35);
	}

	while ((mine | its) != (1ll << Size) - 1)
	{
		int pos, m;
		string s;
		//char c;
		cin >> s;
		if (s != "Skip")
		{
			//cin >> m;
			pos = 8 * (s[0] - 'a') + (s[1] - '1');
			reverse(its, mine, pos);
		}
			
		/*if ((mine | its) == (1ll << Size) - 1)
		{
			cout << "Skip\n";
			break;
		}*/
		move();
	}
	
	return 0;
}

/*
int fun(vector <vector <bool> > mine, vector <vector <bool> > its)
{
	int res = 0;
	
	for (int x = 0; x < Side; x++)
	{
		for (int y = 0; y < Side; y++)
		{
			res += mine[x][y] * (corner(x, y) ? CostCorn : (side(x, y) ? CostSide : 1));
			res -= its[x][y] * (corner(x, y) ? CostCorn : (side(x, y) ? CostSide : 1));
		}
	}
	
	return res;
}

bool reverse(vector <vector <bool> > &my, vector <vector <bool> > &yo, int _x, int _y)
{
	bool ok = false;
	my[_x][_y] = true;
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (dx == 0 && dy == 0)
				continue;
			int y = _y + dy;
			int x = _x + dx;
			if (!in_bounds(x, y))
				continue;
			bool gap = false;
			for (; in_bounds(x, y); plusxy(x, y, dx, dy))
			{
				if (yo[x][y])
				{
					gap = true;
					continue;
				}
				if (my[x][y] && gap)
				{
					ok = true;
					x -= dx;
					y -= dy;
					for (; x != _x || y != _y; minusxy(x, y, dx, dy))
					{
						my[x][y] = true;
						yo[x][y] = false;
					}
					break;
				}
				else
					break;
			}
		}
	}
	return ok;
}

int minimax(vector <vector <bool> > mine, vector <vector <bool> > its, int depth, int color, int n)
{
	int x = fun(mine, its);
	if (n == Size || depth == 0)
		return x;
	int ans = -1000 * color;
	for (int x = 0; x < Side; x++)
	{
		for (int y = 0; y < Side; y++)
		{		
			if (mine[x][y] || its[x][y])
				continue;
			vector <vector <bool> > nxt = mine;
			vector <vector <bool> > oth = its;
			if (!reverse(nxt, oth, x, y))
				continue;
	
			if (color == 1)
				ans = max(ans, minimax(oth, nxt, depth - 1, -color, n + 1));
			else
				ans = min(ans, minimax(oth, nxt, depth - 1, -color, n + 1));
		}
	}
	return ans;
}

void move()
{
	int mx = -1000, _x = -1, _y = -1;
	for (int x = 0; x < Side; x++)
	{
		for (int y = 0; y < Side; y++)
		{		
			if (mine[x][y] || its[x][y])
				continue;
			vector <vector <bool> > nxt = mine;
			vector <vector <bool> > oth = its;
			if (!reverse(nxt, oth, x, y))
				continue;
	
			int res = minimax(oth, nxt, 3, -1, cnt + 1);
			if (mx < res && abs(res) != 100)
			{
				mx = res;
				_x = x;
				_y = y;
			}
		}
	}
	if (_x == -1)
	{
		cout << "Skip\n";
		return;
	}
	mine[_x][_y] = true;
	cnt++;
	cout << s[_x] << _y + 1 << '\n';
}

int main()
{
	cin >> player;

	mine.resize(Side);

	for (int i = 0; i < Side; i++)
	{
		mine[i].resize(Side);
	}

	its = mine;

	if (player == 1)
	{
		its[3][3] = true;
		mine[3][4] = true;
		mine[4][3] = true;
		its[4][4] = true;
		move();
	}
	else
	{
		mine[3][3] = true;
		its[3][4] = true;
		its[4][3] = true;
		mine[4][4] = true;
	}

	while (true)
	{
		int pos, m;
		char c;
		cin >> c;
		if (c != 'S')
		{
			cin >> m;
			reverse(its, mine, c - 'a', m - 1);
			cnt++;
		}
		move();
	}
	
	return 0;
}*/