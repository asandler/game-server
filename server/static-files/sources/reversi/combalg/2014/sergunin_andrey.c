#include <stdio.h>
#include <stdlib.h>

#define INF 10000000

void print_board(int *board[8][8])
{
    int i, j;
    printf(" 12345678\n");
    for (i = 0; i < 8; i++)
    {
        printf("%c", i + 'a');
        for (j = 0; j < 8; j++)
            printf("%c", board[i][j]);
        printf("\n");
    }
}

int check_move(int x, int y, int dx, int dy, int *board[8][8], char color)
{
    char color_opp;
    if (color == '@')
        color_opp = 'O';
    else
        color_opp = '@';

    int a = x + dx;
    int b = y + dy;
    while (a >= 0 && a < 8 && b >= 0 && b < 8 && board[a][b] == color_opp)
    {
        a += dx;
        b += dy;
    }
    if (a >= 0 && a < 8 && b >= 0 && b < 8 && board[a][b] == color && (x + dx != a || y + dy != b))
        return 1;
    else
        return 0;
}

void search_turn (int *turn[], char color, int *board[8][8], int *q_turn)
{
    int i, j;
    int k = 0;

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            if (board[i][j] == '.')
            {
                int flag;
                flag = check_move(i, j, 0, 1, board, color) || check_move(i, j, 1, 0, board, color) ||
                check_move(i, j, -1, 0, board, color) || check_move(i, j, 0, -1, board, color) ||
                check_move(i, j, 1, 1, board, color) || check_move(i, j, 1, -1, board, color) ||
                check_move(i, j, -1, 1, board, color) || check_move(i, j, -1, -1, board, color);
                if (flag)
                    turn[k++] = i * 8 + j;
            }
        }
    *q_turn = k;
}

void update_line(int *board[8][8], int x, int y, int dx, int dy, char color)
{
    if (check_move(x,y, dx, dy, board, color))
    {
        int color_opp;
        if (color == '@')
            color_opp = 'O';
        else
            color_opp = '@';
        int a = x + dx;
        int b = y + dy;
        while (a >= 0 && a < 8 && b >= 0 && b < 8 && board[a][b] == color_opp)
        {
            board[a][b] = color;
            a += dx;
            b += dy;
        }
    }
}

void update_board (int *new_board[8][8], int *board[8][8], int position, char color)
{
    int x, y, i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            new_board[i][j] = board[i][j];
    x = position / 8;
    y = position % 8;
    new_board[x][y] = color;
    update_line(new_board, x, y, 0, 1, color);
    update_line(new_board, x, y, 0, -1, color);
    update_line(new_board, x, y, 1, 0, color);
    update_line(new_board, x, y, -1, 0, color);
    update_line(new_board, x, y, 1, 1, color);
    update_line(new_board, x, y, 1, -1, color);
    update_line(new_board, x, y, -1, 1, color);
    update_line(new_board, x, y, -1, -1, color);
}

int check_position (int *board[8][8])
{
    int i, j;
    int a, b;
    a = 3;
    b = 15;
    int result = 0;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            if (board[i][j] == '@')
            {
                result++;
                if ((i == 0 || board[i - 1][j] != '.') &&
                    (i == 7 || board[i + 1][j] != '.') &&
                    (j == 0 || board[i][j - 1] != '.') &&
                    (j == 7 || board[i][j + 1] != '.'))
                        result += a;
            }

            if (board[i][j] == 'O')
            {
                result--;
                if ((i == 0 || board[i - 1][j] != '.') &&
                    (i == 7 || board[i + 1][j] != '.') &&
                    (j == 0 || board[i][j - 1] != '.') &&
                    (j == 7 || board[i][j + 1] != '.'))
                        result -= a;
            }

        }

    if (board[0][0] = '@')
        result += b;

    if (board[0][7] = '@')
        result += b;

    if (board[7][0] = '@')
        result += b;

    if (board[7][7] = '@')
        result += b;

    if (board[0][0] = 'O')
        result -= b;

    if (board[0][7] = 'O')
        result -= b;

    if (board[7][0] = 'O')
        result -= b;

    if (board[7][7] = 'O')
        result -= b;
    return result;
}

int minimax(int *board[8][8], int *next_turn, char color, int depth)
{
    char color_opp;
    if (color == '@')
        color_opp = 'O';
    else
        color_opp = '@';
    int i, j, k;
    int turn[64];

    int best;
    if (color == '@')
        best = -INF;
    else
        best = INF;
    int best_turn = -1;

    if (depth > 0)
    {
        search_turn(turn, color, board, &k);
        if (k != 0)
        {
            for (i = 0; i < k; i++)
            {
                int new_board[8][8];
                int a = 0;

                update_board(new_board, board, turn[i], color);

                int current = minimax(new_board, &a, color_opp, depth - 1);
                if (color == '@')
                {
                    if (current > best)
                    {
                        best = current;
                        best_turn = turn[i];
                    }
                }
                else
                {
                    if (current < best)
                    {
                        best = current;
                        best_turn = turn[i];
                    }
                }
            }
        }
        else
        {
            int  a = 0;
            best = minimax(board, &a, color_opp, depth - 1);
            best_turn = -1;
        }
    }
    else
    {
        best = check_position(board);
    }

    *next_turn = best_turn;
    return best;
}

