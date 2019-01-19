#include <assert.h>
#include <string.h>

#include <string>
#include <iostream>

#define BOARD_SIZE 10
#define SIZE (BOARD_SIZE + 2)

#define SHIPS_NUM 10
#define SHOTS_NUM 100

#define CFG_SIZE (5 * SHIPS_NUM - 1)

#define FIGHTING 1
#define WOUNDED 2
#define CRUSHED 4

#define MINE -1
#define EMPTY 0
#define DECK 1
#define FOUL 2
#define DAMAGE 3

using namespace std;

struct pos_t {
    int beg_row;
    int beg_col;
    int end_row;
    int end_col;
};

struct game_state {
    int mv_num;
    struct {
        char last_move[8];

        int board[SIZE][SIZE];

        struct pos_t ships[SHIPS_NUM];
        int ship_state[SHIPS_NUM];

        int shots_num;
    } player[2];
};

int get_decks_cnt(int board[SIZE][SIZE]) {
    int cnt = 0;

    for (int row = 1; row <= BOARD_SIZE; row++) {
        for (int col = 1; col <= BOARD_SIZE; col++) {
            if (board[row][col] == DECK) {
                ++cnt;
            }
        }
    }
    return cnt;
}

bool move_format_correct(const string& mv) {
    const string directions = "NESWX";

    size_t len = mv.length();

    if (len != 4 && len != 7) {
        cout << "i" << "Incorrect move format" << endl;
        return false;
    }

    if (mv[0] < '0' || mv[0] > '9' || directions.find(mv[1]) == string::npos || mv[2] != ' ') {
        cout << "i" << "Incorrect move format" << endl;
        return false;
    }

    if ((len == 4 && mv[3] != 'K' && mv[3] != 'H') ||
        (len == 7 && (mv[3] < 'A' || mv[3] > 'A' + BOARD_SIZE - 1 ||
                      mv[5] < 'A' || mv[5] > 'A' + BOARD_SIZE - 1 ||
                      mv[4] < '0' || mv[4] > '0' + BOARD_SIZE - 1 ||
                      mv[6] < '0' || mv[6] > '0' + BOARD_SIZE - 1))) {
        cout << "i" << "Incorrect move format" << endl;
        return false;
    }

    return true;
}

bool game_completed(const struct game_state* gms) {
    int cr0 = 0, cr1 = 0;

    for (int i = 0; i < 10; i++) {
        if (gms->player[0].ship_state[i] == CRUSHED) {
            cr0++;
        }
        if (gms->player[1].ship_state[i] == CRUSHED) {
            cr1++;
        }
    }
    return gms->player[0].shots_num == SHOTS_NUM || gms->player[1].shots_num == SHOTS_NUM || cr0 == SHIPS_NUM || cr1 == SHIPS_NUM;
}

void change_pos(struct pos_t* t, char dir) {
    switch (dir) {
        case 'N':
            t->beg_row++;
            t->end_row++;
            break;
        case 'E':
            t->beg_col++;
            t->end_col++;
            break;
        case 'S':
            t->beg_row--;
            t->end_row--;
            break;
        case 'W':
            t->beg_col--;
            t->end_col--;
            break;
    }
}

/* Определяет, есть ли палубы рядом с клеткой (row, col) на поле игрока plr */
int get_state(struct game_state* gms, int plr, int row, int col) {
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (gms->player[plr].board[r][c] == DECK || gms->player[plr].board[r][c] == DAMAGE) {
                return FOUL;
            }
        }
    }
    return EMPTY;
}

/* Находит корабль игрока plr, которому принадлежит клетка (row, col) */
int get_ship_num(struct game_state* gms, int plr, int row, int col) {
    for (int i = 0; i < SHIPS_NUM; i++) {
        struct pos_t* t = &gms->player[plr].ships[i];
        if (row >= t->beg_row && row <= t->end_row && col >= t->beg_col && col <= t->end_col) {
            return i;
        }
    }
    return -1;
}

