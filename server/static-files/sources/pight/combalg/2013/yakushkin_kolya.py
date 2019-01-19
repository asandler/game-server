#!/usr/bin/env python3
from random import*


def IsFood(Map, Python, Turn):
    H, PH = Python.S[-1], Python.S[-2]
    return (Turn == 'L' and Map[29 - (H.y + H.x - PH.x)][H.x - H.y + PH.y] == '1') or (Turn == 'R' and Map[29 - (H.y - H.x + PH.x)][H.x + H.y - PH.y] == '1') or (Turn == 'F' and Map[29 - (2 * H.y - PH.y)][2 * H.x - PH.x] == '1')


class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __str__(self):
        return '(' + str(self.x) + ', ' + str(self.y) + ')'


class Python:
    def __init__(self, S, length):
        self.S, self.length = S, length

    def move(self, turn):
        H = self.S[-1]
        PH = self.S[-2]
        if turn == 'L':
            H = Point(H.x - H.y + PH.y, H.y + H.x - PH.x)
            self.S = self.S[1:] + [H]
        elif turn == 'R':
            H = Point(H.x + H.y - PH.y, H.y - H.x + PH.x)
            self.S = self.S[1:] + [H]
        else:
            H = Point(2 * H.x - PH.x, 2 * H.y - PH.y)
            self.S = self.S[1:] + [H]

    def turn(self, other):
        H = self.S[-1]
        PH = self.S[-2]
        T =  ['L', 'F', 'R']
        if (H.x == 0 and PH.x == 1) or (H.x == 39 and PH.x == 38) or (H.y == 0 and PH.y == 1) or (H.y == 29 and PH.y == 28):
            T = ['L', 'R']
        if (H.x == 0 and H.y > PH.y) or (H.x == 39 and H.y < PH.y) or (H.y == 0 and H.x < PH.x) or (H.y == 29 and H.x > PH.x):
            T = T[1:]
        if (H.x == 0 and H.y < PH.y) or (H.x == 39 and H.y > PH.y) or (H.y == 0 and H.x > PH.x) or (H.y == 29 and H.x < PH.x):
            T = T[:-1]
        i = 0
        while i < self.length - 2 and len(T) > 0:
            P = self.S[i]
            if P.x == H.x - H.y + PH.y and P.y == H.y + H.x - PH.x and T[0] == 'L':
                T = T[1:]
            elif P.x == H.x + H.y - PH.y and P.y == H.y - H.x + PH.x and T[-1] == 'R':
                T = T[:-1]
            elif P.x == 2 * H.x - PH.x and P.y == 2 * H.y - PH.y:
                if T[0] == 'F':
                    T = T[1:]
                elif len(T) > 1 and T[1] == 'F':
                    T = [T[0]] + T[2:]
            i += 1
        while i < other.length and len(T) > 0:
            P = other.S[i]
            if P.x == H.x - H.y + PH.y and P.y == H.y + H.x - PH.x and T[0] == 'L':
                T = T[1:]
            elif P.x == H.x + H.y - PH.y and P.y == H.y - H.x + PH.x and T[-1] == 'R':
                T = T[:-1]
            elif P.x == 2 * H.x - PH.x and P.y == 2 * H.y - PH.y:
                if T[0] == 'F':
                    T = T[1:]
                elif len(T) > 1 and T[1] == 'F':
                    T = [T[0]] + T[2:]
            i += 1
        if T == []:
            return 'F'
        else:
            return choice(T)


Player = int(input())
Map = []
for i in range(30):
    Map += [list(input().rstrip())]
Map[0][0], Map[1][0], Map[2][0], Map[3][0], Map[29][39], Map[28][39], Map[27][39], Map[26][39] = 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'
if Player == 1:
    EnemySnake = [Point(39, 0), Point(39, 1), Point(39, 2), Point(39, 3)]
    MyTurn = choice(['L', 'F'])
    print(MyTurn)
    Map[0][0] = '0'
    if MyTurn == 'L':
        MySnake = [Point(0, 28), Point(0, 27), Point(0, 26), Point(1, 26)]
        Map[3][1] = 'X'
    else:
        MySnake = [Point(0, 28), Point(0, 27), Point(0, 26), Point(0, 25)]
        Map[4][0] = 'X'
else:
    MySnake = [Point(39, 0), Point(39, 1), Point(39, 2), Point(39, 3)]
    EnemySnake = [Point(0, 29), Point(0, 28), Point(0, 27), Point(0, 26)]
Py = Python(MySnake, 4)
Enemy = Python(EnemySnake, 4)
MyFood = []
EnemyFood = []
while True:
    EnemyTurn = input()
    if IsFood(Map, Enemy, EnemyTurn):
        EnemyFood += [Enemy.length]
    EnemyTail = EnemySnake[0]
    Enemy.move(EnemyTurn)
    k = -1
    for i in range(len(EnemyFood)):
        EnemyFood[i] -= 1
        if EnemyFood[i] == 0:
            k = i
            for j in range(len(EnemyFood)):
                if j != i:
                    EnemyFood[j] += 1
            Enemy.length += 1
            Enemy.S = [EnemyTail] + Enemy.S
    if k != -1:
        EnemyFood = EnemyFood[:k] + EnemyFood[k + 1:]
    EnemySnake = Enemy.S
    Map[29 - EnemyTail.y][EnemyTail.x] = '0'
    EnemyHead = EnemySnake[-1]
    Map[29 - EnemyHead.y][EnemyHead.x] = 'X'
    MyTail, MyHead = MySnake[0], MySnake[-1]
    MyTurn = Py.turn(Enemy)
    if IsFood(Map, Py, MyTurn):
        MyFood += [Py.length]
    Py.move(MyTurn)
    k = -1
    for i in range(len(MyFood)):
        MyFood[i] -= 1
        if MyFood[i] == 0:
            k = i
            for j in range(len(MyFood)):
                if j != i:
                    MyFood[j] += 1
            Py.length += 1
            Py.S = [MyTail] + Py.S
    if k != -1:
        MyFood = MyFood[:k] + MyFood[k + 1:]
    MySnake = Py.S
    Map[29 - MyTail.y][MyTail.x] = '0'
    MyHead = MySnake[-1]
    Map[29 - MyHead.y][MyHead.x] = 'X'
    print(MyTurn)
