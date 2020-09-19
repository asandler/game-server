#!/usr/bin/env python3

from numpy import matrix
import numpy as np

FIELD = [
    '0000000000000000000000000000000000000001',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000001000000000000100000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000010000000000000000000000000',
    '0000000000000010000000000000001000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000001000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
    '0000000000000000000000000000000000000000',
]


class vec:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return vec(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return vec(self.x - other.x, self.y - other.y)

    def len2(self):
        return self.x * self.x + self.y * self.y
    def len(self):
        return (self.x * self.x + self.y * self.y) ** 0.5

    @staticmethod
    def ro2(vec1, vec2):
        return (vec1.x - vec2.x) ** 2 + (vec1.y - vec2.y) ** 2

    @staticmethod
    def ro(vec1, vec2):
        return ((vec1.x - vec2.x) ** 2 + (vec1.y - vec2.y) ** 2) ** 0.5

class mymat:
    def __init__(self, f):
        #INPUT 30x40
        self.pole = []
        self.w = 40
        self.h = 30
        self.fill(f)
        self.current_data = self.data()

    def fill(self, f):
        a = 0

        for l in f:
            for s in l:
                if s == '0':
                    a = 0
                elif s == '1':
                    a = 0.99
                elif s == '#':
                    a = 0.4
                elif s == '*':
                    a = 0.5
                elif s == '$':
                    a = 0.6
                elif s == '&':
                    a = 0.7

                self.pole.append(a)

    def data(self):
        aa = []
        lis = []

        for i in range(len(self.pole)):
            if i == 1199:
                lis.append(self.pole[i])
                aa.append(lis)
            elif len(lis) != self.w:
                lis.append(self.pole[i])

            elif len(lis) == self.w:
                aa.append(lis)
                lis = []
                lis.append(self.pole[i])

        s = matrix(aa)
        return s

    def get(self, vec):
        return self.pole[vec.x + 1199 - vec.y * self.w]

    def push(self, vec, val):
        b = self.pole[vec.x + 1199 - vec.y * self.w]
        self.pole[vec.x + 1199 - vec.y * self.w] = val
        return b

    def find(self, val, mat):
        y, x = np.where(mat == val)
        f1, f2 = list(x), list(y)
        a = []
        for k in range(len(f1)):
            a.append(vec(f1[k] + 1, 30 - f2[k]))
        return a

    def view(self):
        for i in range(len(self.pole)):
            a = self.pole[i]
            if a == 0:
                a = '.'
            elif a == 0.99:
                a = '1'
            elif a == 0.4:
                a = '#'
            elif a == 0.5:
                a = '%'
            elif a == 0.6:
                a = '$'
            elif a == 0.7:
                a = '&'

            print(a, end='')
            if i % 40 == 39:
                print('\n', end='')

def get_min(pos, li):
    min = (li[0] - pos).len2()
    last = li[0]
    for i in li:
        a = (i - pos).len2()
        if min > a:
            min = a
            last = i
    return min

def netw(field, snake):
    data = field.data()
    mas = field.find(0.5, data)
    pos = mas[0]
    li = field.find(0.99, data)
    mi = get_min(pos, li)
    turn = ''
    for G in ['R', 'L', 'F']:
        v = snake.direction
        snake.new_dir(G)
        pos1 = snake.pos + snake.direction
        f = get_min(pos1, li)
        if f < mi:
            mi = f
            turn = G
        snake.direction = v
    return turn

class enemy_snake:
    def __init__(self, v, o, field):
        self.pos = v
        self.tails = []
        self.isturn = not o
        if self.isturn:
            for i in range(1, 4):
                self.tails.append(self.pos + vec(0, i))
            self.direction = vec(0, -1)
        elif self.isturn == 0:
            for i in range(1, 4):
                self.tails.append(self.pos + vec(0, -i))
            self.direction = vec(0, 1)
        self.push(field)

    def do_turn(self, G, field):
        le = len(self.tails)
        last = self.tails[le - 1]
        for i in range(1, le):
            self.tails[le - i] = self.tails[le - i - 1]
        self.tails[0] = self.pos

        self.new_dir(G)
        self.pos = self.pos + self.direction
        n = self.push(field)

        if n == 0.99:
            self.add_tail()
    def new_dir(self, G):
        if self.direction.y == 1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(1, 0)
            elif G == 'L':
                self.direction = vec(-1, 0)
        elif self.direction.y == -1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(-1, 0)
            elif G == 'L':
                self.direction = vec(1, 0)
        elif self.direction.x == 1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(0, -1)
            elif G == 'L':
                self.direction = vec(0, 1)
        elif self.direction.x == -1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(0, 1)
            elif G == 'L':
                self.direction = vec(0, -1)

    def add_tail(self):
        self.tails.append(self.tails[len(self.tails) - 1])

    def push(self, field):
        field.push(self.pos, 0.7) #0.7??
        for i in self.tails:
            field.push(i, 0.6)


class player_snake:
    def __init__(self, v, o, field):
        self.pos = v
        self.tails = []
        self.isturn = not o
        if self.isturn:
            for i in range(1, 4):
                self.tails.append(self.pos + vec(0, i))
            self.direction = vec(0, -1)
        elif self.isturn == 0:
            for i in range(1, 4):
                self.tails.append(self.pos + vec(0, -i))
            self.direction = vec(0, 1)
        self.push(field)


    def add_tail(self):
        self.tails.append(self.tails[len(self.tails) - 1])

    def count_turn(self, field):
        a = netw(field, self)
        v = self.direction
        self.new_dir(a)
        pos = self.pos + self.direction
        if field.get(pos) == 0 or field.get(pos) == 0.99:
            self.direction = v
            return a


    def do_turn(self, G, field):
        le = len(self.tails)
        last = self.tails[le - 1]
        for i in range(1, le):
            self.tails[le - i] = self.tails[le - i - 1]
        self.tails[0] = self.pos

        self.new_dir(G)
        self.pos = self.pos + self.direction
        field.push(last, 0)
        n = self.push(field)
        if n == 0.99:
            self.add_tail()

    def new_dir(self, G):
        if self.direction.y == 1:
            if G == 'F':
              pass
            elif G == 'R':
                self.direction = vec(1, 0)
            elif G == 'L':
                self.direction = vec(-1, 0)
        elif self.direction.y == -1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(-1, 0)
            elif G == 'L':
                self.direction = vec(1, 0)
        elif self.direction.x == 1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(0, -1)
            elif G == 'L':
                self.direction = vec(0, 1)
        elif self.direction.x == -1:
            if G == 'F':
                pass
            elif G == 'R':
                self.direction = vec(0, 1)
            elif G == 'L':
                self.direction = vec(0, -1)

    def push(self, field):
        b = field.push(self.pos, 0.5) #0.7??
        for i in self.tails:
            field.push(i, 0.4)
        return b


# ft = 1 || ft = 2
class game:
    def __init__(self, ft, b):
        self.vectors = [vec(1, 27), vec(40, 4)]
        self.field = mymat(b)
        if ft == 1:
            self.my_snake = player_snake(self.vectors[0], 0, self.field)
            self.enemy_snake = enemy_snake(self.vectors[1], 1, self.field)

            while True:
                G = self.my_snake.count_turn(self.field)
                self.my_snake.do_turn(G, self.field)
                print(G, end='\n')

                s = input()
                self.enemy_snake.do_turn(s, field=self.field)


        elif ft == 2:
            self.my_snake = player_snake(self.vectors[1], 1, self.field)
            self.enemy_snake = enemy_snake(self.vectors[0], 0, self.field)

            while True:
                s = input()
                self.enemy_snake.do_turn(s, field=self.field)
                G = self.my_snake.count_turn(self.field)
                self.my_snake.do_turn(G, self.field)
                print(G, end='\n')

        self.field.view()

a = int(input())
b = []
for _ in range(30):
    b.append(input())

g = game(a, b)