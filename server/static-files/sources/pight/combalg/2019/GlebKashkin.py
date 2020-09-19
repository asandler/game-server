#!/usr/bin/env python3
from collections import deque  
from copy import deepcopy

def main():
    global n, k, INF
    n, k, INF, depth = 30, 40, 10**9, 4

    turn = int(input())
    turn = turn if turn == 1 else -1
    applemap = []
    for i in range(n):
        applemap.append(list(map(int, input())))
    
    NODE = Node(applemap)
    
    if turn == -1:
        NODE.snakes[0].move(input())


    while True:
        Children = NODE.children()
        max_ = -INF
        max_index = INF
        turn = 1
        for i in range(3):
            a = -megamax(Children[i], depth, -turn)
            if max_ < a:
                max_ = a
                max_index = i

        NODE.snakes[1 if turn == 1 else 0].move(['F', 'L', 'R'][max_index])
        print(['F', 'L', 'R'][max_index])
        NODE.snakes[0 if turn == 1 else 1].move(input())



def general_test():
    global n, k, INF
    n, k, INF, depth = 30, 40, 10**9, 4
    A = []

    for i in range(n):
        A.append([])
        for j in range(k):
            if abs(i - j) <= 4 and i > 7 or i == 5 or i == 24:
                A[i].append(1)
            else:
                A[i].append(0)

    NODE = Node(A)
    for _ in range(10):
        Children = NODE.children()
        max_ = -INF
        max_index = INF
        turn = 1
        for i in range(3):
            #print('!!!')
            #Children[i].output()
            a = -megamax(Children[i], depth, -turn)
            if max_ < a:
                max_ = a
                max_index = i
        print(max_)
        NODE.snakes[1].move(['F', 'L', 'R'][max_index])
        NODE.snakes[0].move('F')
        NODE.output()
        print()
    
def test():
    global n, k, INF
    n, k, INF, depth = 30, 40, 10**9, 10**5
    A = []

    for i in range(n):
        A.append([])
        for j in range(k):
            if abs(i - j) <= 4 and i > 7 or i == 5 or i == 26:
                A[i].append(1)
            else:
                A[i].append(0)

    NODE = Node(A)
    NODE.output()
    print()

    for i in range(25):
        NODE.snakes[1].move('F')
    
    NODE.snakes[1].move('R')
    NODE.output()
    print()

    for i in range(39):
        NODE.snakes[1].move('F')
    
    NODE.output()
    print()
    print(NODE.isTerminal())
    print(NODE.snakes[1].isDead())
    print(NODE.snakes[0].isDead())


def input_test():
    global n, k, INF
    n, k, INF = 30, 40, 10**9
    A = []

    for i in range(n):
        A.append([])
        for j in range(k):
            if abs(i - j) <= 4 and i > 7 or i == 5 or i == 26:
                A[i].append(1)
            else:
                A[i].append(0)

    NODE = Node(A)
    NODE.output()
    print()
    print()
    while True:
        NODE.snakes[1].move(input())
        NODE.output()
        print(NODE.euristics(NODE.time % 2))
        print('isTerminal', NODE.isTerminal())
        print('isDead_1', NODE.snakes[1].isDead())
        print('isDead_2', NODE.snakes[0].isDead())


def vector_sum(a, b):
    return (a[0] + b[0], a[1] + b[1])


class Node:
    def __init__(self, Apples):
        self.snakes = [snake(0, self), snake(1, self)]
        self.time = 0
        self.side = 0
        self.applemap = Apples

    def children(self):
        node_f = deepcopy(self)
        node_f.snakes[(self.time + 1) % 2].move('F')
        node_l = deepcopy(self)
        node_l.snakes[(self.time + 1) % 2].move('L')
        node_r = deepcopy(self)
        node_r.snakes[(self.time + 1) % 2].move('R')

        return [node_f, node_l, node_r]
    
    def isTerminal(self):
        snake_1 = self.snakes[1]
        snake_2 = self.snakes[0]
        return snake_1.isDead() or snake_2.isDead()

    def euristics(self, turn):
        snake_1 = self.snakes[1]# if turn == 1 else 0]
        snake_2 = self.snakes[0]# if turn == 1 else 1]
        if snake_1.isDead():
            return -INF
        elif snake_2.isDead():
            return INF
        else:
            return len(snake_1) + len(snake_1.digest) - len(snake_2) - len(snake_2.digest)
        

    def output(self):
        for i in range(n):
            print(str(i) if i >= 10 else (str(i) + ' '), end = '|')
            for j in range(k):
                if (i, j) == self.snakes[1].head():
                    print('&', end='|')
                elif (i, j) == self.snakes[0].head():
                    print('@', end='|')
                elif (i, j) in self.snakes[1].body or (i, j) in self.snakes[0].body:
                    print('#', end='|')
                elif self.applemap[i][j]:
                    print('*', end='|')
                else:
                    print('_', end='|')
            print()



class snake(Node):
    def __init__(self, flag, parent_node): 
        if flag:
            self.body = deque([(3, 0), (2, 0), (1, 0), (0, 0)])
            self.direction = (1, 0)
        else:
            self.body = deque([(n - 4, k - 1), (n - 3, k - 1), (n - 2, k - 1), (n - 1, k - 1)])
            self.direction = (-1, 0)

        self.digest = deque()
        self.upgrade = False
        self.parent = parent_node
        self.flag = flag

    def __eq__(self, other):
        return self.flag == other.flag

    def digest_check(self):
        if len(self.digest) > 0 and (self.digest[0] + len(self)) <= self.parent.time:
            self.upgrade = True
            self.digest.popleft()

    def __len__(self):
        return len(self.body)

    def head(self):
        return self.body[0]

    def isDead(self):
        snake_1 = self
        snake_2 = self.parent.snakes[(self.flag + 1) % 2]
        return (snake_1.body.count(snake_1.head()) + snake_2.body.count(snake_1.head())) != 1 or not (0 <= snake_1.head()[0] < n and 0 <= snake_1.head()[1] < k)


    def direction_change(self, inp_direction):
        if inp_direction == 'L':
            self.direction = (0, 1) if self.direction == (1, 0) else (0, -1) if self.direction == (-1, 0) else (-1, 0) if self.direction == (0, 1) else (1, 0)
        elif inp_direction == 'R':
            self.direction = (0, -1) if self.direction == (1, 0) else (0, 1) if self.direction == (-1, 0) else (1, 0) if self.direction == (0, 1) else (-1, 0)

    def move(self, direction):
        self.direction_change(direction)                 
        self.body.appendleft(vector_sum(self.head(), self.direction))
        self.digest_check()
        if not self.isDead() and self.parent.applemap[self.head()[0]][self.head()[1]]:
            self.parent.applemap[self.head()[0]][self.head()[1]] = 0
            self.digest.append(self.parent.time)

        if not self.upgrade:
            self.body.pop()
        else:
            self.upgrade = False
        
        self.parent.time += 1

        return 0


def megamax(node, depth, turn):
    if depth <= 0 or node.isTerminal():
        #print(depth, turn, node.isTerminal(), node.euristics(turn) * turn)
        #node.output()
        
        return node.euristics(turn) * turn
    bestval = -INF
    for child in node.children():
        bestval = max(bestval, -megamax(child, depth - 1, -turn))
    #print(depth, turn, node.isTerminal(), bestval)
    #node.output()
    
    return bestval



main()
