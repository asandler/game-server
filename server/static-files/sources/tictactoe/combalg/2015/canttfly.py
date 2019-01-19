#!/usr/bin/env python3


def val(n):
    for i in range(3):
        if sum(n[i]) == 3 or n[0][i] + n[1][i] + n[2][i] == 3:
            return float('inf')
        elif sum(n[i]) == -6 or n[0][i] + n[1][i] + n[2][i] == -6:
            return float('-inf')
    if n[0][0] + n[1][1] + n[2][2] == 3 or n[0][2] + n[1][1] + n[2][0] == 3:
        return float('inf')
    elif n[0][0] + n[1][1] + n[2][2] == -6 or \
            n[0][2] + n[1][1] + n[2][0] == -6:
        return float('-inf')
    else:
        ans = 0
        for i in range(3):
            if sum(n[i]) == 2 or n[0][i] + n[1][i] + n[2][i] == 2:
                ans += 1
            elif sum(n[i]) == -4 or n[0][i] + n[1][i] + n[2][i] == -4:
                ans -= 1
        if n[0][0] + n[1][1] + n[2][2] == 2 or \
                n[1][1] + n[2][0] + n[0][2] == 2:
            ans += 1
        elif n[0][0] + n[1][1] + n[2][2] == -4 or \
                n[1][1] + n[2][0] + n[0][2] == -4:
            ans -= 1
        return ans


def minimax(n, d, f):
    c = []
    for i in range(9):
        if not n[i//3][i % 3]:
            if f:
                h = [[n[j][k] for k in range(3)] for j in range(3)]
                h[i//3][i % 3] = 1
                c.append((h, i))
            else:
                h = [[n[j][k] for k in range(3)] for j in range(3)]
                h[i//3][i % 3] = -2
                c.append((h, i))
    if d < 0 or not c:
        return [val(n), 9]
    if f:
        b = [float('-inf'), -1]
        for i in c:
            b = max([minimax(i[0], d - 1, not f)[0], i[1]], b)
            if b[0] == float('inf'):
                return b
        return b
    else:
        b = [float('inf'), 9]
        for i in c:
            b = min([minimax(i[0], d - 1, not f)[0], i[1]], b)
            if b[0] == float('-inf'):
                return b
        return b

p = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
a = int(input())
import sys
if a == 1:
    s = minimax(p, 3, True)[1]
    sys.stdout.write(str(s)+'\n')
    p[s//3][s % 3] = 1
    while True:
        a = int(input())
        p[a//3][a % 3] = -2
        s = minimax(p, 5, True)[1]
        sys.stdout.write(str(s)+'\n')
        p[s//3][s % 3] = 1
else:
    while True:
        a = int(input())
        p[a//3][a % 3] = 1
        s = minimax(p, 5, False)[1]
        sys.stdout.write(str(s)+'\n')
        p[s//3][s % 3] = -2
