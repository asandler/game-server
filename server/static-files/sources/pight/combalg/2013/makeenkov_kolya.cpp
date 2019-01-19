/*CODE BY NICKOLAY MAKEENKOV*/

#include <cstdio>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <string>
#include <queue>
#include <ctime>

using namespace std;

#define cps CLOCKS_PER_SEC

int maxlev = 2;
long long from = -100500;
bool cont = 1;

const double maxcps = 0.7;
const double mincps = 0.3;
const int maxdept = 9;

const int inf = 2000 * 1000 * 1000 + 7;

const int n = 30;
const int m = 40;

struct point {
	int x, y;
};

struct ret {
	int func;
	char dir;
};

char t[n + 5][m + 5];
point coord[4];
char powertogo[3];

struct snake {
	int dir;
	vector <point> s;//snake
	vector <point> e;//eaten

	void go(char c) {
		if (c == 'L') {
			dir = (dir + 3) % 4;
		}
		if (c == 'R') {
			dir = (dir + 1) % 4;
		}
		
		point lol;
		lol.x = s[s.size() - 1].x + coord[dir].x;
		lol.y = s[s.size() - 1].y + coord[dir].y;
		if (lol.x < 0 || lol.x >= n || lol.y < 0 || lol.y >= m || (t[lol.x][lol.y] == 'S')) {
			//cerr << c << "  " << lol.x << " " << lol.y << "  " << s[s.size() - 1].x << " " << s[s.size() - 1].y << "   " << dir;
			//cerr << t[lol.x][lol.y] << "\n";
			return;
		}
		s.push_back(lol);

		bool any = 0;
		if (e.size() > 0 && s[0].x == e[0].x && s[0].y == e[0].y) {
			any = 1;
	
			//pop_front
			for (int i = 1; i < (int) e.size(); ++i) {
				e[i - 1] = e[i];
			}
			e.pop_back();
		}

		if (!any) {
			t[s[0].x][s[0].y] = '0';

			//pop_front
			for (int i = 1; i < (int) s.size(); ++i) {
				s[i - 1] = s[i];
			}
			s.pop_back();
		}

		if (t[lol.x][lol.y] == '1') {
			e.push_back(lol);
		}
		t[lol.x][lol.y] = 'S';
	}

};

snake me, him;
snake lol, lol2;

void init(int mee) {
	vector <point> a(4);
	for (int i = 0; i < 4; ++i) {
		a[i].x = i;
		a[i].y = 0;
		t[i][0] = 'S';
	}
	me.s = a;
	me.dir = 0;
	me.e.clear();
	for (int i = 0; i < 4; ++i) {
		a[i].x = n - 1 - i;
		a[i].y = m - 1;
		t[n - 1 - i][m - 1] = 'S';
	}
	him.s = a;
	him.dir = 2;
	him.e.clear();
	
	if (mee == 2) {
		swap(me, him);
	}

	coord[0].x = 1;
	coord[0].y = 0;

	coord[1].x = 0;
	coord[1].y = -1;
	
	coord[2].x = -1;
	coord[2].y = 0;
	
	coord[3].x = 0;
	coord[3].y = 1;
	
	powertogo[0] = 'F';
	powertogo[1] = 'L';
	powertogo[2] = 'R';
}

int f() {
	if (maxlev > 6) {
		//time is out
		if ((1.0 * (clock() - from)) / cps > maxcps) {
			cont = 0;
		}
		//time is out
	}
	
	//100% lose
	queue <point> q;
	q.push(lol.s[lol.s.size() - 1]);
	int bal = -1;
	bool used[n][m];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			used[i][j] = 0;
		}
	}
	while (!(q.empty() || bal > (int) lol.s.size())) {
		point now = q.front();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			point to;
			to.x = now.x + coord[i].x;
			to.y = now.y + coord[i].y;
			if (to.x >= 0 && to.x < n && to.y >= 0 && to.y < m && t[to.x][to.y] != 'S' && used[to.x][to.y] == 0) {
				++bal;
				q.push(to);
				used[to.x][to.y] = 1;
			}
		}
	}
	if (bal < ((int) lol.s.size() + 5) / 2) {
		return (-inf + 1);
	}

	//the second snake
	
	q.push(lol2.s[lol2.s.size() - 1]);
	bal = -1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			used[i][j] = 0;
		}
	}
	while (!(q.empty() || bal > (int) lol2.s.size())) {
		point now = q.front();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			point to;
			to.x = now.x + coord[i].x;
			to.y = now.y + coord[i].y;
			if (to.x >= 0 && to.x < n && to.y >= 0 && to.y < m && t[to.x][to.y] != 'S' && used[to.x][to.y] == 0) {
				++bal;
				q.push(to);
				used[to.x][to.y] = 1;
			}
		}
	}
	if (bal < ((int) lol2.s.size() + 1) / 2) {
		return (inf - 1);
	}
	//100% lose

	int lendiff = (int) lol.s.size() + (int) lol.e.size() - (int) lol2.s.size() - (int) lol2.e.size();

	int to1 = 0;
	int to2 = 0;
	int d1[n + 5][m + 5];
	int d2[n + 5][m + 5];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			d1[i][j] = 100500;
			d2[i][j] = 100500;
		}
	}
	queue <point> qq;
	qq.push(lol.s[lol.s.size() - 1]);
	while (!qq.empty()) {
		point now = qq.front();
		qq.pop();
		if (d1[now.x][now.y] == 100500) {
			d1[now.x][now.y] = 0;
		}
		for (int i = 0; i < 4; ++i) {
			point to;
			to.x = now.x + coord[i].x;
			to.y = now.y + coord[i].y;
			if (to.x >= 0 && to.x < n && to.y >= 0 && to.y < m && t[to.x][to.y] != 'S' && d1[to.x][to.y] == 100500) {
				d1[to.x][to.y] = d1[now.x][now.y] + 1;
				qq.push(to);
			}
		}
	}
	
	qq.push(lol2.s[lol2.s.size() - 1]);
	while (!qq.empty()) {
		point now = qq.front();
		qq.pop();
		if (d2[now.x][now.y] == 100500) {
			d2[now.x][now.y] = 0;
		}
		for (int i = 0; i < 4; ++i) {
			point to;
			to.x = now.x + coord[i].x;
			to.y = now.y + coord[i].y;
			if (to.x >= 0 && to.x < n && to.y >= 0 && to.y < m && t[to.x][to.y] != 'S' && d2[to.x][to.y] == 100500) {
				d2[to.x][to.y] = d2[now.x][now.y] + 1;
				qq.push(to);
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (t[i][j] == '1') {
				int dd1 = 7000 / d1[i][j];
				int dd2 = 7000 / d2[i][j];

				//if (dd1 > dd2) {
					to1 += dd1;
				//}
				//if (dd2 < dd1) {
					to2 += dd2;
				//}
			}
		}
	}

	int ans = 77777 * lendiff + to1 - to2;
	return ans;
}

