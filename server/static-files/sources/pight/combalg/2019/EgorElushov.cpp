#include <iostream>
#include<stdlib.h>
#include <vector>
#include <utility>
#include<math.h>
#include<time.h>
#define HIGH 30
#define WIDTH 40
using namespace std;

int color[1200];
char game_field[HIGH][WIDTH];
vector<pair<int, int> > apples, current_apples;

struct part_of_snake{
    pair<int, int> coords;
    bool is_appled;
    int direct, direct_of_head;
    /*
    1 - 'F'
    2 - 'R'
    3 - 'L'

    1 - 'U'
    2 - 'R'
    3 - 'L'
    4 - 'D'
    */
};

vector<part_of_snake> snake_1, snake_2, future_snake;
int turn = 0;
part_of_snake current_end;

void scan(){

    char n;
    for(int i = 0; i < HIGH; i++)
        for(int j = 0; j < WIDTH; j++){
            cin >> n;
            if(n - '0' == 1)
                apples.push_back(pair<int, int>(i, j));
        }
}

void change_field(){
    for(int i = 0; i < HIGH; i++)
        for(int j = 0; j < WIDTH; j++)
            game_field[i][j] = '0';
    for(unsigned i = 0; i < snake_1.size(); i++)
        game_field[snake_1[i].coords.first][snake_1[i].coords.second] = '2';

    for(unsigned i = 0; i < snake_2.size(); i++)
        game_field[snake_2[i].coords.first][snake_2[i].coords.second] = '2';
    for(unsigned i = 0; i < apples.size(); i++){
        game_field[apples[i].first][apples[i].second] = '1';
    }
    /*
    for(unsigned i = 0; i < HIGH; i++){
        for(int j = 0; j < WIDTH; j++)
            cout << game_field[i][j];
        cout << endl;
    }
    */
}

void init(){
    part_of_snake tmp;
    tmp.coords.first = 3;
    tmp.coords.second = 0;
    tmp.is_appled = false;
    tmp.direct = 1;
    tmp.direct_of_head = 4;
    snake_1.push_back(tmp);
    tmp.coords.first--;
    snake_1.push_back(tmp);
    tmp.coords.first--;
    snake_1.push_back(tmp);
    tmp.coords.first--;
    snake_1.push_back(tmp);
    tmp.coords.first = 26;
    tmp.coords.second = 39;
    tmp.direct_of_head = 1;
    snake_2.push_back(tmp);
    tmp.coords.first++;
    snake_2.push_back(tmp);
    tmp.coords.first++;
    snake_2.push_back(tmp);
    tmp.coords.first++;
    snake_2.push_back(tmp);
}

void future_move_Left(){
    current_end = future_snake[future_snake.size() - 1];
    for(unsigned i = future_snake.size() - 1; i > 0; i--){
        future_snake[i]  = future_snake[i - 1];
    }

    if(future_snake[0].direct_of_head == 3){
        future_snake[0].coords.first--;
        future_snake[0].direct_of_head = 1;
    }
    else if(future_snake[0].direct_of_head == 2){
        future_snake[0].coords.first++;
        future_snake[0].direct_of_head = 4;
    }
    else if(future_snake[0].direct_of_head == 1){
        future_snake[0].coords.second--;
        future_snake[0].direct_of_head = 2;
    }
    else{
        future_snake[0].coords.second++;
        future_snake[0].direct_of_head = 3;
    }
}

