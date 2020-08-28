#!/usr/bin/env python3

import copy

BLACK = 1
WHITE = 2
stop = False

board = [[0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0]]

board[3][3] = WHITE
board[4][4] = WHITE

board[4][3] = BLACK
board[3][4] = BLACK

board = [copy.deepcopy(board), copy.deepcopy(board), copy.deepcopy(board), copy.deepcopy(board), copy.deepcopy(board)]


def convertCoordinates(strCoord):
    return (ord(strCoord[0]) - 97, int(strCoord[1]) - 1)


def disconvertCoordinates(x, y):
    return (chr(x + 97) + str((y) + 1))


def print_board(board_var):
    for i in range(8):
        print(board_var[i])
    print(' ')


myColor = int(input())
if myColor == BLACK:
    oponentColor = WHITE
else:
    oponentColor = BLACK

makeStep = myColor == BLACK


def score(board_var, color):
    myScore = 0
    oponentScore = 0
    for i in range(8):
        for j in range(8):
            if board_var[i][j] == color:
                myScore += 1
            elif board_var[i][j] != 0:
                oponentScore += 1
    return myScore - oponentScore


def revers(board_var, color, x, y):
    board_var[x][y] = color
    revers_1(board_var, color, x, y, 0, 1)
    revers_1(board_var, color, x, y, 1, 1)
    revers_1(board_var, color, x, y, 1, 0)
    revers_1(board_var, color, x, y, 1, -1)
    revers_1(board_var, color, x, y, 0, -1)
    revers_1(board_var, color, x, y, -1, -1)
    revers_1(board_var, color, x, y, -1, 0)
    revers_1(board_var, color, x, y, -1, 1)


def revers_1(board_var, color, x, y, dx, dy):
    doReverse = check_posibility(board_var, color, x, y, dx, dy)

    if doReverse is True:
        for k in range(1, 8):
            if board_var[x + dx * k][y + dy * k] == color:
                break
            board_var[x + dx * k][y + dy * k] = color


def check_posibility(board_var, color, x, y, dx, dy):
    if x + dx < 0 or x + dx > 7 or y + dy < 0 or y + dy > 7:
        return False

    if color == WHITE:
        reverseColor = BLACK
    else:
        reverseColor = WHITE

    if board_var[x + dx][y + dy] == 0 or board_var[x + dx][y + dy] == color:
        return False

    doReverse = False
    for k in range(1, 8):
        if x + dx * k < 0 or x + dx * k > 7 or y + dy * k < 0 or y + dy * k > 7:
            break
        if board_var[x + dx * k][y + dy * k] == 0:
            break
        if board_var[x + dx * k][y + dy * k] == color:
            doReverse = True
            break
    return doReverse


def doStep():
    INFINITY = 65
    best_move = None
    best_score = None
    board[1] = copy.deepcopy(board[0])
    for i in range(8):
        for j in range(8):
            new_score = alphabeta(1, -INFINITY, INFINITY, max_depth, i, j)
            if best_score is None or new_score > best_score or big_check(board[0], myColor,*convertCoordinates(best_move)) == False:
                best_score = new_score
                best_move = disconvertCoordinates(i, j)
            board[1] = copy.deepcopy(board[0])
    if best_move is None:
        return 'Skip'
    else:
        if big_check(board[0], myColor, *convertCoordinates(best_move)) == False:
            return 'Skip'
        else:
            return best_move


def alphabeta(depth, alpha, beta, max_depth, i, j):
    if depth - 1 == max_depth:
        return score(board[max_depth], myColor)
    position = False
    if depth == 2:
        color = oponentColor
    else:
        color = myColor
    if big_check(board[depth], color, i, j) is True:
        revers(board[depth], color, i, j)
        if depth < max_depth:
            board[depth + 1] = board[depth]
        position = True
        for k in range(8):
            for l in range(8):
                new_score = alphabeta(depth + 1, -beta, -alpha, max_depth, k, l)
                alpha = max(alpha, new_score)
                board[depth] = copy.deepcopy(board[depth - 1])
    if position is False:
        return score(board[depth], myColor)
    else:
        return alpha


def big_check(board_var, color, x, y):
    result = False
    if board_var[x][y]==0:
        result |= check_posibility(board_var, color, x, y, 0, 1)
        result |= check_posibility(board_var, color, x, y, 1, 1)
        result |= check_posibility(board_var, color, x, y, 1, 0)
        result |= check_posibility(board_var, color, x, y, 1, -1)
        result |= check_posibility(board_var, color, x, y, 0, -1)
        result |= check_posibility(board_var, color, x, y, -1, -1)
        result |= check_posibility(board_var, color, x, y, -1, 0)
        result |= check_posibility(board_var, color, x, y, -1, 1)
    return result


skip_received = False
max_depth = 2

while stop is not True:
    if makeStep is True:
        step = doStep()
        print(step, flush=True)
        if step == 'Skip':
            if skip_received is True:
                stop = False
                break
            else:
                skip_received = True
        else:
            skip_received = False
            revers(board[0], myColor, *convertCoordinates(step))
    else:
        oponentStep = input()
        if oponentStep == 'Skip':
            if skip_received is True:
                stop = False
                break
            else:
                skip_received = True
        else:
            skip_received = False
            revers(board[0], oponentColor, *convertCoordinates(oponentStep))

    makeStep = not makeStep