#include <bits/stdc++.h>
using namespace std;


void P(char h[10][10]){
    for (int i = 1; i <= 8; i++){
        for (int j = 1; j <= 8; j++){
            cout << h[i][j] << " ";
        }
        cout << "\n";
    }
}

void Position(int i, int j, int t, char h[10][10]){
    int a, b;
    if (t == 1){
        h[i][j] = 'b';
        if (h[i - 1][j - 1] == 'w'){
            a = i - 1;
            b = j - 1;
            while (h[a][b] == 'w'){
                a -= 1;
                b -= 1;
            }
            if (h[a][b] == 'b'){
                a += 1;
                b += 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a += 1;
                    b += 1;
                }
            }
        }

        if (h[i - 1][j] == 'w'){
            a = i - 1;
            b = j;
            while (h[a][b] == 'w'){
                a -= 1;
            }
            if (h[a][b] == 'b'){
                a += 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a += 1;
                }
            }
        }

        if (h[i - 1][j + 1] == 'w'){
            a = i - 1;
            b = j + 1;
            while (h[a][b] == 'w'){
                a -= 1;
                b += 1;
            }
            if (h[a][b] == 'b'){
                a += 1;
                b -= 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a += 1;
                    b -= 1;
                }
            }
        }

        if (h[i][j + 1] == 'w'){
            a = i;
            b = j + 1;
            while (h[a][b] == 'w'){
                b += 1;
            }
            if (h[a][b] == 'b'){
                b -= 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    b -= 1;
                }
            }
        }

        if (h[i + 1][j + 1] == 'w'){
            a = i + 1;
            b = j + 1;
            while (h[a][b] == 'w'){
                a += 1;
                b += 1;
            }
            if (h[a][b] == 'b'){
                a -= 1;
                b -= 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a -= 1;
                    b -= 1;
                }
            }
        }

        if (h[i + 1][j] == 'w'){
            a = i + 1;
            b = j;
            while (h[a][b] == 'w'){
                a += 1;
            }
            if (h[a][b] == 'b'){
                a -= 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a -= 1;
                }
            }
        }

        if (h[i + 1][j - 1] == 'w'){
            a = i + 1;
            b = j - 1;
            while (h[a][b] == 'w'){
                a += 1;
                b -= 1;
            }
            if (h[a][b] == 'b'){
                a -= 1;
                b += 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    a -= 1;
                    b += 1;
                }
            }
        }

        if (h[i][j - 1] == 'w'){
            a = i;
            b = j - 1;
            while (h[a][b] == 'w'){
                b -= 1;
            }
            if (h[a][b] == 'b'){
                b += 1;
                while (h[a][b] == 'w'){
                    h[a][b] = 'b';
                    b += 1;
                }
            }
        }
    }

    if (t == 2){
        h[i][j] = 'w';
        if (h[i - 1][j - 1] == 'b'){
            a = i - 1;
            b = j - 1;
            while (h[a][b] == 'b'){
                a -= 1;
                b -= 1;
            }
            if (h[a][b] == 'w'){
                a += 1;
                b += 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a += 1;
                    b += 1;
                }
            }
        }

        if (h[i - 1][j] == 'b'){
            a = i - 1;
            b = j;
            while (h[a][b] == 'b'){
                a -= 1;
            }
            if (h[a][b] == 'w'){
                a += 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a += 1;
                }
            }
        }

        if (h[i - 1][j + 1] == 'b'){
            a = i - 1;
            b = j + 1;
            while (h[a][b] == 'b'){
                a -= 1;
                b += 1;
            }
            if (h[a][b] == 'w'){
                a += 1;
                b -= 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a += 1;
                    b -= 1;
                }
            }
        }

        if (h[i][j + 1] == 'b'){
            a = i;
            b = j + 1;
            while (h[a][b] == 'b'){
                b += 1;
            }
            if (h[a][b] == 'w'){
                b -= 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    b -= 1;
                }
            }
        }

        if (h[i + 1][j + 1] == 'b'){
            a = i + 1;
            b = j + 1;
            while (h[a][b] == 'b'){
                a += 1;
                b += 1;
            }
            if (h[a][b] == 'w'){
                a -= 1;
                b -= 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a -= 1;
                    b -= 1;
                }
            }
        }

        if (h[i + 1][j] == 'b'){
            a = i + 1;
            b = j;
            while (h[a][b] == 'b'){
                a += 1;
            }
            if (h[a][b] == 'w'){
                a -= 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a -= 1;
                }
            }
        }

        if (h[i + 1][j - 1] == 'b'){
            a = i + 1;
            b = j - 1;
            while (h[a][b] == 'b'){
                a += 1;
                b -= 1;
            }
            if (h[a][b] == 'w'){
                a -= 1;
                b += 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    a -= 1;
                    b += 1;
                }
            }
        }

        if (h[i][j - 1] == 'b'){
            a = i;
            b = j - 1;
            while (h[a][b] == 'b'){
                b -= 1;
            }
            if (h[a][b] == 'w'){
                b += 1;
                while (h[a][b] == 'b'){
                    h[a][b] = 'w';
                    b += 1;
                }
            }
        }
    }
}

