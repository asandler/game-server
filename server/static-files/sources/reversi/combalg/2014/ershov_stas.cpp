#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

char board[8][8], BB='&', WW='*', boards[100][8][8];
int cnt, cntbin1, cntbin2, cnt2;
int t=0;
clock_t start_time=0;
int max_depth=4;
bool print_moves=true, print_board=true, random_depth;

void print_move(int i, int j, bool skip) {
	if (!print_moves) return;
	if (skip) cout << "Skip" << endl;
	else cout << (char)((char)j+'a') << (char)((char)i+'1') << endl;
}

inline void changeWB() {
	swap(BB, WW);
}

bool make_move(int i, int j) {
	int i2, j2;
	bool flag=false;
	for (i2 = i - 1; i2 >= 1 && board[i2][j] == WW; i2--) ;
	if (i-i2>1&&board[i2][j]==BB) for (int i3=i2; i3<=i; i3++) board[i3][j]=BB, flag=true;
	for (i2=i+1; i2<7&&board[i2][j]==WW; i2++) ;
	if (i2-i>1&&board[i2][j]==BB) for (int i3=i; i3<=i2; i3++) board[i3][j]=BB, flag=true;
	
	for (j2=j-1; j2>=1 && board[i][j2]==WW; j2--) ;
	if (j-j2>1 && board[i][j2]==BB) for (int j3=j2; j3<=j; j3++) board[i][j3]=BB, flag=true;
	for (j2=j+1; j2<7&&board[i][j2]==WW; j2++) ;
	if (j2-j>1&&board[i][j2]==BB) for (int j3=j; j3<=j2; j3++) board[i][j3]=BB, flag=true;
	
	for (j2=1; j2+i<7&&j2+j<7&&board[i+j2][j+j2]==WW; j2++) ;
	if (j2>1&&board[i+j2][j+j2]==BB) for (int j3=0; j3<=j2; j3++) board[i+j3][j+j3]=BB, flag=true;
	for (j2=-1; j2+i>=1&&j2+j>=1&&board[i+j2][j+j2]==WW; j2--) ;
	if (j2<-1&&board[i+j2][j+j2]==BB) for (int j3=j2; j3<=0; j3++) board[i+j3][j+j3]=BB, flag=true;
	
	for (j2=1; j2+i<7&&j-j2>=1&&board[i+j2][j-j2]==WW; j2++) ;
	if (j2>1&&board[i+j2][j-j2]==BB) for (int j3=0; j3<=j2; j3++) board[i+j3][j-j3]=BB, flag=true;
	for (j2=-1; j2+i>=1&&j-j2<7&&board[i+j2][j-j2]==WW; j2--) ;
	if (j2<-1&&board[i+j2][j-j2]==BB) for (int j3=j2; j3<=0; j3++) board[i+j3][j-j3]=BB, flag=true;
	return flag;
}

bool possible_move(int i, int j) {
	int i2, j2;
	for (i2=i-1; i2>=1&&board[i2][j]==WW; i2--) ;
	if (i-i2>1&&board[i2][j]==BB) return true;
	for (i2=i+1; i2<7&&board[i2][j]==WW; i2++) ;
	if (i2-i>1&&board[i2][j]==BB) return true;
	
	for (j2=j-1; j2>=1&&board[i][j2]==WW; j2--) ;
	if (j-j2>1&&board[i][j2]==BB) return true;
	for (j2=j+1; j2<7&&board[i][j2]==WW; j2++) ;
	if (j2-j>1&&board[i][j2]==BB) return true;
	
	for (j2=1; j2+i<7&&j2+j<7&&board[i+j2][j+j2]==WW; j2++) ;
	if (j2>1&&board[i+j2][j+j2]==BB) return true;
	for (j2=-1; j2+i>=1&&j2+j>=1&&board[i+j2][j+j2]==WW; j2--) ;
	if (j2<-1&&board[i+j2][j+j2]==BB) return true;
	
	for (j2=1; j2+i<7&&j-j2>=1&&board[i+j2][j-j2]==WW; j2++) ;
	if (j2>1&&board[i+j2][j-j2]==BB) return true;
	for (j2=-1; j2+i>=1&&j-j2<7&&board[i+j2][j-j2]==WW; j2--) ;
	if (j2<-1&&board[i+j2][j-j2]==BB) return true;
	return false;
}

bool any_move() {
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (board[i][j]=='_') if (possible_move(i, j)) return true;
	return false;
}