char get_correct_state(struct game_state* gms, int plr, int row, int col) {
    int k = get_ship_num(gms, plr, row, col);
    assert(k >= 0);

    struct pos_t* t = &gms->player[plr].ships[k];

    for (int r = t->beg_row; r <= t->end_row; r++) {
        for (int c = t->beg_col; c <= t->end_col; c++) {
            if (gms->player[plr].board[r][c] == DECK) {
                return 'H';
            }
            assert(gms->player[plr].board[r][c] == DAMAGE);
        }
    }

    return 'K';
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int read_game_config(struct game_state* gms, int plr) {
    const int ship_lengths[10] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
    string s;

    getline(cin, s);
    if (s.length() != CFG_SIZE) {
        cout << "Wrong config for player " << plr << ": wrong length of string. Config line: " << s << endl;
        return 1;
    }

    gms->player[plr].shots_num = 0;

    for (int j = 0; j < SHIPS_NUM; j++) {
        gms->player[plr].ship_state[j] = FIGHTING;

        struct pos_t* t = &gms->player[plr].ships[j];
        t->beg_col = s[j * 5 + 0] - 'A' + 1;
        t->beg_row = s[j * 5 + 1] - '0' + 1;
        t->end_col = s[j * 5 + 2] - 'A' + 1;
        t->end_row = s[j * 5 + 3] - '0' + 1;
        if ((j < SHIPS_NUM - 1 && s[j * 5 + 4] != ' ') ||
            t->beg_col < 1 || t->beg_col > BOARD_SIZE ||
            t->end_col < 1 || t->end_col > BOARD_SIZE ||
            t->beg_row < 1 || t->beg_row > BOARD_SIZE ||
            t->end_row < 1 || t->end_row > BOARD_SIZE)
        {
            cout << "Wrong config for player " << plr << ": wrong coordinates for ship #" << j + 1 << ". Config line: " << s << endl;
            return 1;
        }

        if (t->beg_row > t->end_row) {
            swap(&t->beg_row, &t->end_row);
        }
        if (t->beg_col > t->end_col) {
            swap(&t->beg_col, &t->end_col);
        }
        int dcol = t->end_col - t->beg_col + 1;
        int drow = t->end_row - t->beg_row + 1;
        if (!((dcol == ship_lengths[j] && drow == 1) || (dcol == 1 && drow == ship_lengths[j]))) {
            cout << "Wrong config for player " << plr << ": wrong size for ship #" << j + 1 << ". Config line: " << s << endl;
            return 1;
        }

        for (int row = t->beg_row; row <= t->end_row; row++) {
            for (int col = t->beg_col; col <= t->end_col; col++) {
                if (gms->player[plr].board[row][col] == EMPTY) {
                    gms->player[plr].board[row][col] = DECK;
                } else {
                    cout << "Wrong config for player " << plr << ": misplace of ship #" << j + 1 << ". Config line: " << s << endl;
                    return 1;
                }
            }
        }

        for (int row = t->beg_row - 1; row <= t->end_row + 1; row++) {
            for (int col = t->beg_col - 1; col <= t->end_col + 1; col++) {
                if (gms->player[plr].board[row][col] == EMPTY) {
                    gms->player[plr].board[row][col] = FOUL;
                }
            }
        }
    }

    getline(cin, s); //second line of config, not interesting
    return 0;
}

bool make_move(struct game_state* gms, const string& mv) {
    /* Ход соответствует правилам, если:
     * 1. Корабль совершает корректное перемещение, т.е.
     *    -- перемещения как такового нет, т.е. указано направление X
     *    -- перемещается корабль, все палубы которого исправны,
     *    -- перемещение выполняется в пределах поля,
     *    -- ни одна из новых клеток занимаемых кораблём не занята миной,
     *    -- ни одна из новых клеток занимаемых кораблём не граничит с другим кораблём
     * 2. При выполнении хода длиной 4, mv[3] == 'K' или 'H' (это проверено в move_format_correct()), 
     *    -- последний ход соперника должен быть выстрел,
     *    -- клетка с координатами последнего выстрела соперника на доске игрока
     *       должна быть исправной палубой, отметить её как DAMAGE.
     *    -- для случая 'K' эта палуба должна быть последней в корабле, необходимо проверить это 
     *       и отметить состояние корабля как CRUSHED.
     *    -- для 'H' -- не последней.
     * 3. При выполнении хода длиной 7,
     *    -- последний ход соперника может быть либо 'K' либо 'H' (это проверено на предыдущем ходе),
     *    -- либо последний ход соперника был выстрел, но соперник промахнулся 
     *       (проверить, текущее  перемещение корабля игрока к этому моменту выполнено, поставить мину).
     *    -- клетка, из которой выполняется выстрел, должна быть занята исправной палубой.
     *       (координаты цели на доске соперника могут быть любыми в пределах доски --  
     *        это проверяется move_format_correct())
     *    -- увеличить число выстрелов игрока.
     * Формат хода считается правильным, т.к. он проверен move_format_correct(), 
     */

    int cur_plr = gms->mv_num % 2;

    /* 1. */
    if (mv[1] != 'X') {
        int ship = mv[0] - '0';
        if (gms->player[cur_plr].ship_state[ship] != FIGHTING) {
            cout << "i" << "Game rules violation: try to move wounded ship" << endl;
            return false;
        }

        /* Перемещение выполняем за пять шагов
        * 1. Снимаем корабль с доски, отмечая занимаемые им клетки как пустые
        * 2. Заново сканируем и определяем состояние всех клеток
        *    занимаемых кораблём в исходной позиции и вокруг него. 
        *    (занимаемых кораблём можно было бы не проверять -- они останутся пустыми)
        * 3. Изменяем координаты корабля согласно направлению перемещения
        *    в списке координат
        * 4. Устанавливаем корабль в новую позицию, проверяя, что все вновь 
        *    занимаемые клетки свободны 
        * 5. Сканируем соседние клетки для новой позиции и изменяем их состояние 
        */
        /* 1.1. */
        struct pos_t* t = &gms->player[cur_plr].ships[ship];

        for (int row = t->beg_row; row <= t->end_row; row++) {
            for (int col = t->beg_col; col <= t->end_col; col++) {
                assert(gms->player[cur_plr].board[row][col] == DECK);
                gms->player[cur_plr].board[row][col] = EMPTY;
            }
        }
        /* 1.2. */
        for (int row = t->beg_row - 1; row <= t->end_row + 1; row++) {
            for (int col = t->beg_col - 1; col <= t->end_col + 1; col++) {
                int* cell = &gms->player[cur_plr].board[row][col];
                if (*cell == EMPTY || *cell == FOUL) {
                    *cell = get_state(gms, cur_plr, row, col);
                }
            }
        }
        /* 1.3. */
        change_pos(t, mv[1]);
        /* 1.4. */
        for (int row = t->beg_row; row <= t->end_row; row++) {
            for (int col = t->beg_col; col <= t->end_col; col++) {
                if (gms->player[cur_plr].board[row][col] == EMPTY) {
                    gms->player[cur_plr].board[row][col] = DECK;
                } else {
                    /* Различаются случаи выхода за пределы поля и некорр. передвижение в пределах поля */
                    int out_board = t->beg_row < 1 || t->end_row > BOARD_SIZE || t->beg_col < 1 || t->end_col > BOARD_SIZE;
                    if (out_board) {
                        cout << "i" << "Game rules violation: move ship out of the field" << endl;
                    } else {
                        cout << "i" << "Game rules violation: incorrect ship move "\
                            << (char) (t->beg_col + 'A' - 1) << (char) (t->beg_row + '0' - 1)\
                            << (char) (t->end_col + 'A' - 1) << (char) (t->end_row + '0' - 1) << endl;
                    }
                    return false;
                }
            }
        }
        /* 1.5. */
        for (int row = t->beg_row - 1; row <= t->end_row + 1; row++) {
            for (int col = t->beg_col - 1; col <= t->end_col + 1; col++) {
                int* cell = &gms->player[cur_plr].board[row][col];
                assert(*cell != DECK || (*cell == DECK && col >= t->beg_col && col <= t->end_col && row >= t->beg_row && row <= t->end_row));
                if (*cell != MINE && *cell != DECK && *cell != DAMAGE) {
                    *cell = get_state(gms, cur_plr, row, col);
                }
            }
        }
    }

    if (mv.length() == 4) {
        /* 2.1. Последний ход соперника -- выстрел */
        if (strlen(gms->player[1 - cur_plr].last_move) != 7) {
            cout << "i" << "Game rules violation: opponent could not wound player's ship because "\
                << (gms->mv_num == 0 ? "it is a first move" : "he didn't fire during the previous move") << endl;
            return false;
        }
        /* 2.2. Клетка-цель на доске игрока должна содержать палубу, отметить её как DAMAGE */
        int row = gms->player[1 - cur_plr].last_move[6] - '0' + 1; /* Координаты цели в последнем  */
        int col = gms->player[1 - cur_plr].last_move[5] - 'A' + 1; /* выстреле соперника */

        if (gms->player[cur_plr].board[row][col] != DECK) {
            cout << "i" << "Game rules violation: no unpadded deck in target " << col + 'A' - 1 << row + '0' - 1 << endl;
            return false;
        } else {
            gms->player[cur_plr].board[row][col] = DAMAGE;
        }
        /* 2.3. Проверить кол-во поврежденных палуб в корабле и корректность выбора хода 'K' или 'H' */
        if (get_correct_state(gms, cur_plr, row, col) != mv[3]) {
            cout << "i" << "Game rules violation: reply move \"" << mv[3] << "\" given, but player's ship with deck on"\
                << (char)(col + 'A' - 1) << (char)(row + '0' - 1) << " is wounded" << (mv[3] == 'K' ? " partly" : " completely") << endl;
            return false;
        } else {
            int k = get_ship_num(gms, cur_plr, row, col);
            gms->player[cur_plr].ship_state[k] = (mv[3] == 'K' ? CRUSHED : WOUNDED);
        }
    } else {
        /* 3. Ход-выстрел */
        int row, col;
        /* 3.1. Проверка того, что ответ на предыдущий ход соперника соответствует правилам */
        if (strlen(gms->player[1 - cur_plr].last_move) == 7) {
            row = gms->player[1 - cur_plr].last_move[6] - '0' + 1;
            col = gms->player[1 - cur_plr].last_move[5] - 'A' + 1;
            if (gms->player[cur_plr].board[row][col] == DECK) {
                cout << "i" << "Game rules violation: player hasn't informed opponent about wounding his ship on "\
                    << (char)(col + 'A' - 1) << (char)(row + '0' - 1) << endl;
                return false;
            }
            if (gms->player[cur_plr].board[row][col] != DAMAGE) {
                gms->player[cur_plr].board[row][col] = MINE;
            }
        }
        /* 3.2. Теперь проверить сам выстрел игрока (начальную клетку выстрела) */
        row = mv[4] - '0' + 1;
        col = mv[3] - 'A' + 1;
        if (gms->player[cur_plr].board[row][col] != DECK) {
            cout << "i" << "Game rules violation: player shoot from out of ship" << endl;
            return false;
        }
        gms->player[cur_plr].shots_num++;
    }

    strcpy(gms->player[cur_plr].last_move, mv.c_str());

    return true;
}

void output_result(struct game_state* gms, bool good_ending) {
    const int BONUS = 20;

    int cur_plr = gms->mv_num % 2;
    int win_plr = 0;

    int decks_1 = get_decks_cnt(gms->player[0].board);
    int decks_2 = get_decks_cnt(gms->player[1].board);

    int score_main[2] = {0, 0};
    int score_second[2] = {decks_1, decks_2};

    if (!good_ending) {

        win_plr = 1 - cur_plr;

        score_main[win_plr] = 2;
        score_second[win_plr] += BONUS;
        score_main[1 - win_plr] = 0;
        score_second[1 - win_plr] = 0;

    } else if (decks_1 == decks_2) {

        win_plr = 0;

        score_main[0] = 1;
        score_main[1] = 1;
        score_second[0] += BONUS;
        score_second[1] += BONUS;

    } else {

        win_plr = (decks_1 > decks_2) ? 0 : 1;

        score_main[win_plr] = 2;
        score_main[1 - win_plr] = 0;
        score_second[0] += BONUS + 2 * (decks_1 > decks_2);
        score_second[1] += BONUS + 2 * (decks_2 > decks_1);
    }

    cout << (win_plr + 1) << score_main[0] << ":" << score_main[1] << " (" << score_second[0] << ":" << score_second[1] << ")" << endl;
}

int main() {
    game_state* gms = new game_state;

    memset(gms, 0, sizeof(struct game_state));
    gms->mv_num = 0;

    for (int i = 0; i < 2; i++) {
        memset(gms->player[i].board, MINE, sizeof(gms->player[i].board));

        for (int row = 1; row <= BOARD_SIZE; ++row) {
            for (int col = 1; col <= BOARD_SIZE; ++col) {
                gms->player[i].board[row][col] = EMPTY;
            }
        }
    }

    strcpy(gms->player[1].last_move, "0X H");

    string s;
    bool finished = false, ending = false;

    while (true) {
        getline(cin, s);

        if (finished) {
            output_result(gms, ending);
            continue;
        }

        if (s[0] == 'c') {
            if (read_game_config(gms, s[1] - '0' - 1) == 0) {
                cout << "OK" << endl;
            }
        } else if (s[0] == ' ') {
            s = s.substr(1);
            if (move_format_correct(s) && make_move(gms, s)) {
                if (game_completed(gms)) {
                    finished = true;
                    ending = true;
                } else {
                    ++gms->mv_num;
                    cout << " " << s.substr(3) << endl;
                }
            } else {
                finished = true;
                ending = false;
            }
        } else if (s[0] == 'i') {
            finished = true;
            ending = false;
        } else if (s[0] == 'f') {
            finished = true;
        }

        if (finished) {
            output_result(gms, ending);
        }
    }

    delete gms;

    return 0;
}