int Pos(int i, int j, int t, char h[10][10]){
    int a, b, k = 0;
    if (t == 1 && h[i][j] == '.'){
        if (h[i - 1][j - 1] == 'w'){
            a = i - 1;
            b = j - 1;
            while (h[a][b] == 'w'){
                a -= 1;
                b -= 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i - 1 << " " << j - 1 << "\n";
                return k;
            }
        }

        if (h[i - 1][j] == 'w'){
            a = i - 1;
            b = j;
            while (h[a][b] == 'w'){
                a -= 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i - 1 << " " << j << "\n";
                return k;
            }
        }

        if (h[i - 1][j + 1] == 'w'){
            a = i - 1;
            b = j + 1;
            while (h[a][b] == 'w'){
                a -= 1;
                b += 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i - 1 << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i][j + 1] == 'w'){
            a = i;
            b = j + 1;
            while (h[a][b] == 'w'){
                b += 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i + 1][j + 1] == 'w'){
            a = i + 1;
            b = j + 1;
            while (h[a][b] == 'w'){
                a += 1;
                b += 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i + 1 << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i + 1][j] == 'w'){
            a = i + 1;
            b = j;
            while (h[a][b] == 'w'){
                a += 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i + 1 << " " << j << "\n";
                return k;
            }
        }

        if (h[i + 1][j - 1] == 'w'){
            a = i + 1;
            b = j - 1;
            while (h[a][b] == 'w'){
                a += 1;
                b -= 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i + 1 << " " << j - 1 << "\n";
                return k;
            }
        }

        if (h[i][j - 1] == 'w'){
            a = i;
            b = j - 1;
            while (h[a][b] == 'w'){
                b -= 1;
            }
            if (h[a][b] == 'b'){
                k = 1;
                //cout << i << " " << j - 1 << "\n";
                return k;
            }
        }
    }

    if (t == 2 && h[i][j] == '.'){
        if (h[i - 1][j - 1] == 'b'){
            a = i - 1;
            b = j - 1;
            while (h[a][b] == 'b'){
                a -= 1;
                b -= 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i - 1 << " " << j - 1 << "\n";
                return k;
            }
        }

        if (h[i - 1][j] == 'b'){
            a = i - 1;
            b = j;
            while (h[a][b] == 'b'){
                a -= 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i - 1 << " " << j << "\n";
                return k;
            }
        }

        if (h[i - 1][j + 1] == 'b'){
            a = i - 1;
            b = j + 1;
            while (h[a][b] == 'b'){
                a -= 1;
                b += 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i - 1 << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i][j + 1] == 'b'){
            a = i;
            b = j + 1;
            while (h[a][b] == 'b'){
                b += 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i + 1][j + 1] == 'b'){
            a = i + 1;
            b = j + 1;
            while (h[a][b] == 'b'){
                a += 1;
                b += 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i + 1 << " " << j + 1 << "\n";
                return k;
            }
        }

        if (h[i + 1][j] == 'b'){
            a = i + 1;
            b = j;
            while (h[a][b] == 'b'){
                a += 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i + 1 << " " << j << "\n";
                return k;
            }
        }

        if (h[i + 1][j - 1] == 'b'){
            a = i + 1;
            b = j - 1;
            while (h[a][b] == 'b'){
                a += 1;
                b -= 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i + 1 << " " << j - 1 << "\n";
                return k;
            }
        }

        if (h[i][j - 1] == 'b'){
            a = i;
            b = j - 1;
            while (h[a][b] == 'b'){
                b -= 1;
            }
            if (h[a][b] == 'w'){
                k = 1;
                //cout << i << " " << j - 1 << "\n";
                return k;
            }
        }
    }
    return k;
}