int check_win() {
	int cntw=0, cntb=0, cnt0=0;
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) {
		if (board[i][j]==WW) cntw++;
		if (board[i][j]==BB) cntb++;
	}
	cnt0=64-cntb-cntw;
	if (!any_move()) {
		changeWB();
		if (!any_move()) cnt0=0;
		changeWB();
	}
	if (cnt0==0) {
		if (cntw==cntb) return 3;
		else if (cntw>cntb) return 2;
		else return 1;
	} else if (cntw==0) return 1;
	else if (cntb==0) return 2;
	else return 0;
}

int get_good_special_black(int cntb) {
	int good=0;
	good+=cntb;
	
	int a=50;
	if (board[0][0]==BB) good+=a;
	if (board[7][0]==BB) good+=a;
	if (board[0][7]==BB) good+=a;
	if (board[7][7]==BB) good+=a;
	
	if (board[0][0]==BB) for (int i=1; board[0][i]==BB; i++) good+=a;
	if (board[0][7]==BB) for (int i=6; board[0][i]==BB; i--) good+=a;
	if (board[7][0]==BB) for (int i=1; board[7][i]==BB; i++) good+=a;
	if (board[7][7]==BB) for (int i=6; board[7][i]==BB; i--) good+=a;
	
	if (board[0][0]==BB) for (int i=1; board[i][0]==BB; i++) good+=a;
	if (board[7][0]==BB) for (int i=6; board[i][0]==BB; i--) good+=a;
	if (board[0][7]==BB) for (int i=1; board[i][7]==BB; i++) good+=a;
	if (board[7][7]==BB) for (int i=6; board[i][7]==BB; i--) good+=a;
	
	for (int i=1; i<7; i++) if (board[i][0]=='_'&&board[i-1][0]==BB&&board[i+1][0]==BB&&board[i][1]==BB) good-=a;
	for (int i=1; i<7; i++) if (board[i][7]=='_'&&board[i-1][7]==BB&&board[i+1][7]==BB&&board[i][6]==BB) good-=a;
	for (int i=1; i<7; i++) if (board[0][i]=='_'&&board[0][i-1]==BB&&board[0][i+1]==BB&&board[1][i]==BB) good-=a;
	for (int i=1; i<7; i++) if (board[7][i]=='_'&&board[7][i-1]==BB&&board[7][i+1]==BB&&board[6][i]==BB) good-=a;
	
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (board[i][j]=='_') {
		int i2, j2;
		bool ok=false;
		for (i2=i-1; i2>=1&&board[i2][j]==BB; i2--) ;
		if (i-i2>1&&board[i2][j]==WW) good-=i-i2-1, ok=true;
		for (i2=i+1; i2<7&&board[i2][j]==BB; i2++) ;
		if (i2-i>1&&board[i2][j]==WW) good-=i2-i-1, ok=true;
		
		for (j2=j-1; j2>=1&&board[i][j2]==BB; j2--) ;
		if (j-j2>1&&board[i][j2]==WW) good-=j-j2-1, ok=true;
		for (j2=j+1; j2<7&&board[i][j2]==BB; j2++) ;
		if (j2-j>1&&board[i][j2]==WW) good-=j2-j-1, ok=true;
		
		for (j2=1; j2+i<7&&j2+j<7&&board[i+j2][j+j2]==BB; j2++) ;
		if (j2>1&&board[i+j2][j+j2]==WW) good-=j2-1, ok=true;
		for (j2=-1; j2+i>=1&&j2+j>=1&&board[i+j2][j+j2]==BB; j2--) ;
		if (j2<-1&&board[i+j2][j+j2]==WW) good-=-j2-1, ok=true;
		
		for (j2=1; j2+i<7&&j-j2>=1&&board[i+j2][j-j2]==BB; j2++) ;
		if (j2>1&&board[i+j2][j-j2]==WW) good-=j2-1, ok=true;
		for (j2=-1; j2+i>=1&&j-j2<7&&board[i+j2][j-j2]==BB; j2--) ;
		if (j2<-1&&board[i+j2][j-j2]==WW) good-=-j2-1, ok=true;
		if (ok) good-=6;
	}
	
	return good;
}

int get_good(int cntb, int cntw) {
	int good=get_good_special_black(cntb);
	changeWB();
	good-=get_good_special_black(cntw);
	changeWB();
	return good;
}

void get_prev_board(int depth, int i, int j) {
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) board[i][j]=boards[depth][i][j];
}

