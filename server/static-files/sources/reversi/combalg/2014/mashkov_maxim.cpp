#include <stdio.h>
#include <iostream>

using namespace std;

typedef struct
{
	int count, x, y, c, n[8];
} mv;

int vtx[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
int vty[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int field[10][10];
int mc, inf = -1000000;

void print_field()
{
	printf("\n  ");
	for (int i = 1; i < 9; ++i)
		printf("%d ", i);
	printf("\n");
	for (int i = 1; i < 9; ++i)
	{
		printf("%c ", char(96 + i));
		for (int j = 1; j < 9; ++j)
			if (field[i][j] == 1)
				printf("1 ");
			else if (field[i][j] == -1)
				printf("0 ");
			else
				printf("  ");
		printf("\n");
	}
	printf("\n\n");
}

void check(mv &t)
{
	t.count = 0;
	if (field[t.x][t.y] != 0)
		return;
	int x, y, k;
	for (int i = 0; i < 8; ++i)
	{
		t.n[i] = 0;
		x = t.x; y = t.y; k = -1;
		do {
			x += vtx[i];
			y += vty[i];
			k += 1;
		} while (field[x][y] == -t.c);
		if (field[x][y] == t.c)
		{
			t.count += k;
			t.n[i] = k;
		}
	}
	return;
}

void draw(mv &t)
{
	field[t.x][t.y] = t.c;
	int x, y;
	for (int i = 0; i < 8; ++i)
	{
		x = t.x; y = t.y;
		for (int j = 0; j < t.n[i]; ++j)
		{
			x += vtx[i];
			y += vty[i];
			field[x][y] = t.c;
		}
	}
}

void dedraw(mv &t)
{
	field[t.x][t.y] = 0;
	int x, y;
	for (int i = 0; i < 8; ++i)
	{
		x = t.x; y = t.y;
		for (int j = 0; j < t.n[i]; ++j)
		{
			x += vtx[i];
			y += vty[i];
			field[x][y] = -t.c;
		}
	}
}

int rating()
{
	int s1 = 0, s2 = 0;
	for (int i = 1; i < 9; ++i)
		for (int j = 1; j < 9; ++j)
			if (field[i][j] == 1)
				s1++;
			else if (field[i][j] == -1)
				s2++;
	return s1 - s2;
}

int minmax(int depth, int col)
{
	if (depth == 5)
		return rating();
	mv t; t.c = col;
	int x, y, mx = inf * col, g, f = 1;
	for (t.x = 1; t.x < 9; ++t.x)
		for (t.y = 1; t.y < 9; ++t.y)
		{
			check(t);
			if (t.count)
			{
				f = 0;
				draw(t);
				g = minmax(depth + 1, -col);
				if ((col == 1 && g > mx) || (col == -1 && g < mx))
				{
					mx = g;
					x = t.x;
					y = t.y;
				}
				dedraw(t);
			}
		}
	if (f)
		return minmax(depth + 1, -col);
	else
		return mx;
}

void get_move()
{
	char e1 = 0, e2 = 0;
	while ((e1 < 'a' || e1 > 'h') && (e1 != 'S'))
		scanf("%c", &e1);
	if (e1 == 'S')
		return;
	scanf("%c", &e2);
	mv t = { 0, e1 - 96, e2 - 48, -mc };
	check(t);
	draw(t);
}

void make_move()
{
	mv t; t.c = mc;
	int x, y, mx = inf * mc, g, f = 1;
	for (t.x = 1; t.x < 9; ++t.x)
		for (t.y = 1; t.y < 9; ++t.y)
		{
			check(t);
			if (t.count)
			{
				f = 0;
				draw(t);
				g = minmax(1, -mc);
				if ((mc == 1 && g > mx) || (mc == -1 && g < mx))
				{
					mx = g;
					x = t.x;
					y = t.y;
				}
				dedraw(t);
			}
		}
	if (f) {
		printf("Skip");
                cout << endl;
	} else
	{
		t.x = x;
		t.y = y;
		check(t);
		draw(t);
		printf("%c%c", char(x + 96), char(y + 48));
                cout << endl;
	}
}

int main()
{
	field[4][4] = field[5][5] = -1;
	field[4][5] = field[5][4] = 1;
	scanf("%d", &mc);
	if (mc == 2)
	{
		//print_field();
		mc = -1;
		get_move();
	}
	while (true)
	{
		//print_field();
		make_move();
		//print_field();
		get_move();
	}
	return 0;
}