int minimax(int a, int b, int n, int d, int t, char h[10][10]){
    char h1[10][10];
    int y = 0;
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            h1[i][j] = h[i][j];
        }
    }
    if (n == 1){
        Position(a, b, t, h1);
        /*cout << "n = " << n << "\n";
        P(h1);
        cout << "\n";
        cout << "\n";*/
        if (t == 1) t = 2;
        else t = 1;
        return minimax(a, b, n + 1, d, t, h1);
    }
    else if (n > d){
        /*cout << "n = " << n << "\n";
        P(h1);
        cout << "\n";
        cout << "\n";*/
        int b1 = 0, w1 = 0;
        for (int i = 1; i <= 8; i++){
            for (int j = 1; j <= 8; j++){
                if (h1[i][j] == 'b') b1 += 1;
                else if (h1[i][j] == 'w') w1 += 1;
            }
        }
        if (t == 1){
            return (b1 - w1);
        }
        if (t == 2){
            return(w1 - b1);
        }
    }
    else{
        int k = -2;
        for (int i = 1; i <= 8; i++){
            for (int j = 1; j <= 8; j++){
                if (Pos(i, j, t, h1)){
                    y = 1;
                    Position(i, j, t, h1);
                    /*cout << "n = " << n << "\n";
                    P(h1);
                    cout << "\n";
                    cout << "\n";*/
                    if (n != d){
                        if (t == 1) t = 2;
                        else t = 1;
                    }
                    k = max(k, minimax(i, j, n + 1, d, t, h1));
                    //cout << "k = " << k << "\n";
                    for (int i = 0; i < 10; i++){
                        for (int j = 0; j < 10; j++){
                            h1[i][j] = h[i][j];
                        }
                    }
                    if (n != d){
                        if (t == 1) t = 2;
                        else t = 1;
                    }
                }
            }
        }
        if (y == 0){
            if (t == 1) t = 2;
            else t = 1;
            k = max(k, minimax(a, b, n + 1, d, t, h1));
        }
        return -k;
    }
}




/*void Game(int t){
    int k = 0;
    if (t == 1) cout << a << b;
    while (true){
        if ()
    }
}*/

int main(){
    char h[10][10];
    char s1;
    int n, b1 = 0, w1 = 0, k = -3, a, b, t, x = 0, z = 0, i1, j1, f;
    //cin >> n;
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            h[i][j] = '.';
        }
    }

    h[4][4] = 'w';
    h[5][5] = 'w';
    h[4][5] = 'b';
    h[5][4] = 'b';

    cin >> n;
    if (n == 1){
        t = 1;
        while (true){
            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++){
                    if (Pos(i, j, t, h)){
                        z = 1;
                        f = minimax(i, j, 1, 4, t, h);
                        if (f > k){
                            k = f;
                            i1 = i;
                            j1 = j;
                        }
                    }
                }
            }
            k = -3;
            if (z == 0){
                cout << "Skip" << endl;
                x += 1;
                if (x == 2) break;
                t = 2;
            }
            else{
                Position(i1, j1, t, h);
                x = 0;
                z = 0;
                if (i1 == 1) cout << 'a';
                if (i1 == 2) cout << 'b';
                if (i1 == 3) cout << 'c';
                if (i1 == 4) cout << 'd';
                if (i1 == 5) cout << 'e';
                if (i1 == 6) cout << 'f';
                if (i1 == 7) cout << 'g';
                if (i1 == 8) cout << 'h';
                cout << j1 << "\n";
                t = 2;
            }

            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++){
                    if (Pos(i, j, t, h)) z = 1;
                }
            }
            if (z == 0){
                cout << "Skip" << endl;
                x += 1;
                if (x == 2) break;
                t = 1;
            }
            else{
                cin >> s1 >> b;
                if (s1 == 'a') a = 1;
                if (s1 == 'b') a = 2;
                if (s1 == 'c') a = 3;
                if (s1 == 'd') a = 4;
                if (s1 == 'e') a = 5;
                if (s1 == 'f') a = 6;
                if (s1 == 'g') a = 7;
                if (s1 == 'h') a = 8;
                Position(a, b, t, h);
                t = 1;
                x = 0;
                z = 0;
            }
        }
    }

    if (n == 2){
        t = 2;
        while (true){
            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++){
                    if (Pos(i, j, t, h)) z = 1;
                }
            }
            if (z == 0){
                cout << "Skip" << endl;
                x += 1;
                if (x == 2) break;
                t = 1;
            }
            else{
                cin >> s1 >> b;
                if (s1 == 'a') a = 1;
                if (s1 == 'b') a = 2;
                if (s1 == 'c') a = 3;
                if (s1 == 'd') a = 4;
                if (s1 == 'e') a = 5;
                if (s1 == 'f') a = 6;
                if (s1 == 'g') a = 7;
                if (s1 == 'h') a = 8;
                Position(a, b, t, h);
                t = 1;
                x = 0;
                z = 0;
            }
            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++){
                    if (Pos(i, j, t, h)){
                        z = 1;
                        f = minimax(i, j, 1, 4, t, h);
                        if (f > k){
                            k = f;
                            i1 = i;
                            j1 = j;
                        }
                    }
                }
            }
            k = -3;
            if (z == 0){
                cout << "Skip" << endl;
                x += 1;
                if (x == 2) break;
                t = 2;
            }
            else{
                Position(i1, j1, t, h);
                x = 0;
                z = 0;
                if (i1 == 1) cout << 'a';
                if (i1 == 2) cout << 'b';
                if (i1 == 3) cout << 'c';
                if (i1 == 4) cout << 'd';
                if (i1 == 5) cout << 'e';
                if (i1 == 6) cout << 'f';
                if (i1 == 7) cout << 'g';
                if (i1 == 8) cout << 'h';
                cout << j1 << "\n";
                t = 2;
            }
        }
    }
    return 0;
}
