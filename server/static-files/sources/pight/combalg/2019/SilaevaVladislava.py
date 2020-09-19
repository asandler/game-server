#!/usr/bin/env python3
from random import choice

N, M = 40, 30


def move(d, i, j):
    area = [0] * 4
    area[0] = [i, j - 1]
    area[1] = [i - 1, j]
    area[2] = [i, j + 1]
    area[3] = [i + 1, j]
    for a in area:
        if N > a[0] >= 0 and M > a[1] >= 0:
            a.append(field[a[0]][a[1]])
        else:
            a.append(-1)
    area[(d - 1) % 4].append('L')
    area[d].append('F')
    area[(d + 1) % 4].append('R')
    area[(d + 2) % 4].append('B')
    moves = []
    for a in area:
        if a[2] == 1:
            return a[-1]
        if a[2] == 0:
            moves.append(a[-1])
    if len(moves):
        moves = ['L', 'F', 'R']
    return choice(moves)


def dir_move(d, m):
    if m == 'F':
        return d
    if m == 'L':
        return (d - 1) % 4
    if m == 'R':
        return (d + 1) % 4



def do_move(d, s):
    if d == 0:
        m = (s[0][0], s[0][1] - 1)
    if d == 1:
        m = (s[0][0] - 1, s[0][1])
    if d == 2:
        m = (s[0][0], s[0][1] + 1)
    if d == 3:
        m = (s[0][0] + 1, s[0][1])
    s.insert(0, m)
    if food[s[-1][0]][s[-1][1]] == 0:
        field[s[-1][0]][s[-1][1]] = 0
        del s[-1]
    else:
        food[s[-1][0]][s[-1][1]] = 0
    field[s[0][0]][s[0][1]] = field[s[1][0]][s[1][1]]
    field[s[1][0]][s[1][1]] = field[s[2][0]][s[2][1]]

def check(head):
    return True


me = [3, [(3, 0), (2, 0), (1, 0), (0, 0)]]
en = [1, [(M - 4, N - 1), (M - 3, N - 1), (M - 2, N - 1), (M - 1, N - 1)]]
# |_|1|_|
# |0|_|2|
# |_|3|_|
field = [[0] * N for i in range(M)]
pl = int(input())
for i in range(M):
    field[i] = [int(j) for j in input()]

food = [row[:] for row in field]
for i in range(3):
    field[i][0] = 2  # my body
    field[M - 1 - i][N - 1] = 3  # enemy's body
field[3][0] = 4  # my head
field[M - 4][N - 1] = 5  # enemy's head


game = True
if pl == 1:
    m = move(me[0], *me[1][0])
    print(m)
    me[0] = dir_move(me[0], m)
    do_move(*me)
    game = check(en[1][0])

while game:
    en[0] = dir_move(en[0], input())
    do_move(*en)
    m = move(me[0], *me[1][0])
    print(m)
    me[0] = dir_move(me[0], m)
    do_move(*me)
    game = check(me[1][0])