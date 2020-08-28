#!/usr/bin/env python3
from copy import deepcopy

convertNumbers = {0: 'a', 1: 'b', 2: 'c', 3: 'd', 4: 'e', 5: 'f', 6: 'g', 7: 'h'}
for k in range(-100, 100):
    if k not in convertNumbers:
        convertNumbers[k] = '0'
convertString = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4, 'f': 5, 'g': 6, 'h': 7}
for k in range(-100, 100):
    if k not in convertString:
        convertString[k] = '0'


class MyGame():

    gameTree = [# 1 2  3  4  5  6  7  8
                [0, 0, 0, 0, 0, 0, 0, 0],   # a
                [0, 0, 0, 0, 0, 0, 0, 0],   # b
                [0, 0, 0, 0, 0, 0, 0, 0],   # c
                [0, 0, 0, 2, 1, 0, 0, 0],   # d
                [0, 0, 0, 1, 2, 0, 0, 0],   # e
                [0, 0, 0, 0, 0, 0, 0, 0],   # f
                [0, 0, 0, 0, 0, 0, 0, 0],   # g
                [0, 0, 0, 0, 0, 0, 0, 0],   # h
                ]

    gamex = -1
    gamey = -1
    newPositions = []

    def __init__(self, player):
        self.player = player


    def countNewPosition(self, dx, dy, dy_1, dx_1):
            tmp = False
            for s in range(1, 8):
                tmp_2 = dy + (s * dy_1)
                tmp_3 = dx + (s * dx_1)
                if tmp_2 not in range(0, 8) or tmp_3 not in range(0, 8):
                    break
                if self.gameTree[tmp_2][tmp_3] == 3 - self.player:
                    tmp = True
                elif self.gameTree[tmp_2][tmp_3] == 0 and tmp is True:
                    self.newPositions.append(str(tmp_2) + str(tmp_3))
                    tmp = False
                else:
                    break


    def countRevers(self, dx, dy, dy_1, dx_1):
            tmp = {}
            for s in range(1, 8):
                tmp_2 = dy + (s * dy_1)
                tmp_3 = dx + (s * dx_1)
                if tmp_2 not in range(0, 8) or tmp_3 not in range(0, 8):
                    break
                if self.gameTree[tmp_2][tmp_3] == 3 - self.player:
                    tmp[str(tmp_2) + str(tmp_3)] = self.player
                elif self.gameTree[tmp_2][tmp_3] == self.player:
                    for t in tmp:
                        self.gameTree[int(t[0])][int(t[1])] = self.player


    def newPosition(self, dx, dy):
        self.countNewPosition(dx, dy, 1, 1)
        self.countNewPosition(dx, dy, -1, -1)
        self.countNewPosition(dx, dy, 1, -1)
        self.countNewPosition(dx, dy, -1, 1)
        self.countNewPosition(dx, dy, 0, 1)
        self.countNewPosition(dx, dy, 1, 0)
        self.countNewPosition(dx, dy, -1, 0)
        self.countNewPosition(dx, dy, 0, -1)



    def revers(self):
            dy = self.gamey
            dx = self.gamex
            self.countRevers(dx, dy, 1, 1)
            self.countRevers(dx, dy, -1, -1)
            self.countRevers(dx, dy, 1, -1)
            self.countRevers(dx, dy, -1, 1)
            self.countRevers(dx, dy, 0, 1)
            self.countRevers(dx, dy, 1, 0)
            self.countRevers(dx, dy, -1, 0)
            self.countRevers(dx, dy, 0, -1)


    def NegaScout(self, alpha, beta, g, depth):
        g = deepcopy(self.gameTree)
        if depth == 4:
            return self.Ev()


    def printGame(self):
        if len(self.newPositions) != 0:
            tmp = -1
            tmp_dy = -1
            tmp_dx = -1
            for x in self.newPositions:
                dx = int(x[1])
                dy = int(x[0])
                tmp2 = self.Ev(dx,dy)
                if tmp < tmp2:
                    tmp_dy = dy
                    tmp_dx = dx
                    tmp = tmp2
            self.gamex = tmp_dx
            self.gamey = tmp_dy
            self.gameTree[tmp_dy][tmp_dx] = self.player
            print(convertNumbers[tmp_dy] + str(tmp_dx+1))
        else:
         print('Skip')


    def Ev(self, dx, dy):
        a = self.coutEv(dx, dy, 1, 1)
        a += self.coutEv(dx, dy, -1, -1)
        a += self.coutEv(dx, dy, -1, 1)
        a += self.coutEv(dx, dy, 1, -1)
        a += self.coutEv(dx, dy, 0, 1)
        a += self.coutEv(dx, dy, 0, -1)
        a += self.coutEv(dx, dy, 1, 0)
        a += self.coutEv(dx, dy, -1, 0)
        return a


    def coutEv(self, dx, dy, dy_1, dx_1):
            tmp = 0
            for s in range(1,8):
                tmp_dx = dx + (s * dx_1)
                tmp_dy = dy + (s * dy_1)
                if tmp_dx not in range(0, 8) or tmp_dy not in range(0, 8):
                    break
                if self.gameTree[tmp_dy][tmp_dx] == self.player:
                    tmp += 5
                elif self.gameTree[tmp_dy][tmp_dx] == 3 - self.player:
                    tmp += 3
                else:
                    tmp += 1
            return tmp







f = int(input())
game = MyGame(2)
if f == 1:
    print('c4')
    game.gameTree[2][3] = 1
    game.player = 1
    game.gamex = 3
    game.gamey = 2
    game.revers()
while True:
    game.player = 3 - game.player
    k = str(input())
    game.gamey = convertString[k[0]]
    game.gamex = int(k[1])-1
    game.gameTree[game.gamey][game.gamex] = game.player
    game.revers()
    game.player = 3 - game.player
    for dx in range(0, 8):
        for dy in range(0, 8):
            if game.gameTree[dy][dx] == game.player:
                game.newPosition(dx, dy)
    game.printGame()
    game.revers()
    game.newPositions = []