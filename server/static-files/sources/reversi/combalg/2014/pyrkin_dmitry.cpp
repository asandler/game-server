#include<iostream>
#include <stdio.h>
#define inf 2000000000
using namespace std;
int f[10][10],tox,toy;
pair<int,int>w[8];
struct dr{
	int s,w[8];
};
int ranc(){
	int b=0,w=0;
	for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
			if(f[i][j]==1)
				b++;
			else
				if(f[i][j]==-1)
					w++;
	if(!b)
		return -inf;
	if(!w)
		return inf;
	return b-w;
}
void check(int x, int y, int player, dr &t){
	for(int i=0;i<8;i++){
		int x2=x+w[i].first,y2=y+w[i].second,s=0;
		if(f[x2][y2]==-player){
			while(f[x2][y2]==-player){
				x2+=w[i].first;
				y2+=w[i].second;
				s++;
			}
			if(f[x2][y2]==player){
				t.w[i]=s;
				t.s+=s;
			}
		}
	}
}
void draw(int x, int y, int player, dr &t){
	for(int i=0;i<8;i++)
		if(t.w[i]){
			int x2=x+w[i].first,y2=y+w[i].second;
			while(f[x2][y2]!=player){
				f[x2][y2]=player;
				x2+=w[i].first;
				y2+=w[i].second;
			}
		}
	f[x][y]=player;
}
void undraw(int x, int y, int player, dr &t){
	f[x][y]=0;
	for(int i=0;i<8;i++)
		if(t.w[i]){
			int x2=x+w[i].first,y2=y+w[i].second;
			for(int j=0;j<t.w[i];j++){
				f[x2][y2]=-player;
				x2+=w[i].first;
				y2+=w[i].second;
			}
		}
}
int minmax(int player, int deep){
	if(deep==4)
		return ranc();
	char flag=1;
	int br=(inf+1)*-player;;
	for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
			if(!f[i][j]){
				dr t;
				t.s=0;
				for(int k=0;k<8;k++)
					t.w[k]=0;
				check(i,j,player,t);
				if(t.s){
					flag=0;
					draw(i,j,player,t);
					int r=minmax(-player,deep+1);
					undraw(i,j,player,t);
					if(player==1){
						if(r>br){
							br=r;
							if(!deep){
								tox=i;
								toy=j;
							}
						}
					}else
						if(r<br){
							br=r;
							if(!deep){
								tox=i;
								toy=j;
							}
						}
				}
			}
	if(flag && deep)
		return minmax(-player,deep+1);
	return br;
}
void print(){
	printf(" ");
	for(int i=1;i<=8;i++)
		printf(" %d",i);
	printf("\n");
	for(int i=1;i<=8;i++){
		printf("%c",i+'a'-1);
		for(int j=1;j<=8;j++)
			if(f[i][j]==1)
				printf(" 1");
			else
				if(f[i][j]==-1)
					printf(" 2");
				else
					printf(" 0");
		printf("\n");
	}
}
int main(){
	for(int i=0;i<=9;i++)
		f[0][i]=f[i][0]=f[9][i]=f[i][9]=2;
	f[4][5]=f[5][4]=1;
	f[4][4]=f[5][5]=-1;
	w[0]=make_pair(1,0);
	w[1]=make_pair(0,1);
	w[2]=make_pair(-1,0);
	w[3]=make_pair(0,-1);
	w[4]=make_pair(1,1);
	w[5]=make_pair(-1,-1);
	w[6]=make_pair(1,-1);
	w[7]=make_pair(-1,1);
	int player;
	dr t;
	cin>>player;
	if(player==1){
		tox=-1;
		toy=-1;
		minmax(player,0);
		t.s=0;
		for(int i=0;i<8;i++)
			t.w[i]=0;
		check(tox,toy,player,t);
		draw(tox,toy,player,t);
		printf("%c%d",tox+'a'-1,toy);
                cout << endl;
	}else
		player=-1;
	for(;;){
		int x,y;
		char x2;
		cin>>x2;
		if(x2!='S'){
			cin>>y;
			x=x2-'a'+1;
			t.s=0;
			for(int i=0;i<8;i++)
				t.w[i]=0;
			check(x,y,-player,t);
			draw(x,y,-player,t);
		}else
			scanf("%*c%*c%*c");
		tox=-1;
		toy=-1;
		minmax(player,0);
		t.s=0;
		for(int i=0;i<8;i++)
			t.w[i]=0;
		check(tox,toy,player,t);
		draw(tox,toy,player,t);
		if(tox==-1) {
			printf("Skip");
                        cout << endl;
		} else {
			printf("%c%d",tox+'a'-1,toy);
                        cout << endl;
                }
	}
}
