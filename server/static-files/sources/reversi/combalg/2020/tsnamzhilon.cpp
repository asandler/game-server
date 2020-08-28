#include <bits/stdc++.h>
#include <chrono>
#include <deque>

using namespace std;

void mechanika(vector<vector<short> >&board, short hrz, short vrt, short color)
{
    /// Горизонтали
    for(short i=hrz+1;i<8;i++){
        if(board[i][vrt] != 0){
            if(board[i][vrt] == color){
                for(short j=hrz+1;j<i;j++){
                    board[j][vrt]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    for(short i=hrz-1;i>=0;i--){
        if(board[i][vrt] != 0){
            if(board[i][vrt] == color){
                for(short j=hrz-1;j>i;j--){
                    board[j][vrt]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    /// Вертикали
    for(short i=vrt+1;i<8;i++){
        if(board[hrz][i] != 0){
            if(board[hrz][i] == color){
                for(short j=vrt+1;j<i;j++){
                    board[hrz][j]=color;
                }
                break;
            }
        }
        else{
            break;
        }
    }
    for(short i=vrt-1;i>=0;i--){
        if(board[hrz][i] != 0){
            if(board[hrz][i] == color){
                for(short j=vrt-1;j>i;j--){
                    board[hrz][j]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    /// Диагонали
    for(short i=1; i+vrt < 8 && i+hrz < 8;i++){
        if(board[hrz+i][vrt+i] != 0){
            if(board[hrz+i][vrt+i] == color){
                for(short j=1;j<i;j++){
                    board[hrz+j][vrt+j]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    for(short i=1; vrt-i >= 0 && i+hrz < 8;i++){
        if(board[hrz+i][vrt-i] != 0){
            if(board[hrz+i][vrt-i] == color){
                for(short j=1;j<i;j++){
                    board[hrz+j][vrt-j]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    for(short i=1; i+vrt < 8 && hrz-i >= 0;i++){
        if(board[hrz-i][vrt+i] != 0){
            if(board[hrz-i][vrt+i] == color){
                for(short j=1;j<i;j++){
                    board[hrz-j][vrt+j]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
    for(short i=1; vrt-i >= 0 && hrz-i >= 0;i++){
        if(board[hrz-i][vrt-i] != 0){
            if(board[hrz-i][vrt-i] == color){
                for(short j=1;j<i;j++){
                    board[hrz-j][vrt-j]=color;  //пометка
                }
                break;
            }
        }
        else{
            break;
        }
    }
}






void find_legal(vector<vector<short> > board, deque<short>&legal_moves_vrt, deque<short>&legal_moves_hrz, short hrz, short vrt, short my_color, short enemy_color)
{
    /// Горизонтали
    bool YoN=0;
    for(short i=hrz+1;i<8;i++){
        if(board[i][vrt] != 0){
            if(board[i][vrt] == my_color){
                if(i != hrz+1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    for(short i=hrz-1;i>=0;i--){
        if(board[i][vrt] != 0){
            if(board[i][vrt] == my_color){
                if(i != hrz-1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    /// Вертикали
    for(short i=vrt+1;i<8;i++){
        if(board[hrz][i] != 0){
            if(board[hrz][i] == my_color){
                if(i != vrt+1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    for(short i=vrt-1;i>=0;i--){
        if(board[hrz][i] != 0){
            if(board[hrz][i] == my_color){
                if(i != vrt-1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    /// Диагонали
    for(short i=1; i+vrt < 8 && i+hrz < 8;i++){
        if(board[hrz+i][vrt+i] != 0){
            if(board[hrz+i][vrt+i] == my_color){
                if(i != 1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    for(short i=1; vrt-i >= 0 && i+hrz < 8;i++){
        if(board[hrz+i][vrt-i] != 0){
            if(board[hrz+i][vrt-i] == my_color){
                if(i != 1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    for(short i=1; i+vrt < 8 && hrz-i >= 0;i++){
        if(board[hrz-i][vrt+i] != 0){
            if(board[hrz-i][vrt+i] == my_color){
                if(i != 1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    for(short i=1; vrt-i >= 0 && hrz-i >= 0;i++){
        if(board[hrz-i][vrt-i] != 0){
            if(board[hrz-i][vrt-i] == my_color){
                if(i != 1){
                    YoN=1;
                }
                else{
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    if(YoN == 1){
        legal_moves_hrz.push_back(hrz);
        legal_moves_vrt.push_back(vrt);
    }
}
///Рандомизатор
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 gen(seed);
static uniform_real_distribution<double> coin(0,1);

/*
short dvizhok(const int n){
    switch (n){
        case 1: return 1; break;
        case 2: return 2; break;
        case 3: return 3; break;
      //  case 4: cout<<"dvizhok 4"<<endl; break;
        default: cout<<"not 1,2,3"<<endl; break;
    }
}
*/
int get_random_sector_according_to_v(const vector<double>&v){
    double p=coin(gen); // бросили монетку -- получили равномерно распределенное в [0,1] случайное число
    double q=0;
    int i=0;
    while(q<p){q+=v[i];++i;}
    return i;
}

///Движки
unsigned int random_comp(unsigned int n)
{
    n*=n;
    n=n+61;
    n=n*n;
    n=n-785;
    return n;
}

unsigned short greed(deque<short> legal_moves_hrz, deque<short> legal_moves_vrt, vector<vector<short> > board,short my_color)
{
    short maxnum=0;
    unsigned short i_memory=0;
    for(short i=0;i<legal_moves_hrz.size();i++){
        short num_of_check=0;
        vector<vector<short> > simul_board(8,vector<short> (8));
        for(short k=0;k<8;k++){
            for(short j=0;j<8;j++){
                simul_board[k][j]=board[k+1][j+1];
            }
        }
        simul_board[legal_moves_hrz[i]][legal_moves_vrt[i]]=my_color;
        mechanika(simul_board,legal_moves_hrz[i],legal_moves_vrt[i],my_color);
        for(short k=0;k<8;k++){
            for(short j=0;j<8;j++){
                if(simul_board[k][j] == my_color){
                    num_of_check++;
                }
            }
        }
        if(maxnum < num_of_check){
            maxnum=num_of_check;
            i_memory=i;
        }
    }
    return i_memory;
}

unsigned short ungreed(deque<short> legal_moves_hrz, deque<short> legal_moves_vrt, vector<vector<short> > board,short my_color)
{
    short minnum=1000;
    unsigned short i_memory=0;
    for(short i=0;i<legal_moves_hrz.size();i++){
        short num_of_check=0;
        vector<vector<short> > simul_board(8,vector<short> (8));
        for(short k=0;k<8;k++){
            for(short j=0;j<8;j++){
                simul_board[k][j]=board[k+1][j+1];
            }
        }
        simul_board[legal_moves_hrz[i]][legal_moves_vrt[i]]=my_color;
        mechanika(simul_board,legal_moves_hrz[i],legal_moves_vrt[i],my_color);
        for(short k=0;k<8;k++){
            for(short j=0;j<8;j++){
                if(simul_board[k][j] == my_color){
                    num_of_check++;
                }
            }
        }
        if(minnum >= num_of_check){
            minnum=num_of_check;
            i_memory=i;
        }
    }
    return i_memory;
}
/**
short analys(deque<short> legal_moves_hzr, deque<short> legal_moves_vrt, vector<vector<short> > board, short my_color, short enemy_color,short depth)
{
    short max_depth = 6;
    short scope=0,WoL=0;
    for(short i=0;i<legal_moves_hzr.size();i++){
        vector<vector<short> > simul_board(8,vector<short> (8));
        for(short i=0;i<8;i++){
            for(short j=0;j<8;j++){
                simul_board[i][j]=board[i+1][j+1];
            }
        }
        simul_board[legal_moves_hzr[i]][legal_moves_vrt[i]]=my_color;
        mechanika(simul_board,legal_moves_hzr[i],legal_moves_vrt[i],my_color);
        for(short i=0;i<8;i++){
            for(short j=0;j<8;j++){
                if(simul_board[i][j] == my_color){
                    scope++;
                }
                if(simul_board[i][j] == enemy_color){
                    scope--;
                }
            }
        }
        if(scope > 0){
            WoL++;
        }
        if(scope < 0){
            WoL--;
        }
        if(depth < max_depth){
            deque<short> simul_h;
            deque<short> simul_v;
            for(short k=0;k<8;k++){
                for(short t=0;t<8;t++){
                    if(simul_board[k][t] == 0){
                        find_legal(simul_board,simul_v,simul_h,k,t,enemy_color,my_color);
                    }
                }
            }
            WoL+=analys(simul_h,simul_v,simul_board,enemy_color,my_color,depth+1);
        }
    }
    return WoL; /// Допроверить analys + написать к нему smart
}
**/
int main()
{
    vector<vector<short> > board(8, vector<short> (8));
    deque<short> legal_moves_vrt;
    deque<short> legal_moves_hrz;
    for(short i=0;i<8;i++){
        for(short j=0;j<8;j++){
            board[i][j]=0; /// board [Горизонталь] [Вертикаль]
        }
    }
    board[3][3]=5;//
    board[4][4]=5;//                 Начальная расстановка
    board[4][3]=9;//               5 - white          5 9
    board[3][4]=9;//               9 - black          9 5
    short first_move,my_color,enemy_color,num_of_check=2;
    string enemy_move;
    cin>>first_move;
    if(first_move == 1){
        my_color=9;
        enemy_color=5;
        ///
    }
    if(first_move == 2){
        my_color=5;
        enemy_color=9;
        cin>>enemy_move;
        short vrt=enemy_move[0]-'0',hrz=enemy_move[1]-'0';
        vrt=vrt-49;
        hrz--;
        board[hrz][vrt]=enemy_color;
        mechanika(board,hrz,vrt,enemy_color);
        num_of_check=1;
    }
    for(short i=0;i<8;i++){
        for(short j=0;j<8;j++){
            if(board[i][j] == 0){
                find_legal(board,legal_moves_vrt,legal_moves_hrz,i,j,my_color,enemy_color);
            }
        }
    }
    /**for(short i=0;i<legal_moves_hrz.size();i++){
        char var2 = 'a' + legal_moves_vrt[i];
        cout<< var2 << legal_moves_hrz[i]+1 << endl;
    }
    cout<<endl;**/
    unsigned short n=171;
    /// Основа
    vector<double> v1={1,0,0};            ///вероятность
    short all_check_num=4;
    while(1){
        num_of_check=0;
        for(short i=0;i<8;i++){
            for(short j=0;j<8;j++){
                if(board[i][j] == my_color){
                    num_of_check++;
                }
            }
        }
        if(legal_moves_hrz.size() == 0){
            cout<<"Skip"<<endl;
        }
        else{
            int k1=get_random_sector_according_to_v(v1);
            char var='0';
            unsigned short t=0;
            ///cout<<endl<<"==="<<endl<<k1<<endl<<"==="<<endl;



            if(k1 == 1){
                n=random_comp(n);
                t=n % legal_moves_hrz.size();
                var = 'a' + legal_moves_vrt[t];
            }
            if(k1 == 2){
                t=greed(legal_moves_hrz,legal_moves_vrt,board,my_color);        ///Проблебмное место !!!!!!
                cout<<endl<<"prov 1"<<endl;                                     ///Весь кусок
                var = 'a' + legal_moves_vrt[t];
            }
            if(k1 == 3){
                t=ungreed(legal_moves_hrz,legal_moves_vrt,board,my_color);
                cout<<endl<<"prov 2"<<endl;
                var = 'a' + legal_moves_vrt[t];
            }



            cout<<var<<legal_moves_hrz[t]+1<<endl;
            board[legal_moves_hrz[t]][legal_moves_vrt[t]]=my_color;
            mechanika(board,legal_moves_hrz[t],legal_moves_vrt[t],my_color);
            legal_moves_hrz.clear();
            legal_moves_vrt.clear();
            all_check_num++;
            /**for(int i=0;i<8;i++){
                for(int j=0;j<8;j++){
                    cout<<board[i][j]<<" ";
                }
                cout<<endl;
            }**/
        }
        ///cout<<endl;
        cin>>enemy_move;
        if(enemy_move != "Skip"){
            short vrt=enemy_move[0]-'0',hrz=enemy_move[1]-'0';
            vrt=vrt-49;
            hrz--;
            board[hrz][vrt]=enemy_color;
            mechanika(board,hrz,vrt,enemy_color);
        }
        for(short i=0;i<8;i++){
            for(short j=0;j<8;j++){
                if(board[i][j] == 0){
                    find_legal(board,legal_moves_vrt,legal_moves_hrz,i,j,my_color,enemy_color);
                }
            }
        }
        /**for(short i=0;i<legal_moves_hrz.size();i++){
            char var2 = 'a' + legal_moves_vrt[i];
            cout<< var2 << legal_moves_hrz[i]+1 << endl;
        }**/
    }
}
