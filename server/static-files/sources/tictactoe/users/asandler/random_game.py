#!/usr/bin/env python3

import random

pool = [i for i in range(9)]

def random_move():
    r = random.choice(pool)
    print(r)
    pool.remove(r)

turn = input()
if (int(turn) == 1):
    random_move()
while True:
    t = int(input())
    pool.remove(t)
    if pool:
        random_move()
