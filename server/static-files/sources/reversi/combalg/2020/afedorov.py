#!/usr/bin/env python3
#-*- encoding: utf-8 -*-


from copy import deepcopy
from random import choice
#import torch
#import torch.nn as nn
#import torch.nn.functional as F


def correct(x, y):
    return 0 <= x < 8 and 0 <= y < 8


class Reversi:
    axis = {'a': 0, 'b': 1, 'c': 2, 'd': 3,
            'e': 4, 'f': 5, 'g': 6, 'h': 7}
    alf = 'abcdefgh'

    def __init__(self, game=None):
        if game is None:
            self.player = 1
            self.board = [[0]*8 for _ in range(8)]
            start_f = [['e4', 1], ['e5', -1],
                       ['d4', -1], ['d5', 1]]
            for step, color in start_f:
                x, y = self.translate(step)
                self.board[x][y] = color
            self.dp_moves = None
            self.skipped = 0
        else:
            self.player = game.player
            self.board = deepcopy(game.board)
            self.dp_moves = deepcopy(game.dp_moves)
            self.skipped = game.skipped
        self.out = False

    def detect_lines(self, x, y):
        if self.board[x][y] != 0:
            return []
        color = self.player
        dirs = [
            [0, 1], [0, -1], [1, 0], [-1, 0],
            [1, 1], [1, -1], [-1, 1], [-1, -1]
        ]
        res = []
        for k, l in dirs:
            p = []
            found = False
            i = 1
            while correct(x + k*i, y + l*i) and self.board[x + k*i][y + l*i] != 0 and not found:
                if self.board[x + k*i][y + l*i] == color:
                    found = True
                else:
                    p.append([x + k*i, y + l*i])
                i += 1
            if found:
                res.extend(p)
        return res

    def translate(self, step):
        return self.axis[step[0]], int(step[1]) - 1

    def retranslate(self, x, y):
        return self.alf[x] + str(y + 1)

    def copy(self):
        return Reversi(self)

    def move(self, step):
        if step == 'Skip':
            if self.moves():
                raise Exception('Impossible move')
            self.player *= -1
            self.dp_moves = None
            self.skipped += 1
            return self
        x, y = self.translate(step)
        if self.board[x][y] != 0:
            raise Exception('Impossible move')
        changes = self.detect_lines(x, y)
        if len(changes) == 0:
            raise Exception('Impossible move')
        self.board[x][y] = self.player
        for x, y in changes:
            self.board[x][y] = self.player
        self.player *= -1
        self.dp_moves = None
        self.skipped = 0
        return self

    def moves(self):
        if self.dp_moves is not None:
            return self.dp_moves
        res = []
        for x in range(8):
            for y in range(8):
                lines = self.detect_lines(x, y)
                if len(lines) > 0:
                    res.append([self.retranslate(x, y), lines])
        self.dp_moves = res
        return res

    def score(self):
        b, w = 0, 0
        for line in self.board:
            b += line.count(1)
            w += line.count(-1)
        return [b, w]

    def play_game(self, player_1, player_2, auto_skip=False):
        while self.skipped < 2:
            if not auto_skip or self.moves():
                if self.player == 1:
                    player_1(self)
                else:
                    player_2(self)
            else:
                self.move('Skip')
        return self.score()

    def __str__(self):
        s = 'Player: {}\n'.format({1: '+', -1: '='}[self.player])
        s += '* | 1 2 3 4 5 6 7 8\n'
        for i, line in enumerate(self.board):
            s += self.alf[i] + ' | ' + ' '.join(map(lambda x: {1: '+', 0: '_', -1: '='}[x], line)) + '\n'
        return s

'''
class MyNet(nn.Module):
    def __init__(self):
        super().__init__()

        self.fc1 = nn.Linear(64, 256)
        self.fc2 = nn.Linear(256, 32)
        self.fc3 = nn.Linear(32, 1)

    def forward(self, x):
        x = x.view(-1, 64)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.sigmoid(self.fc3(x))
        x = x*100
        return x
'''

