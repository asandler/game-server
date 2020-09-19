#!/usr/bin/env python3
from collections import deque
import random
from copy import deepcopy

INF = 1e9

NORTH = (-1, 0)
EAST = (0, 1)
SOUTH = (1, 0)
WEST = (0, -1)

#KNOWN = dict()

def rotate(cdir, cmd):
    if cmd == "L":
        return (-cdir[1], cdir[0])
    elif cmd == "R":
        return (cdir[1], -cdir[0])
    else:
        return cdir

def move(coord, delta):
    return [coord[0] + delta[0], coord[1] + delta[1]]

class State:
    def __init__(self):
        self.food = set()
        # x[1] or x[-1]
        self.snakes = [None, deque(([3, 0], [2, 0], [1, 0], [0, 0])), deque(([26, 39], [27, 39], [28, 39], [29, 39]))]
        self.size = [None, 4, 4]
        self.dir = [None, SOUTH, NORTH]
        self.eatenFood = 0

def init():
    if input() == "1":
        turn = 1
    else:
        turn = -1
    state = State()
    for i in range(30):
        for j, c in enumerate(input()):
            if c == "1":
                if turn == 1:
                    state.food.add((i, j))
                else:
                    state.food.add((29 - i, 39 - j))
    return turn, state

def apply(state, turn, cmd):
    if cmd not in "LRF":
        raise Exception("")
    state.dir[turn] = rotate(state.dir[turn], cmd)
    state.snakes[turn].appendleft(move(state.snakes[turn][0], state.dir[turn]))
    head = state.snakes[turn][0]
    if (head[0],head[1]) in state.food:
        state.eatenFood += 1
    tail = state.snakes[turn][-1]
    if (tail[0], tail[1]) in state.food:
        state.food.remove((tail[0], tail[1]))
        state.eatenFood -= 1
    else:
        state.snakes[turn].pop()

def predict(state, turn):
    return negamax(state, 4, turn)[0]

def negamax(state, depth, turn):
    if depth == 0 or isTerm(state):
        return "F", turn * score(state, turn)
    bestVal = -INF
    bestCmd = "F"
    for cmd, child in getChildren(state, turn):
        val = -negamax(child, depth - 1, -turn)[1]
        if val > bestVal:
            bestVal = val
            bestCmd = cmd
        del child
    return bestCmd, bestVal

def score(state, turn):
    isT = isTerm(state)
    if isT == 2:
        if len(state.snakes[turn]) > len(state.snakes[-turn]):
            return INF
        elif len(state.snakes[turn]) < len(state.snakes[-turn]):
            return -INF
        return 0
    if isT == turn:
        return -INF
    if isT == -turn:
        return INF
    # Take nearest food into account
    return len(state.snakes[turn]) - len(state.snakes[-turn]) + state.eatenFood

def isValid(state, pos):
    return 0 <= pos[0] < 30 and 0 <= pos[1] < 40 and pos not in state.snakes[1] and pos not in state.snakes[-1]

def getChildren(state, turn):
    r = []
    if isValid(state, move(state.snakes[turn][0], state.dir[turn])):
        _state = deepcopy(state)
        apply(_state, turn, "F")
        r.append(("F", _state))
    if isValid(state, move(state.snakes[turn][0], rotate(state.dir[turn], "L"))):
        _state = deepcopy(state)
        apply(_state, turn, "L")
        r.append(("L", _state))
    if isValid(state, move(state.snakes[turn][0], rotate(state.dir[turn], "R"))):
        _state = deepcopy(state)
        apply(_state, turn, "R")
        r.append(("R", _state))
    return r

# Not entirely accurate!
def isTerm(state, turn=0):
    if turn != 0:
        return (not (isValid(state, move(state.snakes[turn][0], state.dir[turn])) or \
                     isValid(state, move(state.snakes[turn][0], rotate(state.dir[turn], "L"))) or \
                     isValid(state, move(state.snakes[turn][0], rotate(state.dir[turn], "R"))))) or \
                     len(state.food) == 0
    t1, t2 = isTerm(state, 1), isTerm(state, -1)
    if t1 and t2:
        return 2
    if t1 and not t2:
        return -1
    if not t1 and t2:
        return 1
    if not t1 and not t2:
        return 0

def log(state):
    print("=====")
    for i in range(30):
        for j in range(40):
            c = "#" if (i, j) in state.food else "_"
            if [i, j] in state.snakes[1]:
                if [i, j] == state.snakes[1][0]:
                    c = "[" + c + "]"
                else:
                    c = "(" + c + ")"
            elif [i, j] in state.snakes[-1]:
                if [i, j] == state.snakes[-1][0]:
                    c = "<" + c + ">"
                else:
                    c = "{" + c + "}"
            else:
                c = " " + c + " "
            print(c, end="")
        print()
    print("=====")

def main():
    turn, STATE = init()
    while True:
        if turn == -1:
            #cmd = predict(STATE, turn)
            #print(">", cmd)
            cmd = input()
        else:
            #cmd = input()
            cmd = predict(STATE, turn)
            print(cmd)
        apply(STATE, turn, cmd)
        #log(STATE)
        #if isTerm(STATE):
            #print(isTerm(STATE))
            #break
        turn = -turn
    #log(STATE)

main()


#1
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000011111111111100000000000000000000000
#0010000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#1110001100000000000000000000000000000000
#0000000000000000011111111111000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000011111111100000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000011111111000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
#0000000000000000000000000000000000000000