int alpha_beta(int depth, int alpha, int beta, bool maximizer) {
	int cntw=0, cntb=0, cnt0=0, win=0;
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) {
		if (board[i][j]==WW) cntw++;
		if (board[i][j]==BB) cntb++;
	}
	cnt0=64-cntb-cntw;
	
	bool is_any_move=any_move();
	if (!is_any_move) {
		changeWB();
		if (!any_move()) cnt0=0;
		changeWB();
	}
	if (cnt0==0) {
		if (cntw==cntb) win=3;
		else if (cntw>cntb) win=2;
		else return 1;
	} else if (cntw==0) win=1;
	else if (cntb==0) win=2;
	else win=0;
	if (win==1) return maximizer?1000000:-1000000;
	if (win==2) return maximizer?-1000000:1000000;
	if (win==3) return 0;
	
	if (depth==0||(random_depth&&(depth==1&&!(rand()%3)))||((double)clock()-start_time)/CLOCKS_PER_SEC>=1.9) {
		return maximizer?get_good(cntb, cntw):-get_good(cntb, cntw);
	} else {
		cnt++;
		for (int i=0; i<8; i++) for (int j=0; j<8; j++) boards[depth][i][j]=board[i][j];
		for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (board[i][j]=='_') {
			if (make_move(i, j)) {
				cnt2++;
				changeWB();
				if (maximizer==true) {
					alpha=max(alpha, alpha_beta(depth-1, alpha, beta, false));
					changeWB();
					get_prev_board(depth, i, j);
					if (beta<=alpha) return alpha;
				} else {
					beta=min(beta, alpha_beta(depth-1, alpha, beta, true));
					changeWB();
					get_prev_board(depth, i, j);
					if (beta<=alpha) return beta;
				}
			}
		}
		
		if (!is_any_move) {
			cnt2++;
			changeWB();
			if (maximizer==true) {
				alpha=max(alpha, alpha_beta(depth-1, alpha, beta, false));
				changeWB();
				if (beta<=alpha) return alpha;
			} else {
				beta=min(beta, alpha_beta(depth-1, alpha, beta, true));
				changeWB();
				if (beta<=alpha) return beta;
			}
		}
		if (maximizer==true) return alpha;
		else return beta;
	}
	return 0;
}

pair<int, int> find_move(int depth) {
	int board_saved[8][8], cntb=0, maxi=0, maxj=0, max=10000000;
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) board_saved[i][j]=board[i][j];
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (board[i][j]==BB) cntb++;
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (board[i][j]=='_') {
		make_move(i, j);
		int cntb2=0;
		for (int i2=0; i2<8; i2++) for (int j2=0; j2<8; j2++) if (board[i2][j2]==BB) cntb2++;
		if (cntb2>cntb) {
			changeWB();
			int alpha_beta_res=alpha_beta(depth, -1000000, +1000000, true);
			if (alpha_beta_res<max) maxi=i, maxj=j, max=alpha_beta_res;
			changeWB();
		}
		for (int i=0; i<8; i++) for (int j=0; j<8; j++) board[i][j]=board_saved[i][j];
	}
	if (max<10000000) return make_pair(maxi, maxj);
	else return make_pair(-1, -1);
}

void comp_move(int depth) {
	start_time=clock();
	cnt=cnt2=0;
	pair<int, int> ans;
	ans=find_move(depth);
	random_depth=false;
	while (depth<20) {
		if (((double)clock()-start_time)/CLOCKS_PER_SEC<0.01) depth+=4, ans=find_move(depth);
		else if (((double)clock()-start_time)/CLOCKS_PER_SEC<0.03) depth+=3, ans=find_move(depth);
		else if (((double)clock()-start_time)/CLOCKS_PER_SEC<0.07) depth+=2, ans=find_move(depth);
		else if (((double)clock()-start_time)/CLOCKS_PER_SEC<0.25) depth+=1, ans=find_move(depth);
		else if (((double)clock()-start_time)/CLOCKS_PER_SEC<0.5) random_depth=true, ans=find_move(depth);
		else break;
	}
	if (ans.first!=-1) {
		make_move(ans.first, ans.second);
		print_move(ans.first, ans.second, false);
	}
	else return print_move(0, 0, true);
}

int main() {
	srand((int)time(NULL));
	for (int i=0; i<8; i++) for (int j=0; j<8; j++) board[i][j]='_';
	board[3][4]=board[4][3]=BB;
	board[3][3]=board[4][4]=WW;
	cin >> t;
	if (t==1) comp_move(0);
	while (true) {
		if (t==1) changeWB();
		char m[10];
		cin >> m;
		if (m[0]!='S') {
			int i=m[1]-'1', j=m[0]-'a';
			make_move(i, j);
		}
		if (t==1) changeWB();
		
		if (check_win()) break;
		
		if (t==2) changeWB();
		comp_move(max_depth);
		if (t==2) changeWB();
		
		if (check_win()) break;
	}
	return 0;
}