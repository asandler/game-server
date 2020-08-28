#!/usr/bin/env python3


def corr(x, y):
    if 0 <= x < 8 and 0 <= y < 8:
        return True
    return False


def p():
    return
    for i in u:
        print(*i)


def make_move(pos, inp):
    global u
    x, y = t_out(pos)
    cl = 3 - inp
    u[x][y] = cl
    for k in range(8):
        xi, yi = x + dx[k], y + dy[k]
        cnt = 0
        while True:
            if not corr(xi, yi) or u[xi][yi] == 0:
                cnt = 0
                break
            if u[xi][yi] == cl:
                break
            cnt += 1
            xi, yi = xi + dx[k], yi + dy[k]
        xi, yi = x + dx[k], y + dy[k]
        for i in range(cnt):
            u[xi][yi] = cl
            xi += dx[k]
            yi += dy[k]


def check_move(x, y, inp):
    mx = 0
    for k in range(8):
        xi, yi = x + dx[k], y + dy[k]
        cnt = 0
        while True:
            if not corr(xi, yi) or u[xi][yi] == 0:
                cnt = 0
                break
            if u[xi][yi] == inp:
                break
            cnt += 1
            xi, yi = xi + dx[k], yi + dy[k]
        mx += cnt
    return mx


def check_end():
    for i in range(n):
        for j in range(n):
            if u[i][j] != 0:
                continue
            if check_move(i, j, 1) != 0 or check_move(i, j, 2) != 0:
                return False
    return True


def heuristic():
    p1 = 15
    p2 = [4, 7, 10, 12, 14, 16, 18, 20]
    cnt = 0
    for i in range(n):
        for j in range(n):
            if u[i][j] == 0:
                continue
            elif u[i][j] == st_color:
                cnt += 1
            else:
                cnt -= 1
    return cnt
    if check_end():
        if cnt > 0:
            return INF
        else:
            return -INF
    for i in range(n):
        ok = True
        if u[i][0] == 0:
            ok = False
        for j in range(1, n):
            if u[i][j] != u[i][j - 1]:
                ok = False
                break
        if ok:
            if u[i][0] == st_color:
                cnt += 20
            else:
                cnt -= 20
        ok = True
        if u[0][i] == 0:
            ok = False
        for j in range(1, n):
            if u[j][i] != u[j - 1][i]:
                ok = False
                break
        if ok:
            if u[0][i] == st_color:
                cnt += 20
            else:
                cnt -= 20
    for i in range(n):
        #starting points - 0, i and i, 0
        x, y = 0, i
        ok = True
        if u[x][y] == 0:
            ok = False
        x += 1; y += 1
        while corr(x, y):
            if u[x][y] != u[0][i]:
                ok = False
                break
            x += 1; y += 1
        if ok and u[0][i] == st_color:
            cnt += p2[-i - 1]
        elif ok:
            cnt -= p2[-i - 1]
        #
        x, y = i, 0
        ok = True
        if u[x][y] == 0:
            ok = False
        x += 1; y += 1
        while corr(x, y):
            if u[x][y] != u[i][0]:
                ok = False
                break
            x += 1; y -= 1
        if ok and u[i][0] == st_color:
            cnt += p2[i]
        elif ok:
            cnt -= p2[i]
        #
        x, y = i, n - 1
        ok = True
        if u[x][y] == 0:
            ok = False
        x -= 1; y -= 1
        while corr(x, y):
            if u[x][y] != u[i][n - 1]:
                ok = False
                break
            x -= 1; y -= 1
        if ok and u[i][n - 1] == st_color:
            cnt += p2[i]
        elif ok:
            cnt -= p2[i]
        #
        x, y = n - 1, i
        ok = True
        if u[x][y] == 0:
            ok = False
        x -= 1; y -= 1
        while corr(x, y):
            if u[x][y] != u[n - 1][i]:
                ok = False
                break
            x -= 1; y -= 1
        if ok and u[n - 1][i] == st_color:
            cnt += p2[-i - 1]
        elif ok:
            cnt -= p2[-i - 1]
    return cnt


def t_in(x, y):
    return chr(ord('a') + x) + str(y + 1)


def t_out(s):
    return (d[s[0]] - 1, int(s[1]) - 1)


def mm(depth, alfa, beta, inp):
    global u
    if depth == 0 or check_end():
        return heuristic()
    u1 = []
    for i in range(n):
        u1.append(u[i][:])
    if inp == st_color:
        value = -INF
        for i in range(n):
            for j in range(n):
                if u[i][j] == 0 and check_move(i, j, 3 - inp) != 0:
                    make_move(t_in(i, j), 3 - inp)
                    value = max(value, mm(depth - 1, alfa, beta, 3 - inp))
                    u = []
                    for i1 in range(n):
                        u.append(u1[i1][:])
                    alfa = max(alfa, value)
                    if alfa >= beta:
                        break
        return value
    else:
        value = INF
        for i in range(n):
            for j in range(n):
                if u[i][j] == 0 and check_move(i, j, 3 - inp) != 0:
                    make_move(t_in(i, j), 3 - inp)
                    value = min(value, mm(depth - 1, alfa, beta, 3 - inp))
                    u = []
                    for i1 in range(n):
                        u.append(u1[i1][:])
                    beta = min(beta, value)
                    if alfa >= beta:
                        break
        return value


u = []
n = 8
#0 - empty, 1 - black, 2 - white
INF = 10000000000
for i in range(n):
    u.append([0] * n)
d = {'a' : 1, 'b' : 2, 'c' : 3, 'd' : 4,
     'e' : 5, 'f' : 6, 'g' : 7, 'h' : 8}
u[3][3] = u[4][4] = 2
u[3][4] = u[4][3] = 1
inp = int(input())
st_color = inp
dx = [-1, -1, -1, 0, 1, 1, 1, 0]
dy = [1, 0, -1, -1, -1, 0, 1, 1]
if inp == 2:
    make_move(input(), inp)
    p()
while True:
    mx = -INF - 1
    ok = True
    x = y = 0
    for i in range(n):
        for j in range(n):
            if u[i][j] == 0 and check_move(i, j, inp) > 0:
                ok = False
                cur = mm(4, -INF, INF, inp)
                if cur > mx:
                    mx = cur
                    x, y = i, j
    if ok:
        print('Skip')
    else:
        pos = t_in(x, y)
        print(pos)
        make_move(pos, 3 - inp)
    p()
    mv = input()
    if mv == '!':
        break
    if mv != 'Skip':
        make_move(mv, inp)
    p()