int alphabeta(int *board[8][8], int *next_turn, char color, int depth, int border)
{
    char color_opp;
    if (color == '@')
        color_opp = 'O';
    else
        color_opp = '@';
    int i, j, k;
    int turn[64];
    int best_turn = -1;
    int new_border;

    if (color == '@')
        new_border = -INF;
    else
        new_border = INF;

    if (depth > 0)
    {
        search_turn(turn, color, board, &k);
        if (k != 0)
        {
            for (i = 0; i < k; i++)
            {
                int new_board[8][8];
                int a = 0;

                update_board(new_board, board, turn[i], color);

                int current = alphabeta(new_board, &a, color_opp, depth - 1, new_border);
                if (current != 2 * INF)
                {
                     if (color == '@')
                    {
                        if (current < border)
                        {
                            if (new_border < current)
                            {
                                new_border = current;
                                best_turn = turn[i];
                            }
                        }
                        else
                            return 2 * INF;
                    }
                    else
                    {
                        if (current > border)
                        {
                            if (new_border > current)
                            {
                                new_border = current;
                                best_turn = turn[i];
                            }
                        }
                        else
                            return 2 * INF;
                    }
                }
            }
            *next_turn = best_turn;
            return new_border;
        }
        else
        {
            int  a = 0;
            *next_turn = -1;
            return (alphabeta(board, &a, color_opp, depth - 1, new_border));
        }
    }
    else
    {
        return(check_position(board));
    }
}

int end_of_game(int *board[8][8])
{
    int b = 0, w = 0;
    int k, l;
    int turn[64];
    search_turn(turn, '@', board, &k);
    search_turn(turn, 'O', board, &l);
    if (k + l == 0)
        return 0;
    else
        return 1;
}

void minimax_turn(int *board[8][8], char color, int depth)
{
    int next_turn;
    int best;
    best = minimax(board, &next_turn, color, depth);
    if (next_turn == - 1)
    {
        //printf("Skip\n");
    }
    else
    {
        //printf("%c%d\n", next_turn / 8 + 'a', next_turn % 8 + 1);
        update_board(board, board, next_turn, color);
    }
}

void alphabeta_turn(int *board[8][8], char color, int depth)
{
    int next_turn;
    int best;
    int t;
    if (color == '@')
        t = INF;
    else
        t = -INF;
    best = alphabeta(board, &next_turn, color, depth, t);
    //printf("%d\n", next_turn);
    //printf("%d\n",best);
    if (next_turn == - 1)
    {
        printf("Skip\n");
        fflush(stdout);
    }
    else
    {
        printf("%c%d\n", next_turn / 8 + 'a', next_turn % 8 + 1);
        fflush(stdout);
        update_board(board, board, next_turn, color);
    }
}

void player_turn(int *board[8][8], char color)
{
    int a = 0, b = 0;
    char s[5];
    scanf("%s", s);
    if (s[0] != 'S')
    {
        a = s[0];
        b = s[1];
        update_board(board, board, 8 * (a - 'a') + b - '1', color);
    }
}

void random_turn(int *board[8][8], char color)
{
        int turn[64];
        int k;
        search_turn(turn, color, board, &k);
        int next_turn;
        if (k != 0)
        {
            next_turn = turn[rand() % k];
            update_board(board, board, next_turn, color);
            //printf("%c%d\n", next_turn / 8 + 'a', next_turn % 8 + 1);
        }
        else
        {
            //printf("Skip\n");
        }
}

int check_winner(int *board[8][8])
{
    int i, j, k = 0;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            if (board[i][j] == '@')
                k++;
            if (board[i][j] == 'O')
                k--;
        }
    if (k > 0)
        return 1;
    if (k < 0)
        return -1;
    if (k == 0)
        return 0;
}

int make_new_board(int *board[8][8])
{
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            board[i][j] = '.';
    board[3][3] = 'O';
    board[3][4] = '@';
    board[4][3] = '@';
    board[4][4] = 'O';
}

int main()
{
    int depth = 5;
    char color, color_opp, c;
    char s[5];
    scanf("%s", s);
    if (s[0] == '1')
    {
        color = '@';
        color_opp = 'O';
    }
    else
    {
        color = 'O';
        color_opp = '@';
    }

    int board[8][8];

    int t, p1, p2, p0;
    p1 = p2 = p0 = 0;

    for (t = 0; t < 1; t++)
    {
        make_new_board(board);
        int q_turn = 0;
        if (color == 'O')
        {
            player_turn(board, color_opp);
            //print_board(board);
        }

        while (end_of_game(board))
        {
            alphabeta_turn(board, color, depth);
            //print_board(board);

            player_turn(board, color_opp);
            //print_board(board);
            q_turn++;
        }
        int winner;
        winner = check_winner(board);
        if (winner == 1)
            p1++;
        if (winner == -1)
            p2++;
        if (winner == 0)
            p0++;
        //printf("%d %d %d\n", p1, p2, p0);
        //printf("%d\n", q_turn);
    }

    return 0;
}
