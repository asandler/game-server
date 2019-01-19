#!/usr/bin/env python3
import sys

sys.setrecursionlimit(10000000)

END_NO = -1
END_TIE = 0
END_TIC = 1
END_TAC = 2

CELL_NONE = 0
CELL_TIC = 1
CELL_TAC = 2

TURN_TIC = 1
TURN_TAC = 2

moves = dict()

class Position:
    def __init__(self, field=[[CELL_NONE for i in range(3)] for j in range(3)], turn=TURN_TIC):
        self.field = field
        self.strfield = ''.join([str(field[i // 3][i % 3]) for i in range(9)])
        self.turn = turn
        
    def iswin(self):
        for i in range(3):
            tiehope = False
            if self.field[i] == [CELL_TIC for iterator in range(3)]:
                return END_TIC
            elif self.field[i] == [CELL_TAC for iterator in range(3)]:
                return END_TAC
        for i in range(3):
            if [self.field[0][i], self.field[1][i], self.field[2][i]] == [CELL_TIC for iterator in range(3)]:
                return END_TIC
            elif [self.field[0][i], self.field[1][i], self.field[2][i]] == [CELL_TAC for iterator in range(3)]:
                return END_TAC
        if [self.field[0][0], self.field[1][1], self.field[2][2]] == [CELL_TIC for iterator in range(3)]:
            return END_TIC
        if [self.field[0][0], self.field[1][1], self.field[2][2]] == [CELL_TAC for iterator in range(3)]:
            return END_TAC
        if [self.field[0][2], self.field[1][1], self.field[2][0]] == [CELL_TIC for iterator in range(3)]:
            return END_TIC
        if [self.field[0][2], self.field[1][1], self.field[2][0]] == [CELL_TAC for iterator in range(3)]:
            return END_TAC
        for i in range(3):
            for j in range(3):
                if self.field[i][j] == CELL_NONE:
                    return END_NO
        return END_TIE
    
    def recursive_search(self, goto=False):
        if self.iswin() != END_NO:
            return self.iswin()
        best = (-1, (-1, -1))
        if self.strfield in moves.keys():
            return moves[self.strfield][int(goto)]
        for i in range(3):
            for j in range(3):
                if self.field[i][j] == CELL_NONE:
                    newfield = [[CELL_NONE for i in range(3)] for j in range(3)]
                    for x in range(3):
                        for y in range(3):
                            newfield[x][y] = self.field[x][y]
                    newfield[i][j] = self.turn
                    result = Position(newfield,
                TURN_TAC + TURN_TIC - self.turn).recursive_search()
                    if result == self.turn:
                        moves[self.strfield] = (self.turn, (i, j))
                        if goto:
                            return (i, j)
                        else:
                            return self.turn
                    if result == END_TIE:
                        best = (END_TIE, (i, j))
                    elif best[0] != END_TIE:
                        best = (result, (i, j))
        moves[self.strfield] = best
        return best[int(goto)]

field = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

turnnow = input()
if turnnow == '1':
    field[1][1] = 1
    print(4, end='\n')
enturn = int(input())
field[enturn // 3][enturn % 3] = 2
if field[1][1] == CELL_NONE:
    field[1][1] = 1
    print(4, end='\n')
elif field[0][0] == CELL_NONE:
    field[0][0] = 1
    print(0, end='\n')
else:
    field[0][2] = 1
    print(2, end='\n')
while True:
    enturn = int(input())
    field[enturn // 3][enturn % 3] = 2
    myturn = Position(field).recursive_search(True)
    field[myturn[0]][myturn[1]] = 1
    print(myturn[0] * 3 + myturn[1], end='\n')
    