void move_Left(){
    if(turn == 1){
        current_end = snake_1[snake_1.size() - 1];
        for(unsigned i = snake_1.size() - 1; i > 0; i--){
            snake_1[i]  = snake_1[i - 1];
        }
        snake_1[0].is_appled = false;
        if(snake_1[0].direct_of_head == 3){
            snake_1[0].coords.first--;
            snake_1[0].direct_of_head = 1;
        }
        else if(snake_1[0].direct_of_head == 2){
            snake_1[0].coords.first++;
            snake_1[0].direct_of_head = 4;
        }
        else if(snake_1[0].direct_of_head == 1){
            snake_1[0].coords.second--;
            snake_1[0].direct_of_head = 2;
        }
        else{
            snake_1[0].coords.second++;
            snake_1[0].direct_of_head = 3;
        }
    }
    else{
        current_end = snake_2[snake_2.size() - 1];
        for(unsigned i = snake_2.size() - 1; i > 0; i--){
            snake_2[i]  = snake_2[i - 1];
        }
        snake_2[0].is_appled = false;
        if(snake_2[0].direct_of_head == 3){
            snake_2[0].coords.first--;
            snake_2[0].direct_of_head = 1;
        }
        else if(snake_2[0].direct_of_head == 2){
            snake_2[0].coords.first++;
            snake_2[0].direct_of_head = 4;
        }
        else if(snake_2[0].direct_of_head == 1){
            snake_2[0].coords.second--;
            snake_2[0].direct_of_head = 2;
        }
        else{
            snake_2[0].coords.second++;
            snake_2[0].direct_of_head = 3;
        }
    }
}

void future_move_Right(){
    current_end = future_snake[future_snake.size() - 1];
    for(unsigned i = future_snake.size() - 1; i > 0; i--){
        future_snake[i]  = future_snake[i - 1];
    }

    if(future_snake[0].direct_of_head == 3){
        future_snake[0].coords.first++;
        future_snake[0].direct_of_head = 4;
    }
    else if(future_snake[0].direct_of_head == 2){
        future_snake[0].coords.first--;
        future_snake[0].direct_of_head = 1;
    }
    else if(future_snake[0].direct_of_head == 1){
        future_snake[0].coords.second++;
        future_snake[0].direct_of_head = 3;
    }
    else{
        future_snake[0].coords.second--;
        future_snake[0].direct_of_head = 2;
    }
}

void move_Right(){
    if(turn == 1){
        current_end = snake_1[snake_1.size() - 1];
        for(unsigned i = snake_1.size() - 1; i > 0; i--){
            snake_1[i]  = snake_1[i - 1];
        }
        snake_1[0].is_appled = false;
        if(snake_1[0].direct_of_head == 3){
            snake_1[0].coords.first++;
            snake_1[0].direct_of_head = 4;
        }
        else if(snake_1[0].direct_of_head == 2){
            snake_1[0].coords.first--;
            snake_1[0].direct_of_head = 1;
        }
        else if(snake_1[0].direct_of_head == 1){
            snake_1[0].coords.second++;
            snake_1[0].direct_of_head = 3;
        }
        else{
            snake_1[0].coords.second--;
            snake_1[0].direct_of_head = 2;
        }
    }
    else{
        current_end = snake_2[snake_2.size() - 1];
        for(unsigned i = snake_2.size() - 1; i > 0; i--){
            snake_2[i]  = snake_2[i - 1];
        }
        snake_2[0].is_appled = false;
        if(snake_2[0].direct_of_head == 3){
            snake_2[0].coords.first++;
            snake_2[0].direct_of_head = 4;
        }
        else if(snake_2[0].direct_of_head == 2){
            snake_2[0].coords.first--;
            snake_2[0].direct_of_head = 1;
        }
        else if(snake_2[0].direct_of_head == 1){
            snake_2[0].coords.second++;
            snake_2[0].direct_of_head = 3;
        }
        else{
            snake_2[0].coords.second--;
            snake_2[0].direct_of_head = 2;
        }
    }
}

void future_move_Forward(){
    current_end = future_snake[future_snake.size() - 1];
    for(unsigned i = future_snake.size() - 1; i > 0; i--){
        future_snake[i]  = future_snake[i - 1];
    }

    if(future_snake[0].direct_of_head == 3){
        future_snake[0].coords.second++;
    }
    else if(future_snake[0].direct_of_head == 2){
        future_snake[0].coords.second--;
    }
    else if(future_snake[0].direct_of_head == 1){
        future_snake[0].coords.first--;
    }
    else{
        future_snake[0].coords.first++;
    }
}

