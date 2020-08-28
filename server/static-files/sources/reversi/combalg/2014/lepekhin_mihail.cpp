#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

int inf = 1000000, my_pl;
char f[10][10];

struct res
{
	int a, b, c;
	res(int d, int e, int f) : a(d), b(e), c(f) {};
};

struct TryMove
{
	int all;
	int count[10];
};

char inv(char c)
{
	if (c == '1') return '2';
	if (c == '2') return '1';
    return '9';
}

int px[] = {1, 0, -1, 0, 1, -1, -1, 1}, py[] = {0, 1, 0, -1, 1, -1, 1, -1};

TryMove Check(int y, int x, char c)
{
	bool right = 0;
	TryMove res;
	res.all = 0;
	for (int i = 0; i < 8; i++)
	{
		res.count[i] = 0;
		int cury = y + py[i], curx = x + px[i];
		int k = 0;
		while (f[cury][curx] == inv(c))
			k++, cury += py[i], curx += px[i];
		if (f[cury][curx] == c)
			res.count[i] = k, res.all += k;
	}
	return res;
}

void Draw(TryMove t, int y, int x, char c)
{
	bool right = 0;
	for (int i = 0; i < 8; i++)
	{
		int cury = y + py[i], curx = x + px[i];
		if (t.count[i] > 0)
			right = 1;
		for (int j = 0; j < t.count[i]; j++)
			f[cury][curx] = c, cury += py[i], curx += px[i];
	}
	if (right)
		f[y][x] = c;
}

void Clear(TryMove t, int y, int x, char c)
{
	bool right = 0;
	for (int i = 0; i < 8; i++)
	{
		int cury = y + py[i], curx = x + px[i];
		if (t.count[i] > 0)
			right = 1;
		for (int j = 0; j < t.count[i]; j++)
			f[cury][curx] = inv(c), cury += py[i], curx += px[i];
	}
	if (right)
		f[y][x] = '0';
}

void Print_field()
{
	for (int i = 1; i <= 8; i++)
	{
		printf ("%c ", char('a' + i - 1));
		for (int j = 1; j <= 8; j++)
			if (f[i][j] != '0')
				printf ("%d ", (f[i][j] - '0') % 2);
			else
				printf ("  ");
		printf ("\n");
	}
	printf ("  ");
	for (int i = 1; i <= 8; i++)
		printf ("%d ",  i);
	printf ("\n");
	printf ("\n");
}

int Mark()
{
	int white = 0, black = 0;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			if (f[i][j] == '1')
				black++;
			else
				white++;
	return black - white;
}

int MinMax(int pl)
{
	if (pl == 4 + my_pl)
		return Mark();
	int optimal = -inf, optimaly = 0, optimalx = 0;
	if (pl % 2 == 0)
		optimal = -optimal;
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
		{
			TryMove t = Check(i, j, 2 - pl % 2 + '0');
			if (f[i][j] == '0' && t.all)
			{
				Draw(t, i, j, 2 - pl % 2 + '0');
				int curd = MinMax(pl + 1);
				if (curd < optimal && pl % 2 == 0)
					optimal = curd, optimaly = i, optimalx = j;
				if (curd > optimal && pl % 2 == 1)
					optimal = curd, optimaly = i, optimalx = j;
				Clear(t, i, j, 2 - pl % 2 + '0'); 
			}
		}
	return optimal;
}

int main()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			f[i][j] = '0';
	f[4][4] = f[5][5] = '2';
	f[4][5] = f[5][4] = '1';
	scanf ("%d", &my_pl);
	int cur_pl = 1;
	int ost = 64;
	//Print_field();
	while (ost)
	{
		bool makemove = 0;
		if (cur_pl == my_pl)
		{
			int maximal = inf;
			int curx = 1, cury = 1;
			for (int i = 1; i <= 8; i++)
				for (int j = 1; j <= 8; j++)
					if (f[i][j] == '0')
					{
						TryMove cur = Check(i, j, cur_pl + '0');
						if (cur.all)
						{
							makemove = 1;
							Draw(cur, i, j, cur_pl + '0');
							int curmark = MinMax(cur_pl + 1);
							if (maximal == inf)
								maximal = curmark, cury = i, curx = j;
							if (curmark > maximal && cur_pl == 1)
								maximal = curmark, cury = i, curx = j;
							if (curmark < maximal && cur_pl == 2)
								maximal = curmark, cury = i, curx = j;
							Clear(cur, i, j, cur_pl + '0');
						}
					}
			if (makemove)
			{
				TryMove t2 = Check(cury, curx, cur_pl + '0');
				Draw(t2, cury, curx, cur_pl + '0');
				printf ("%c%d", cury - 1 + 'a', curx);
                                cout << endl;
			} else {
				printf ("Skip");
                                cout << endl;
			//Print_field();
                        }
		}
		else
		{
			string s;
			cin >> s;
			if (s != "Skip")
			{
				TryMove x = Check(s[0] - 'a' + 1, s[1] - '0', cur_pl + '0');
				Draw(x, s[0] - 'a' + 1, s[1] - '0', cur_pl + '0');
			//	Print_field();
			}
		}
		ost--, cur_pl = 3 - cur_pl;
	}
	return 0;
}
