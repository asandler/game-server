#!/usr/bin/env python3

import random
import copy
from operator import itemgetter
di = {'a': 1, 'b': 2, 'c': 3, 'd': 4,
      'e': 5, 'f': 6, 'g': 7, 'h': 8}
ls = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
li_b = []
li_w = []
board = [['o' for i in range(9)] + ['o'] for i in range(9)]
board.append(['o' for i in range(10)])
board = [['o' for i in range(10)]] + board
lp = [(0, -1), (0, 1), (-1, 0), (1, 0), (-1, -1), (-1, 1), (1, -1), (1, 1)]
board[4][4], board[5][5] = 'w', 'w'
board[4][5], board[5][4] = 'b', 'b'
c = 'w'
dc = 'b'


def get_moves(board, c, dc, lp):
    global u
    ans = []
    for i in range(1, 9):
        for j in range(1, 9):
            f = False
            if board[i][j] != 'w' and board[i][j] != 'b':

                g = False
                pl = 0
                for h in lp:
                    s = i + h[0]
                    r = j + h[1]

                    su = 0
                    while board[s][r] != 'o':
                        if board[s][r] == c:
                            if su > 0:
                                g = True
                            break
                        else:
                            su += 1
                        s += h[0]
                        r += h[1]
                    pl += su
                if g:
                    ans.append([(i, j), pl])

    return ans


def replace(i, j, lp, board, c):
    for h in lp:
        s = i + h[0]
        r = j + h[1]
        mk = []
        while board[s][r] != 'o':
            if board[s][r] == c:
                for e in mk:
                    board[e[0]][e[1]] = c
                break
            else:
                mk.append((s, r))
            s += h[0]
            r += h[1]
    return board


def minimax(moves, depth, board, isMax, h, lp, c, dc):
    if isMax:
        if moves == []:
            return (0, 0)
        for j in moves:
            board1 = board
            board1[j[0][0]][j[0][1]] = c
            board1 = replace(j[0][0], j[0][1], lp, board, c)

            moves2 = get_moves(board, dc, c, lp)
            l1 = minimax(moves2, depth+1, board1, not isMax, h, lp, dc, c)
            j[1] += l1
        return sorted(moves, key=itemgetter(1))[-1]
    else:
        if moves == []:
            return 0
        if depth == h:
            le = sorted(moves, key=itemgetter(1))
            return -le[-1][1]
        else:
            l2 = []
            for i in moves:
                board3 = board
                board3[i[0][0]][i[0][1]] = c
                board3 = replace(i[0][0], i[0][1], lp, board3, c)
                i[1] = -i[1] + minimax(get_moves(board3, dc, c, lp), depth, board3, not isMax, h, lp, dc, c)[1]
                l2.append(i[1])

            return sorted(l2)[0]




q = (int(input()) == 1)
if q:
    c = 'b'
    dc = 'w'
    #print(['d3', 'c4', 'e6', 'f5'][random.randint(0, 3)])
    print('d3')
    board[4][3] = c
    board = replace(4, 3, lp, board, c)
while True:
    a = input()
    if a != 'Skip':
        board[di[a[0]]][int(a[1])] = dc
        board = replace(di[a[0]], int(a[1]), lp, board, dc)
    moves = get_moves(board, c, dc, lp)
    if moves == []:
        print('Skip')
        continue
    move = minimax(moves, 0, copy.deepcopy(board), True, 7, lp, c, dc)
    board[move[0][0]][move[0][1]] = c
    board = replace(move[0][0], move[0][1], lp, board, c)
    move = ls[move[0][0] - 1] + str(move[0][1])
    print(move)