void move_Forward(){
    if(turn == 1){
        current_end = snake_1[snake_1.size() - 1];
        for(unsigned i = snake_1.size() - 1; i > 0; i--){
            snake_1[i]  = snake_1[i - 1];
        }
        snake_1[0].is_appled = false;
        if(snake_1[0].direct_of_head == 3){
            snake_1[0].coords.second++;
        }
        else if(snake_1[0].direct_of_head == 2){
            snake_1[0].coords.second--;
        }
        else if(snake_1[0].direct_of_head == 1){
            snake_1[0].coords.first--;
        }
        else{
            snake_1[0].coords.first++;
        }
    }
    else{
        current_end = snake_2[snake_2.size() - 1];
        for(unsigned i = snake_2.size() - 1; i > 0; i--){
            snake_2[i]  = snake_2[i - 1];
        }
        snake_2[0].is_appled = false;
        if(snake_2[0].direct_of_head == 3){
            snake_2[0].coords.second++;
        }
        else if(snake_2[0].direct_of_head == 2){
            snake_2[0].coords.second--;
        }
        else if(snake_2[0].direct_of_head == 1){
            snake_2[0].coords.first--;
        }
        else{
            snake_2[0].coords.first++;
        }
    }
}

bool is_appled(){
    if(turn == 1){
        bool tmp = snake_1[snake_1.size() - 1].is_appled;
        snake_1[snake_1.size() - 1].is_appled = false;
        if(tmp){
            for(unsigned i = 0; i < apples.size(); i++){
                if(snake_1[snake_1.size() - 1].coords == apples[i])
                    apples.erase(apples.begin() + i, apples.begin() + i + 1);
            }
        }
        return tmp;
    }
    bool tmp = snake_2[snake_2.size() - 1].is_appled;
    snake_2[snake_2.size() - 1].is_appled = false;
    if(tmp){
        for(unsigned i = 0; i < apples.size(); i++){
            if(snake_2[snake_2.size() - 1].coords == apples[i])
                apples.erase(apples.begin() + i, apples.begin() + i + 1);
            }
    }
    return tmp;
}

unsigned nearest_apple(){
    double min = 60, min_i = 0;
    if(turn == 1){
        for(unsigned i = 0; i < apples.size(); i++){
            if(sqrt((apples[i].first - snake_1[0].coords.first) * (apples[i].first - snake_1[0].coords.first) + (apples[i].second - snake_1[0].coords.second) * (apples[i].second - snake_1[0].coords.second)) < min){
                min = sqrt((apples[i].first - snake_1[0].coords.first) * (apples[i].first - snake_1[0].coords.first) + (apples[i].second - snake_1[0].coords.second) * (apples[i].second - snake_1[0].coords.second));
                min_i = i;
            }
        }
    }
    else{
         for(unsigned i = 0; i < apples.size(); i++){
            for(unsigned j = 0; j < current_apples.size(); j++){
                if(current_apples[j] == apples[i]){
                    i++;
                    break;
                }
            }
            if(sqrt((apples[i].first - snake_2[0].coords.first) * (apples[i].first - snake_2[0].coords.first) + (apples[i].second - snake_2[0].coords.second) * (apples[i].second - snake_2[0].coords.second)) < min){
                min = sqrt((apples[i].first - snake_2[0].coords.first) * (apples[i].first - snake_2[0].coords.first) + (apples[i].second - snake_2[0].coords.second) * (apples[i].second - snake_2[0].coords.second));
                min_i = i;
            }
        }
    }
    current_apples.push_back(apples[min_i]);
    return min_i;
}

void eat_apple();

void move(char direct, int a){
    if(a == 0){
        bool tmp = is_appled();
        if(direct == 'L'){
            move_Left();
        }
        else if(direct == 'F'){
            move_Forward();
        }
        else if(direct == 'R'){
            move_Right();
        }
        eat_apple();
        current_end.is_appled = false;
        if(tmp){
            if(turn == 1)
                snake_1.push_back(current_end);
            else
                snake_2.push_back(current_end);
        }

        change_field();
    }
    else{
        bool tmp = future_snake[future_snake.size() - 1].is_appled;
        if(direct == 'L'){
            future_move_Left();
        }
        else if(direct == 'F'){
            future_move_Forward();
        }
        else if(direct == 'R'){
            future_move_Right();
        }
        if(tmp)
            future_snake.push_back(current_end);
    }
}

