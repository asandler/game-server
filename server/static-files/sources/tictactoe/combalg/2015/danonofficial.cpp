#include <iostream>
#include <string>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;
int graph[9];
int main()
{
    srand(time(0));
    for(int i = 0; i < 9; i++){
        graph[i] = 0;
    }
    for(int i = 0; i < 9; i++){
        int a;
        cin >> a;
        if(a == 1){
            a = rand() % 9;
            while(graph[a] != 0){
                a = rand() % 9;
            }
            cout << a << endl;
        }
        else{
            cin >> a;
            graph[a] = 1;
            a = rand() % 9;
            while(graph[a] != 0){
                a = rand() % 9;
            }
            cout << a << endl;
        }
    }
}
