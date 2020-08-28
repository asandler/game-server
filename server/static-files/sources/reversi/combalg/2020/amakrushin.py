#!/usr/bin/env python3

from copy import deepcopy


def end(area_end):
    for i in area_end:
        if -1 in i:
            return False
    return True


def score_board(pl, area_score):
    c = 0
    s = 0
    for i in range(8):
        for j in range(8):
            if area_score[i][j] == pl:
                s += table[i][j]
                c += 1
    return s / c


def minimax(pl1, depth, area_minix, max_depth, scoring_function):
    # Если мы достигли максимальной глубины обхода или игра закончилась, то оцениваем позицию.
    if max_depth == depth or end(area_minix):
        return scoring_function(pl1, area_minix)
    # Иначе - для каждого возможного хода анализируем его.
    best_score = None
    attacks = can_attack(pl1)
    for x, y in attacks:
        area1 = deepcopy(area_minix)
        update_area(area1, pl1, x, y)
        new_score = -minimax(int(not pl1), depth + 1, area1, max_depth, scoring_function)
        if best_score is None or new_score > best_score:
            best_score = new_score
    if not attacks:
        new_score = -minimax(int(not pl1), depth + 1, area_minix, max_depth, scoring_function)
        if best_score is None or new_score > best_score:
            best_score = new_score
    return best_score


def minimax_step(max_depth=3, scoring_function=score_board):
    global area
    global player
    best_move = (100, 0)
    best_score = None
    attacks = can_attack(player)
    for x, y in attacks:
        area1 = deepcopy(area)
        update_area(area1, player, x, y)
        new_score = -minimax(int(not player), 0, area1, max_depth, scoring_function)
        if best_score is None or new_score > best_score:
            best_score = new_score
            best_move = (x, y)
            continue
    return best_move


def can_attack_1(pl, i, j):
    global area
    enemy = int(not pl)
    if area[i][j] != -1:
        return False
    i1, j1 = i + 1, j

    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i - 1, j
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i + 1, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 += 1
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i + 1, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 += 1
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i - 1, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 -= 1
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True

    i1, j1 = i - 1, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        i1 -= 1
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        return True
    return False


def can_attack(pl):
    global area
    ans = []
    for i in range(8):
        for j in range(8):
            if can_attack_1(pl, i, j):
                ans.append((i, j))
    return ans


def update_area(upd_area, pl, i, j):
    replac = [(i, j)]
    enemy = int(not pl)

    rep = []
    i1, j1 = i + 1, j
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i - 1, j
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i + 1, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 += 1
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i + 1, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 += 1
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i - 1, j + 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 -= 1
        j1 += 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep

    rep = []
    i1, j1 = i - 1, j - 1
    x = False
    while 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == enemy:
        rep += [(i1, j1)]
        i1 -= 1
        j1 -= 1
        x = True
    if x and 0 <= i1 <= 7 and 0 <= j1 <= 7 and area[i1][j1] == pl:
        replac += rep
    for abc in replac:
        upd_area[abc[0]][abc[1]] = pl


def draw(pole):
    for ii, i in enumerate(pole):
        print('abcdefgh'[ii], end=' ')
        for j in i:
            if j == -1:
                print('.', end='')
            if j == 0:
                print('&', end='')
            if j == 1:
                print('#', end='')
        print()
    print('  12345678')
    print()


# 0 - С‡РµСЂРЅС‹Рµ
# 1 - Р±РµР»С‹Рµ

area = [
    [-1, -1, -1, -1, -1, -1, -1, -1],
    [-1, -1, -1, -1, -1, -1, -1, -1],
    [-1, -1, -1, -1, -1, -1, -1, -1],
    [-1, -1, -1, 1, 0, -1, -1, -1],
    [-1, -1, -1, 0, 1, -1, -1, -1],
    [-1, -1, -1, -1, -1, -1, -1, -1],
    [-1, -1, -1, -1, -1, -1, -1, -1],
    [-1, -1, -1, -1, -1, -1, -1, -1]
]
table = [
    [15, 1, 7, 7, 7, 7, 1, 15],
    [1, 1, 3, 4, 4, 3, 1, 1],
    [7, 3, 5, 6, 6, 5, 3, 7],
    [7, 4, 3, 8, 8, 6, 4, 7],
    [7, 4, 3, 8, 8, 6, 4, 7],
    [7, 3, 5, 6, 6, 5, 3, 7],
    [1, 1, 3, 4, 4, 3, 1, 1],
    [15, 1, 7, 7, 7, 7, 1, 15]
]
d = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7}
d1 = {0: 'a', 1: 'b', 2: 'c', 3: 'd', 4: 'e', 5: 'f', 6: 'g', 7: 'h'}
player = int(input())
player = (1 if player == 2 else 0)
while True:
    if player:
        enemy_step = input()
        if enemy_step != 'Skip':
            a, b = d[enemy_step[0]], int(enemy_step[1]) - 1
            update_area(area, 0, a, b)
        a, b = minimax_step()
        if a == 100:
            print('Skip')
        else:
            print(d1[a], b + 1, sep='')
            update_area(area, 1, a, b)
    else:
        a, b = minimax_step()
        if a == 100:
            print('Skip')
        else:
            print(d1[a], b + 1, sep='')
            update_area(area, 0, a, b)
        enemy_step = input()
        if enemy_step != 'Skip':
            a, b = d[enemy_step[0]], int(enemy_step[1]) - 1
            update_area(area, 1, a, b)