char go_to_apple(){
    vector<char> letter;
    letter.push_back('F');
    letter.push_back('L');
    letter.push_back('R');
    int n = -1;
    int t = turn;
    if(t == 1)
        future_snake = snake_1;
    else
        future_snake = snake_2;
    //cout << "FUTURE SNAKE COORDS: " << future_snake[0].coords.first << " : " << future_snake[0].coords.second <<endl;
    n = rand() % 3;
    move(letter[n], 1);
    //cout << "FUTURE SNAKE COORDS: " << future_snake[0].coords.first << " : " << future_snake[0].coords.second <<endl << "TURN : " << letter[n] << endl;
    if(future_snake[0].coords.first < 0 || future_snake[0].coords.first > 29 || future_snake[0].coords.second > 39 || future_snake[0].coords.second < 0)
        letter.erase(letter.begin() + n, letter.begin() + n + 1);
    if(turn == 2 && future_snake[0].coords == snake_1[0].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
    if(turn == 1 && future_snake[0].coords == snake_2[0].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
    for(unsigned i = 1; i < future_snake.size(); i++){
        if(future_snake[0].coords == future_snake[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
        if(turn == 1 && future_snake[0].coords == snake_2[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
        if(turn == 2 && future_snake[0].coords == snake_1[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
    }
    if(letter.size() == 3)
        return letter[n];
    if(t == 1)
        future_snake = snake_1;
    else
        future_snake = snake_2;
    n = rand() % 2;
    move(letter[n], 1);
    //for(int i = 0; i < letter.size(); i++){
    //   cout <<"LETTER "  << letter[i] << endl;
    //}
    //cout << "FUTURE SNAKE COORDS: " << future_snake[0].coords.first << " : " << future_snake[0].coords.second <<endl << "TURN : " << letter[n] << endl;;
    if(future_snake[0].coords.first < 0 || future_snake[0].coords.first > 29 || future_snake[0].coords.second > 39 || future_snake[0].coords.second < 0)
        letter.erase(letter.begin() + n, letter.begin() + n + 1);
    /*for(int i = 0; turn == 2 && i < snake_1.size(); i++)
        if(future_snake[0].coords == snake_1[i].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
    for(int i = 0; turn == 1 && i < snake_2.size(); i++)
        if(future_snake[0].coords == snake_2[i].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
        */
    if(turn == 2 && future_snake[0].coords == snake_1[0].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
    if(turn == 1 && future_snake[0].coords == snake_2[0].coords)
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
    for(unsigned i = 1; i < future_snake.size(); i++){
        if(future_snake[0].coords == future_snake[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
        if(turn == 1 && future_snake[0].coords == snake_2[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
        if(turn == 2 && future_snake[0].coords == snake_1[i].coords){
            letter.erase(letter.begin() + n, letter.begin() + n + 1);
            break;
        }
    }
    if(letter.size() == 2)
        return letter[n];
    return letter[0];
}

void eat_apple(){
    if(turn == 1){
        for(unsigned i = 0; i < apples.size(); i++){
            if(snake_1[0].coords == apples[i])
                snake_1[0].is_appled = true;
        }
    }
    else{
        for(unsigned i = 0; i < apples.size(); i++){
            if(snake_2[0].coords == apples[i])
                snake_2[0].is_appled = true;
        }
    }
}



int main(){
    srand(time(NULL));
    cin >> turn;

    scan();
    init();
    for(unsigned i = 0; i < 600; i++){
        cout << 'L' << endl << 'L' << endl << 'F' << endl << 'F' <<endl;
        /*if(turn == 2){
            turn = 1;
            char enemy_turn;
            cin >> enemy_turn;
            move(enemy_turn, 0);
            turn = 2;
            char your_turn = go_to_apple();
            move(your_turn, 0);
            cout << your_turn << endl;

        }
        else{
            turn = 1;
            char your_turn = go_to_apple();
            move(your_turn, 0);
            cout << your_turn << endl;
            char enemy_turn;
            turn = 2;
            cin >> enemy_turn;
            move(enemy_turn, 0);
            turn = 1;
        }
        change_field();*/
    }
    return 0;
}
