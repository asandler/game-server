//
//  main.cpp
//  Tic-Tac-to
//
//  Created by Данила Шепелёв on 22.08.15.
//  Copyright (c) 2015 Данила Шепелёв. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

bool sins(char a, string b){
    for (int i=0; i<b.length(); i++) if (a==b[i]) return true;
    return false;
}

/*  
 
 
 
 
 
 3, 1
 
 */

char ghj1(string b, string a, int m, int s){
    string v="012345678";
    int num=0;
    for (int i=m; i<m+3*s; i+=s){
        for (int j=0; j<a.length(); j++) {
            if (v[i]==a[j] && b[j]=='v') num++;
        }
    }
    if (num>=2) for (int i=m; i<m+3*s; i+=s){
            if (!sins(v[i], a)) return i;
    }
    return 9;
}

char ghj2(string b, string a, int m, int s){
    string v="012345678";
    int num=0;
    for (int i=m; i<m+3*s; i+=s){
        for (int j=0; j<a.length(); j++) {
            if (v[i]==a[j] && b[j]=='i') num++;
        }
    }
    if (num>=2) for (int i=m; i<m+3*s; i+=s){
        if (!sins(v[i], a)) return i;
    }
    return 9;
}


char optim(string a, string b){
    int prior[10];
    string v="012345678";
    for (int i=0; i<9; i++) {
        if (v[i]=='4') prior[i]=3;
        if (sins(v[i], "0268")) prior[i]=2;
        if (sins(v[i], "1357")) prior[i]=1;
    }
    for (int i=0; i<a.length(); i++) {
        for (int j=0; j<9; j++) if (a[i]==v[j]) prior[j]=0;
    }
    int g=0;
    prior[9]=0;
    
    prior[ghj1(b, a, 0, 4)]*=100;
    prior[ghj1(b, a, 2, 2)]*=100;
    prior[ghj1(b, a, 0, 3)]*=100;
    prior[ghj1(b, a, 1, 3)]*=100;
    prior[ghj1(b, a, 2, 3)]*=100;
    prior[ghj1(b, a, 0, 1)]*=100;
    prior[ghj1(b, a, 3, 1)]*=100;
    prior[ghj1(b, a, 6, 1)]*=100;
    
    prior[ghj2(b, a, 0, 4)]*=10;
    prior[ghj2(b, a, 2, 2)]*=10;
    prior[ghj2(b, a, 0, 3)]*=10;
    prior[ghj2(b, a, 1, 3)]*=10;
    prior[ghj2(b, a, 2, 3)]*=10;
    prior[ghj2(b, a, 0, 1)]*=10;
    prior[ghj2(b, a, 3, 1)]*=10;
    prior[ghj2(b, a, 6, 1)]*=10;
    
    for (int i=1; i<9; i++) if (prior[g]<prior[i]) g=i;
    return v[g];
    
    
}

int main(int argc, const char * argv[]) {
    int i;
    //cout<<"test signal\n";
    cin>>i;
    string a="", b="";
    char m;
    if (i==2) {
        //cout<<"Your turn\n";
        cin>>m; a+=m; b+='v';
    }
    //cout<<"start game\n";
    while (a.length()<9){
        if (b[b.length()-1]=='i'){
            char h;
            //cout<<"Your turn\n";
            cin>>h;
            a+=h;
            b+='v';
        }
        else{
            char l=optim(a, b);
            a+=l;
            b+='i';
            //cout<<"my turn\n";
            cout<<a[a.length()-1]<<endl;
        }
    }
    return 0;
}