ret dfs(int lev) {
	/*
	//IF FAIL
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (t[i][j] == 'S') {
				bool fail = 1;
				for (int l = 0; l < (int) lol.s.size(); ++l) {
					if (lol.s[l].x == i && lol.s[l].y == j) {
						fail = 0;
					}
				}
				for (int l = 0; l < (int) lol2.s.size(); ++l) {
					if (lol2.s[l].x == i && lol2.s[l].y == j) {
						fail = 0;
					}
				}
				if (fail) {
					cerr << "FAIL\n";
				}
			}
		}
	}
	//IF FAIL
	*/
	ret ans;
	ans.dir = '/';
	if (lev == maxlev) {
		ans.func = f();
		return ans;
	}
	snake prev;
	if (lev % 2 == 0) {//ME
		prev = lol;
		ans.func = -inf;
		for (int i = 0; i < 3; ++i) {
			char c = powertogo[i];
			lol.go(c);
			if (lol.s[lol.s.size() - 1].x != prev.s[prev.s.size() - 1].x || lol.s[lol.s.size() - 1].y != prev.s[prev.s.size() - 1].y) {
				ret tmp = dfs(lev + 1);
				if (tmp.func > ans.func) {
					ans.func = tmp.func;
					ans.dir = c;
				}
				if (lol.e.size() > 0 && lol.e[lol.e.size() - 1].x == lol.s[lol.s.size() - 1].x &&
					lol.e[lol.e.size() - 1].y == lol.s[lol.s.size() - 1].y) {
					
					t[lol.s[lol.s.size() - 1].x][lol.s[lol.s.size() - 1].y] = '1';
				} else {
					t[lol.s[lol.s.size() - 1].x][lol.s[lol.s.size() - 1].y] = '0';
				}
				t[prev.s[0].x][prev.s[0].y] = 'S';
			}
			lol = prev;
			
			if (!cont) {
				return ans;
			}

		}
	} else {//HIM
		prev = lol2;
		ans.func = inf;
		for (int i = 0; i < 3; ++i) {
			char c = powertogo[i];
			lol2.go(c);
			if (lol2.s[lol2.s.size() - 1].x != prev.s[prev.s.size() - 1].x || lol2.s[lol2.s.size() - 1].y != prev.s[prev.s.size() - 1].y) {
				ret tmp = dfs(lev + 1);
				if (tmp.func < ans.func) {
					ans.func = tmp.func;
					ans.dir = c;
				}
				if (lol2.e.size() > 0 && lol2.e[lol2.e.size() - 1].x == lol2.s[lol2.s.size() - 1].x && 
					lol2.e[lol2.e.size() - 1].y == lol2.s[lol2.s.size() - 1].y) {
					
					t[lol2.s[lol2.s.size() - 1].x][lol2.s[lol2.s.size() - 1].y] = '1';
				} else {
					t[lol2.s[lol2.s.size() - 1].x][lol2.s[lol2.s.size() - 1].y] = '0';
				}
				t[prev.s[0].x][prev.s[0].y] = 'S';
			}
			lol2 = prev;
			
			if (!cont) {
				return ans;
			}

		}
	}
	if (ans.dir == '/') {
		if (lev % 2 == 0) {//ME2
			ans.func = -inf + 1;
		} else {//HIM2
			ans.func = inf - 1;
		}
	}
	return ans;
}

char firstgo() {
	from = clock();
	cont = 1;

	lol = me;
	lol2 = him;
	
	ret res = dfs(0);
	me.go(res.dir);
	
	if ((1.0 * (clock() - from)) / cps < mincps) {
		maxlev++;
		maxlev = min(maxlev, maxdept);
	}
	if (cont == 0) {
		maxlev--;
	}

	return res.dir;
}

int main() {
	int mee;
	cin >> mee;
	string s;
	getline(cin, s);
	for (int i = 0; i < n; ++i) {
		getline(cin, s);
		for (int j = 0; j < m; ++j) {
			t[i][j] = s[j];
		}
	}
	init(mee);
	if (mee == 1) {
		cout << firstgo() << endl;
	}
	while (1) {
		char c;
		cin >> c;
		//cerr << "mak " << maxlev << " " << clock() << "\n";
		him.go(c);
		/*
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				cerr << t[i][j];
			}
			cerr << endl;
		}
		cerr << endl << endl;
		*/
		cout << firstgo() << endl;
	}
	return 0;
}
