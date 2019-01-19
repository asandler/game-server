// CrossZeros.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>

using namespace std;

const int Size = 9;

int player;
unsigned int mine = 0, its = 0;

bool bit(unsigned int x, int i)
{
	return ((x & (1 << i)) >> i);
}

void make_bit(unsigned int &x, int i)
{
	x |= (1 << i);
}

int fun(unsigned int mine, unsigned int its)
{
	for (int i = 0; i < 3; i++)
	{
		if (!bit(its, i))
			continue;
		if (bit(its, i + 3) && bit(its, i + 6))
			return -1;
		if (i == 0)
		{
			if (bit(its, i + 1) && bit(its, i + 2) || bit(its, i + 4) && bit(its, i + 8))
				return -1;
		}
		if (i == 2 && bit(its, 4) && bit(its, 6))
			return -1;
	}
	for (int i = 3; i <= 6; i += 3)
	{
		if (!bit(its, i))
			continue;
		if (bit(its, i + 1) && bit(its, i + 2))
			return -1;
	}
	for (int i = 0; i < 3; i++)
	{
		if (!bit(mine, i))
			continue;
		if (bit(mine, i + 3) && bit(mine, i + 6))
			return 1;
		if (i == 0)
		{
			if (bit(mine, i + 1) && bit(mine, i + 2) || bit(mine, i + 4) && bit(mine, i + 8))
				return 1;
		}
		if (i == 2 && bit(mine, 4) && bit(mine, 6))
			return 1;
	}
	for (int i = 3; i <= 6; i += 3)
	{
		if (!bit(mine, i))
			continue;
		if (bit(mine, i + 1) && bit(mine, i + 2))
			return 1;
	}
	return 0;
}

int minimax(unsigned int mine, unsigned int its, int color)
{
	//if (mine + its == (1 << Size) - 1)
	{
		int x = fun(mine, its);
		if ((mine + its == (1 << Size) - 1) || x != 0)
			return x;
	}
	int ans = -1 * color;
	for (int i = 0; i < Size; i++)
	{
		if (bit(mine, i) || bit(its, i))
			continue;
		unsigned int next = mine;
		if (color == -1)
			next = its;
		make_bit(next, i);

		if (color == 1)
			ans = max(ans, minimax(next, its, -color));
		else
			ans = min(ans, minimax(mine, next, -color));
		//if (ans * color == 1)
		//	return ans;
	}
	return ans;
}

int main()
{
	cin >> player;

	if (player == 1)
	{
		make_bit(mine, 4);
		cout << "4\n";
	}

	while (mine + its != (1 << Size) - 1)
	{
		int pos;
		cin >> pos;
		make_bit(its, pos);
		if (mine + its == (1 << Size) - 1)
			break;
		int mx = -10, ind = -1;
		for (int i = 0; i < Size; i++)
		{
			if (bit(mine, i) || bit(its, i))
				continue;
			unsigned int nxt_me = mine;
			make_bit(nxt_me, i);
			int nxt = minimax(nxt_me, its, -1);
			if (mx < nxt)
			{
				mx = nxt;
				ind = i;
			}
		}
		make_bit(mine, ind);
		cout << ind << '\n';
	}
	
	return 0;
}

