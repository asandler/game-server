#include "iostream"
#include "cmath"
#include "vector"
#include "set"
#include "deque"
#include "map"
#include "stack"
#include "bitset"
#include "string"
#include "time.h"
#include "cstdlib"
#include "cstdio"
#include "string.h"
#include "algorithm"
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

#define mk make_pair
#define times clock()*1.0/CLOCKS_PER_SEC
#define forn(i, n) for(int i = 0; i < (ll)n; i++)
#define fornn(i, s, n) for(ll i = (ll)s; i < (ll)n; i++)

const ll inf = 13e9;
const ll dd = 1e5;

int a, b;
short M[2][8] = {{0, 0, 1, 1, 1, -1, -1, -1}, {1, -1, 0, 1, -1, 0, 1, -1}};
struct T{
	ull q, w;
	T(){
		q = w = 0;
	}
	inline bool getq(int i, int j){
		ull x = i * 8 + j;
		return (bool)(q & ((ull)(1) << x));
	}
	inline bool getw(int i, int j){
		ull x = i * 8 + j;
		return (bool)(w & ((ull)(1) << x));
	}
	inline short get(int i, int j){
		if(i < 0 || i >= 8 || j < 0 || j >= 8)
			return 0;
		if(getq(i, j))
			return (getw(i, j) ? 1 : -1);
		return 0;
	}
	inline void setu(int i, int j, int a){
		ull x = i * 8 + j;
		q |= (ull)(1) << x;
		if(a > 0)
			w |= (ull)(1) << x;
		else
			w &= ~((ull)(1) << x);
	}
	inline int set(int i, int j, int d){
		if(get(i, j) != 0)
			return 0;
		int s = 0;
		forn(k, 8){
			int c = 0, a, b;
			for(a = i + M[0][k], b = j + M[1][k]; a < 8 && a >= 0 && b < 8 && b >= 0 && get(a, b) == -d; a += M[0][k], b += M[1][k])
				c += d;
			if(c != 0 && get(a, b) == d){
				s += c;
				for(a = i + M[0][k], b = j + M[1][k]; a < 8 && a >= 0 && b < 8 && b >= 0 && get(a, b) == -d; a += M[0][k], b += M[1][k])
					setu(a, b, d);
			}
		}
		if(s != 0)
			setu(i, j, d), s += d;
		return s;
	}
}P;

bool fl;
double timeT;
int xx, yy;
int dfsal(int q, int s, int gl, T &t, int al, int bl, int n){
	if((gl == 7 && rand() % 23 != 0) || gl == 9 || times - timeT > 1.95){
		if(gl == 9)
			s += 2 * n;
		return s + t.get(0, 0) + t.get(7, 0) + t.get(0, 7) + t.get(7, 7);
	}
	bool flag = 0;
	T f = t;
	int ans = 1000 * q * -1, x, y;
	forn(i, 8)
		forn(j, 8){
			int k = f.set(i, j, q);
			if(k != 0){
 				int c = dfsal(-q, s + k, gl + 1, f, al, bl, n);
				if(((ans > c) ^ (q > 0)) || (ans == c && rand() % 5 == 0))
					ans = c, flag = 1, x = i, y = j;
				f = t;
				if(q == n)
					bl = max(bl, c * n);
				else
					al = min(al, c * n);
				if(al < bl)
					return ans;
			}
	}
	if(flag){
		xx = x, yy = y; 
		return ans;
	}
	if(gl == 0)
		fl = 0;
	return dfsal(-q, s, gl + 1, t, al, bl, n);
}


int go(int c){
	xx = yy = 0;
	fl = 1;
	timeT = times;
	int d = dfsal(c, 0, 0, P, dd, -dd, c);
	if(fl){
		int ff = P.set(xx, yy, c);
		return d;
	}
	return dd;
}

int main(){
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	ios_base::sync_with_stdio(0);
	srand(time(0));

	P.setu(3, 3, 1), P.setu(4, 4, 1);
	P.setu(3, 4, -1), P.setu(4, 3, -1);
	
	int n, e = -1;
	cin>>n;
	char q, w;
	if(n == 2){
		e = 1;
		cin>>q>>w;
		P.set(q - 'a', w - '1', -e);
	}
	while(1){//1 - X      2 + O
		int a = go(e);
		if(a != dd)
			cout<<(char)(xx + 'a')<<(char)(yy + '1')<<'\n';
		else 
			cout<<"Skip\n";
		string s;
		cin>>s;
		if(s != "Skip")
			P.set(s[0] - 'a', s[1] - '1', -e);
	}
}
