#!/usr/bin/env python3
def posishon_after_going(A, i0, j0, tipe):
    B = []
    for a in A:
        B.append(list(a))
    for i in range(i0 - 1, -1, -1):
        if A[i][j0] == tipe:
            for i10 in range(i0 - 1, i - 1, -1):
                B[i10][j0] = tipe
            break
        elif A[i][j0] == 0:
            break
    for i in range(i0 + 1, 8):
        if A[i][j0] == tipe:
            for i10 in range(i0 + 1, i + 1):
                B[i10][j0] = tipe
            break
        elif A[i][j0] == 0:
            break
    for j in range(j0 - 1, -1, -1):
        if A[i0][j] == tipe:
            for j10 in range(j0 - 1, j - 1, -1):
                B[i0][j10] = tipe
            break
        elif A[i0][j] == 0:
            break
    for j in range(j0 + 1, 8):
        if A[i0][j] == tipe:
            for j10 in range(j0 + 1, j + 1):
                B[i0][j10] = tipe
            break
        elif A[i0][j] == 0:
            break

    for i in range(1, min(i0, j0) + 1):
        if A[i0 - i][j0 - i] == tipe:
            for i10 in range(1, i + 1):
                B[i0 - i10][j0 - i10] = tipe
            break
        elif A[i0 - i][j0 - i] == 0:
            break
    for i in range(1, min(i0 + 1, 8 - j0)):
        if A[i0 - i][j0 + i] == tipe:
            for i10 in range(1, i + 1):
                B[i0 - i10][j0 + i10] = tipe
            break
        elif A[i0 - i][j0 + i] == 0:
            break
    for i in range(1, min(8 - i0, j0 + 1)):
        if A[i0 + i][j0 - i] == tipe:
            for i10 in range(1, i + 1):
                B[i0 + i10][j0 - i10] = tipe
            break
        elif A[i0 + i][j0 - i] == 0:
            break
    for i in range(1, min(8 - i0, 8 - j0)):
        if A[i0 + i][j0 + i] == tipe:
            for i10 in range(1, i + 1):
                B[i0 + i10][j0 + i10] = tipe
            break
        elif A[i0 + i][j0 + i] == 0:
            break
    if A != B:
        B[i0][j0] = tipe
        return [True, B]
    else:
        return [False, []]

#--------------------------------------------
#--------------------------------------------
#--------------------------------------------

def good_print_A(A):
    for i in range(8):
        print(*A[i])

#--------------------------------------------
#--------------------------------------------
#--------------------------------------------

def is_going_ok(A, i0, j0, tipe):


    if A[i0][j0] == 0:
        for i in range(i0 - 1, -1, -1):
            if A[i][j0] == tipe:
                return True
            elif A[i][j0] == 0:
                break
        for i in range(i0 + 1, 8):
            if A[i][j0] == tipe:
                return True
            elif A[i][j0] == 0:
                break
        for j in range(j0 - 1, -1, -1):
            if A[i0][j] == tipe:
                return True
            elif A[i0][j] == 0:
                break
        for j in range(j0 + 1, 8):
            if A[i0][j] == tipe:
                return True
            elif A[i0][j] == 0:
                break

        for i in range(1, min(i0, j0) + 1):
            if A[i0 - i][j0 - i] == tipe:
                return True
            elif A[i0 - i][j0 - i] == 0:
                break
        for i in range(1, min(i0 + 1, 8 - j0)):
            if A[i0 - i][j0 + i] == tipe:
                return True
            elif A[i0 - i][j0 + i] == 0:
                break
        for i in range(1, min(8 - i0, j0 + 1)):
            if A[i0 + i][j0 - i] == tipe:
                return True
            elif A[i0 + i][j0 - i] == 0:
                break
        for i in range(1, min(8 - i0, 8 - j0)):
            if A[i0 + i][j0 + i] == tipe:
                return True
            elif A[i0 + i][j0 + i] == 0:
                break
        return False

    else:
        return False

#--------------------------------------------
#--------------------------------------------
#--------------------------------------------

def all_goings(A, tipe):
    C = []
    for i in range(8):
        for j in range(8):
            if is_going_ok(A, i, j, tipe):
                C.append([i, j])
    return C

#--------------------------------------------
#--------------------------------------------
#--------------------------------------------

