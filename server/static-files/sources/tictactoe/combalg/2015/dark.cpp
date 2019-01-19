#include <bits/stdc++.h>

using namespace std;

int data[9];

void init() {
    for (int i = 0; i < 9; ++i) {
        data[i] = 0;
    }
}

bool fap(int d) {
    if (data[d] != 0) {
        return false;
    }
    cout << d << endl;
    data[d] = 1;
    return true;
}

void read() {
    int a;
    cin >> a;
    data[a] = 1;
}

int main()
{
    srand(time(0));
    init();
    int n, ran;
    cin >> n;
    if (n == 1) {
        while (true) {
            ran = rand() % 9;
            while (!fap(ran)) {
                ran = rand() % 9;
            }
            read();
        }
    }
    else {
        while (true) {
            read();
            ran = rand() % 9;
            while (!fap(ran)) {
                ran = rand() % 9;
            }
        }
    }
    return 0;
}
