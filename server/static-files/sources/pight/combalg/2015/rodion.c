#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define true 1
#define false 0

#define W 40
#define H 30
#define valid(x, y) (x >= 0 && y >= 0 && x < H && y < W && snake_matr[x][y] == 0)

_Bool snake_matr[H][W], field[H][W];

typedef struct {
    int X[500];
    int Y[500];
    int len;
    int dir;
} snake;

snake my_snake, en_snake, temp_snake;

void draw(int step) {
    int i, k, t;
  fprintf(stderr, "TURN: %d:\n", step);
    for (k = 0; k < H; ++k) {
        for (i = 0; i < W; i++) {
            t = field[k][i];

            if (!snake_matr[k][i]) {
                switch (t) {
                    case 0: fprintf(stderr, " "); break;
                    case 1: fprintf(stderr, ":"); break;
                }
            } else {
                if (k == my_snake.X[0] && i == my_snake.Y[0]) {
                    fprintf(stderr, "#");
                } else {
                    fprintf(stderr, "X");
                }
            }
        }
        fprintf(stderr, " %d\n", k);
    }
    for (k = 0; k < W+5; ++k) fprintf(stderr, "_");
    fprintf(stderr, " \n\n");
}

int def_neib(snake s, int way, int *x, int *y) {
    int fo[2], le[2], ri[2];
    int d = s.dir;

    *x = s.X[0];
    *y = s.Y[0];

    switch (d) {
        case 2:
            fo[0] = -1;
            fo[1] = 0;
            le[0] = 0;
            le[1] = -1;
            ri[0] = 0;
            ri[1] = 1;
            break;
        case 3:
            fo[0] = 0;
            fo[1] = 1;
            le[0] = -1;
            le[1] = 0;
            ri[0] = 1;
            ri[1] = 0;
            break;
        case 1:
            fo[0] = 0;
            fo[1] = -1;
            le[0] = 1;
            le[1] = 0;
            ri[0] = -1;
            ri[1] = 0;
            break;
        case 0:
            fo[0] = 1;
            fo[1] = 0;
            le[0] = 0;
            le[1] = 1;
            ri[0] = 0;
            ri[1] = -1;
            break;
    }
    switch (way) {
        case 'F':
            *x += fo[0];
            *y += fo[1];
            break;
        case 'L':
            *x += le[0];
            *y += le[1];
            d -= 1;
            break;
        case 'R':
            *x += ri[0];
            *y += ri[1];
            d += 1;
            break;
    }
    if (d > 3)
        d = 0;
    if (d < 0)
        d = 3;
    return d;
}

void move(snake *s, int x, int y) {
    int i;
    snake_matr[s->X[s->len - 1]][s->Y[s->len - 1]] = 0;

    if (field[s->X[s->len - 1]][s->Y[s->len - 1]]) {
        field[s->X[s->len - 1]][s->Y[s->len - 1]] = 0;
        snake_matr[s->X[s->len - 1]][s->Y[s->len - 1]] = 1;
        s->len = s->len + 1;
    }

    for (i = s->len - 1; i > 0; i--) {
        s->X[i] = s->X[i - 1];
        s->Y[i] = s->Y[i - 1];
    }

    s->X[0] = x;
    s->Y[0] = y;

    snake_matr[s->X[0]][s->Y[0]] = 1;
}

int create(int turn) {
    int i, k;
    char buf[100];

    en_snake.len = 4;
    my_snake.len = 4;

    for (k = 0; k < H; ++k) {
        for (i = 0; i < H; ++i) {
            snake_matr[k][i] = 0;
        }
    }

  if (turn == 1) {
    my_snake.dir = 0;
    en_snake.dir = 2;

    for (i = 3; i >= 0; --i) {
        my_snake.X[i] = 3 - i;
        my_snake.Y[i] = 0;
        snake_matr[3 - i][0] = 1;

        en_snake.X[3 - i] = H - i - 1;
        en_snake.Y[3 - i] = W - 1;
        snake_matr[H - i - 1][W - 1] = 1;
    }
  } else {
    en_snake.dir = 0;
    my_snake.dir = 2;

    for (i = 3; i >= 0; --i) {
        en_snake.X[i] = 3 - i;
        en_snake.Y[i] = 0;
        snake_matr[3 - i][0] = 1;

        my_snake.X[3 - i] = H - i - 1;
        my_snake.Y[3 - i] = W - 1;
        snake_matr[H - i - 1][W - 1] = 1;
    }
  }

    for (k = 0; k < H; ++k) {
        scanf("%s", buf);
        for (i = 0; i < W; ++i) {
            field[k][i] = (_Bool)(buf[i] - '0');
        }
    }

    return 0;
}

char select_way() {
    char my_way, ways[3], best_way;
    int temp = 1, x, y, i;

    my_way = 'F';
    ways[0] = 'F';
    ways[1] = 'L';
    ways[2] = 'R';

/*    do {
        temp = random() % 4;
        def_neib(my_snake, ways[temp], &x, &y);
        my_way = ways[temp % 3];
    } while (!valid(x, y));
*/
  for (i = 0; i < 3; ++i) {
    def_neib(my_snake, ways[i], &x, &y);
    if (valid(x,y) && temp) {
      my_way = ways[i];
      temp = 0;
    }
    if (field[x][y] && valid(x,y)) my_way = ways[i];
  }
    return my_way;
}

int main() {
    char en_way, my_way;
    int x, y;
    int turn;
    char buf[100];
    int step = 0;

    srand(time(NULL));

    scanf("%s", buf);
    turn = buf[0] - '0';

    create(turn);

    if (turn == 2) {
        scanf("%s", buf);
        en_way = buf[0];
        en_snake.dir = def_neib(en_snake, en_way, &x, &y);
        move(&en_snake, x, y);
    }
    while (true) {
      step++;
        my_way = select_way();

        my_snake.dir = def_neib(my_snake, my_way, &x, &y);
        move(&my_snake, x, y);
        draw(step);
        printf("%c\n", my_way);
        fflush(stdout);


        scanf("%s", buf);
        en_way = buf[0];
        en_snake.dir = def_neib(en_snake, en_way, &x, &y);
        move(&en_snake, x, y);

    }

    return 0;
}