def stupid_estimate(game):
    x, y = game.score()
    return (x+y)*(x-y)*game.player


def random_and_play_estimate(game, k=100):
    res = 0
    for i in range(k):
        copy_game = game.copy()
        copy_game.play_game(random_player, random_player)
        x, y = copy_game.score()
        if x > y:
            res += 1
        elif x == y:
            res += 0.5
    return res * game.player


def hand_estimate(game):
    def hand_count(x, y):
        x = min(x, 7-x)
        y = min(y, 7-y)
        if x == y == 0:
            return 15
        if x == 0:
            return 10
        return (4-x)*(4-y)/4

    res = 0
    for x in range(8):
        for y in range(8):
            if game.board[x][y] == game.player:
                res += (hand_count(x, y) + hand_count(y, x))/2
    return res


def input_player(game):
    step = input()
    game.move(step)


def bad_player(game):
    moves = game.moves()
    step = 'Skip'
    if moves:
        step = moves[0][0]
    if game.out:
        print(step, flush=True)
    game.move(step)


def random_player(game):
    moves = game.moves()
    step = 'Skip'
    if moves:
        step = choice(moves)[0]
    if game.out:
        print(step, flush=True)
    game.move(step)


def greed_stupid_player(game):
    moves = game.moves()
    step = 'Skip'
    if moves:
        moves.sort(key=lambda x: -len(x[1]))
        step = moves[0][0]
    if game.out:
        print(step, flush=True)
    game.move(step)


def ab_search(game, k, a, b, estimate):
    moves = game.moves()
    if not moves:
        moves = [['Skip', []]]
    if k >= 1 and game.skipped < 2:
        res = None
        for move in moves:
            copy_game = game.copy()
            copy_game.move(move[0])
            value = -ab_search(copy_game, k-1, -b, -a, estimate)[1]
            if res is None or value > res[1]:
                res = [move[0], value]
            if value > a:
                a = value
            if a >= b:
                break
        return res
    else:
        return [None, estimate(game)]


def minimax_search(game, k, estimate):
    moves = game.moves()
    if not moves:
        moves = [['Skip', []]]
    if k >= 1 and game.skipped < 2:
        res = None
        for move in moves:
            copy_game = game.copy()
            copy_game.move(move[0])
            value = -minimax_search(copy_game, k-1, estimate)[1]
            if res is None or value > res[1]:
                res = [move[0], value]
        return res
    else:
        return [None, estimate(game)]


def minimax_k_player(game, k, estimate):
    moves = game.moves()
    step = 'Skip'
    if moves:
        step = minimax_search(game, k)[0]
    if game.out:
        print(step, flush=True)
    game.move(step)


def ab_k_player(game, k, estimate):
    moves = game.moves()
    step = 'Skip'
    if moves:
        step = ab_search(game, k, -1e9, 1e9, estimate)[0]
    if game.out:
        print(step, flush=True)
    game.move(step)

'''
net = MyNet()
net.load_state_dict(torch.load('./nn/6.pth'))
def net_estimate(game):

    b = torch.tensor(game.board, dtype=torch.float)
    b = b.view(1, 8, 8)
    res = net(b)[0]
    return res
'''

if __name__ == '__main__':
    def player(game):
        ab_k_player(game, 4, hand_estimate)

    def player_2(game):
        ab_k_player(game, 4, stupid_estimate)

    rg = Reversi()
    rg.out = True

#    rg.play_game(player, player_2)
#    print(rg)
#    print(rg.score())

#    rg = Reversi()
#    rg.out = True

#    rg.play_game(player_2, player)
#    print(rg)
#    print(rg.score())

    order = int(input())
    if order == 1:
        rg.play_game(player, input_player)
    else:
        rg.play_game(input_player, player)