def is_my_pozishon_good(A):
    tip_10 = 1
    tip_11 = 2
    tip_20 = 5
    tip_21 = 8
    tip_22 = 1
    tip_23 = 5
    tip_3 = 25
    x = 0
    for i in range(8):
        for j in range(8):
            if A[i][j] == 1:
                if (i == 0 or i == 7) and (j == 0 or j == 7):
                    x += tip_3
                elif (i == 0 or i == 7) or (j == 0 or j == 7):
                    if (i == 0 or i == 7):
                        if A[i][j + 1] < 2 and A[i][j - 1] < 2:
                            x += tip_20
                        elif A[i][j + 1] == 2 and A[i][j - 1] == 2:
                            x += tip_21
                        elif A[i][j + 1] + A[i][j - 1] == 2:
                            x += tip_22
                        else:
                            x += tip_23
                    else:
                        if A[i + 1][j] < 2 and A[i - 1][j] < 2:
                            x += tip_20
                        elif A[i + 1][j] == 2 and A[i - 1][j] == 2:
                            x += tip_21
                        elif A[i + 1][j] + A[i - 1][j] == 2:
                            x += tip_22
                        else:
                            x += tip_23
                else:
                    if A[i + 1][j] < 2 and A[i - 1][j] < 2 and A[i + 1][j + 1] < 2 and A[i - 1][j + 1] < 2 and A[i + 1][j - 1] < 2 and A[i - 1][j - 1] < 2:
                        x += tip_11
                    else:
                        x += tip_10
            elif A[i][j] == 2:
                if (i == 0 or i == 7) and (j == 0 or j == 7):
                    x -= tip_3
                elif (i == 0 or i == 7) or (j == 0 or j == 7):
                    if (i == 0 or i == 7):
                        if A[i][j + 1] != 1 and A[i][j - 1] != 1:
                            x -= tip_20
                        elif A[i][j + 1] == 1 and A[i][j - 1] == 1:
                            x -= tip_21
                        else:
                            x -= tip_23
                    else:
                        if A[i + 1][j] != 1 and A[i - 1][j] != 1:
                            x -= tip_20
                        elif A[i + 1][j] == 1 and A[i - 1][j] == 1:
                            x -= tip_21
                        else:
                            x -= tip_23
                else:
                    if A[i + 1][j] != 1 and A[i - 1][j] != 1 and A[i + 1][j + 1] != 1 and A[i - 1][j + 1] != 1 and A[i + 1][j - 1] != 1 and A[i - 1][j - 1] != 1:
                        x -= tip_11
                    else:
                        x -= tip_10

    return x

#--------------------------------------------
#--------------------------------------------
#--------------------------------------------

def X(D):
    Goings = all_goings(D, 2)
    worest_x = 179179
    for going in Goings:
        tr, D2 = posishon_after_going(D, going[0], going[1], 1)
        if tr:
            best_x = -179179
            GGoings = all_goings(D2, 1)
            for ggoing in GGoings:
                Tr, D3 = posishon_after_going(D2, ggoing[0], ggoing[1], 1)
                if Tr:
                    if best_x < is_my_pozishon_good(D3):
                        best_x = is_my_pozishon_good(D3)
            if worest_x > best_x and best_x != -179179:
                worest_x = best_x
    if worest_x == 179179:
        return is_my_pozishon_good(D)
    else:
        return worest_x

# --------------------------------------------
# --------------------------------------------
# --------------------------------------------

normal_goings = [-1]
A = []
koll_of_null = 60
for i in range(8):
    A.append([0] * 8)
www = int(input())
if www == 2:
    A[3][3] = 1
    A[4][4] = 1
    A[4][3] = 2
    A[3][4] = 2
    sSs = input()
    i179, j179 = ord(sSs[0]) - ord('a'), int(sSs[1]) - 1
    A = posishon_after_going(A, i179, j179, 2)[1]
else:
    A[3][3] = 2
    A[4][4] = 2
    A[4][3] = 1
    A[3][4] = 1
while True:
    Goings = all_goings(A, 1)
    best_Going = [-1, -1]
    best_X = -179179
    posishon_after_best_going = []
    for going in Goings:
        tr, D = posishon_after_going(A, going[0], going[1], 1)
        if tr:
            if X(D) > best_X:
                best_X = X(D)
                best_Going = going
                posishon_after_best_going = list(D)
    if best_Going == [-1, -1]:
        print('Skip')
    else:
        print(chr(ord('a') + best_Going[0]), best_Going[1] + 1, sep='')
        A = posishon_after_best_going
    #good_print_A(A)
    sSs = input()
    if sSs != 'Skip':
        i179, j179 = ord(sSs[0]) - ord('a'), int(sSs[1]) - 1
        A = posishon_after_going(A, i179, j179, 2)[1]
