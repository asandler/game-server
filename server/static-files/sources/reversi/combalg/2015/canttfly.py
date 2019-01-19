#!/usr/bin/env python3


def mtdf(black, white, g, d, f):
    up = float('inf')
    low = float('-inf')
    while low < up:
        b = g
        if g == low:
            b += 1
        g, h = alphabeta(black, white, b-1, b, d, f, first=True)
        if g < b:
            up = g
        else:
            low = g
    return h


def alphabeta(black, white, a, b, d, f, first=False, skip=False):
    global pos
    at, bt = a, b
    s = str(black) + ',' + str(white) + ',' + str(f)
    if s in pos and pos[s][1] >= d:
        if not first and pos[s][2]:
            return pos[s][0]
        if pos[s][2] is None:
            a = max(a, pos[s][0])
        else:
            b = min(b, pos[s][0])
        if a >= b and not first:
            return pos[s][0]
    if not d:
        return val(black, white)
    else:
        t = False
        if f:
            if first:
                for i in range(64):
                    b0, w0 = can_step(black, white, i)
                    if b0 or w0:
                        t = True
                        ans = alphabeta(b0, w0, a, b, d-1, not f)
                        if ans >= a:
                            a = ans
                            h = i
                        if a == float('inf') or b <= a:
                            break
                ans = a
            else:
                for i in range(64):
                    b0, w0 = can_step(black, white, i)
                    if b0 or w0:
                        t = True
                        ans = alphabeta(b0, w0, a, b, d-1, not f)
                        if ans > a:
                            a = ans
                        if a == float('inf') or b <= a:
                            break
                ans = a
        else:
            if first:
                for i in range(64):
                    w0, b0 = can_step(white, black, i)
                    if b0 or w0:
                        t = True
                        ans = alphabeta(b0, w0, a, b, d-1, not f)
                        if ans <= b:
                            b = ans
                            h = i
                        if b == float('-inf') or b <= a:
                            break
                ans = b
            else:
                for i in range(64):
                    w0, b0 = can_step(white, black, i)
                    if b0 or w0:
                        t = True
                        ans = alphabeta(b0, w0, a, b, d-1, not f)
                        if ans < b:
                            b = ans
                        if b == float('-inf') or b <= a:
                            break
                ans = b
        if not t:
            if skip:
                ans = val(black, white)
            else:
                ans = alphabeta(black, white, a, b, d-1, not f, skip=True)
        flag = True
        if at >= ans:
            flag = False
        elif bt <= ans:
            flag = None
        if s not in pos or pos[s][1] <= d:
            pos[s] = ans, d, flag
        if first:
            return ans, h
        else:
            return ans


def can_step(black, white, i):
    b, w = black, white
    j0 = 1 << i
    if b & j0 or w & j0:
        return False, False
    i0 = i + 1
    j = j0
    b0, w0 = b, w
    while not i0 >> 3 ^ i >> 3:
        j = j << 1
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 += 1
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i - 1
    j = j0
    b0, w0 = b, w
    while not i0 >> 3 ^ i >> 3:
        j = j >> 1
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 -= 1
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i + 8
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ 8:
        j = j << 8
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 += 8
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i - 8
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ -1:
        j = j >> 8
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 -= 8
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i + 9
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ 8 and i0 & 7:
        j = j << 9
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 += 9
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i - 9
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ -1 and i0 & 7 ^ 7:
        j = j >> 9
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 -= 9
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i + 7
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ 8 and i0 & 7 ^ 7:
        j = j << 7
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 += 7
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    i0 = i - 7
    j = j0
    b0, w0 = b, w
    while i0 >> 3 ^ -1 and i0 & 7:
        j = j >> 7
        if j & white:
            w0 = w0 & ~j
            b0 = b0 ^ j
            i0 -= 7
        elif j & black:
            b, w = b0, w0
            break
        else:
            break
    if w ^ white:
        return b ^ j0, w
    else:
        return False, False


def val(black, white):
    b, w = 0, 0
    ba, wa = 0, 0
    bx, wx = 0, 0
    bc, wc = 0, 0
    for i in range(64):
        b += black & 1
        w += white & 1
        if not i or not i ^ 7 or not i ^ 56 or not i ^ 63:
            ba += black & 1
            wa += white & 1
        elif not i ^ 9 or not i ^ 14 or not i ^ 54 or not i ^ 49:
            bx += black & 1
            wx += white & 1
        elif not i ^ 1 or not i ^ 8 or not i ^ 15 or not i ^ 6 or not i ^ 48 \
                or not i ^ 57 or not i ^ 55 or not i ^ 62:
            bc += black & 1
            wc += white & 1
        black = black >> 1
        white = white >> 1
    return b - w + (ba - wa << 4) - (bx - wx << 3) - (bc - wc << 2)


def stable(black, white, i):
    pass


def main():
    b = 34628173824
    w = 68853694464
    a = int(input())
    if a & 1:
        while True:
            t = False
            for i in range(64):
                b0, w0 = can_step(b, w, i)
                if b0 or w0:
                    t = True
                    break
            if t:
                g = alphabeta(b, w, float('-inf'), float('inf'), 3, True)
                u = mtdf(b, w, g, 5, True)
                b, w = can_step(b, w, u)
                print(chr((u >> 3) + 97) + str((u & 7) + 1))
            else:
                print('Skip')
            a = input()
            if a != 'Skip':
                a = (ord(a[0]) - 97 << 3) + int(a[1]) - 1
                w, b = can_step(w, b, a)
    else:
        while True:
            a = input()
            if a != 'Skip':
                a = (ord(a[0]) - 97 << 3) + int(a[1]) - 1
                b, w = can_step(b, w, a)
            t = False
            for i in range(64):
                b0, w0 = can_step(w, b, i)
                if b0 or w0:
                    t = True
                    break
            if t:
                g = alphabeta(b, w, float('-inf'), float('inf'), 3, False)
                u = mtdf(b, w, g, 5, False)
                w, b = can_step(w, b, u)
                print(chr((u >> 3) + 97) + str((u & 7) + 1))
            else:
                print('Skip')

pos = dict()
main()
