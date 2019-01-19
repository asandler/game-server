#!/usr/bin/env python3

def win_game(player, pole):
    for i in range(3):
        if max(pole[i]) == min(pole[i]) == player:
            return True
    for j in range(3):
        if pole[0][j] == pole[1][j] == pole[2][j] == player:
            return True
    if pole[0][0] == pole[1][1] == pole[2][2] == player:
        return True
    elif pole[0][2] == pole[1][1] == pole[2][0] == player:
        return True
    return False
def sosed_every(player, pole):
    s = 0
    for i in range(3):
        p1 = 0
        p2 = 0
        for j in range(3):
            if pole[i][j] == player:
                p1 += 1
            elif pole[i][j] != 0:
                p2 += 1
        if p1 == 2 and p2 == 0:
            s += 2
        elif p1 == 1 and p2 == 0:
            s += 1
    p1 = 0
    p2 = 0
    for i in range(3):
        if pole[i][j] == player:
            p1 += 1
        elif pole[i][j] != 0:
            p2 += 1
    if p1 == 2 and p2 == 0:
        s += 2
    elif p1 == 1 and p2 == 0:
        s += 1
    p1 = 0
    p2 = 0
    for i in range(3):
        if pole[i][2-j] == player:
            p1 += 1
        elif pole[i][2-j] != 0:
            p2 += 1
    if p1 == 2 and p2 == 0:
        s += 2
    elif p1 == 1 and p2 == 0:
        s += 1
    return s
def ugoloc(player, pole):
    s = 0
    pole1 = [pole[0][0:], pole[1][0:2] + [0], [pole[2][0]] + [0, 0]]
    if sosed_every(player, pole1) >=2:
        s += 1
    pole1 = [pole[0][0:], [0] + pole[1][1:3], [0, 0] + [pole[2][2]]]
    if sosed_every(player, pole1) >= 2:
        s += 1
    pole1 = [[pole[0][0]] + [0, 0], pole[1][0:2] + [0], pole[2][0:]]
    if sosed_every(player, pole1) >= 2:
        s += 1
    pole1 = [[0, 0] + [pole[0][2]], [0] + pole[1][1:3], pole[2][0:]]
    if sosed_every(player, pole1) >= 2:
        s += 1
    return s
def analis_pos(player1, pole):
    if player1 == 1:
        player2 = 2
    else:
        player2 = 1
    if win_game(player1, pole):
        return float('inf')
    elif win_game(player2, pole):
        return float('-inf')
    res = 0
    res += sosed_every(player1, pole)
    res -= 1000 * sosed_every(player2, pole)
    res += 2 * ugoloc(player1, pole)
    res -= 10000 * ugoloc(player2, pole)
    return res


def analis(max_, pole, first=False, deth=-1):
    status = analis_pos(1, pole)
    if status == float('inf') or status == float('-inf'):
        return status, ''
    if deth == 0:
        return status, ''
    if (0 not in pole[0]) and (0 not in pole[1]) and (0 not in pole[2]):
        return status, ''
    list = []
    list2 = []
    list3 = []
    if max_:
        for ij in range(9):
            i = ij // 3
            j = ij % 3
            if pole[i][j] == 0:
                pole1 = [pole[0][0:], pole[1][0:], pole[2][0:]]
                pole1[i][j] = 1
                an = analis(False, pole1, deth=deth - 1)
                list.append(an[0])
                list2.append(an[1])
                list3.append(i*3+j)
                if an[0] == float('inf'):
                    break
        if first:
            print(list, list2, list3)
        return max(list), str(list3[list.index(max(list))]) + list2[list.index(max(list))]
    else:
        for ij in range(9):
            i = ij // 3
            j = ij % 3
            if pole[i][j] == 0:
                pole1 = [pole[0][0:], pole[1][0:], pole[2][0:]]
                pole1[i][j] = 2
                an = analis(True, pole1, deth=deth - 1)
                list.append(an[0])
                list2.append(an[1])
                list3.append(i*3+j)
                if an[0] == float('-inf'):
                    break
        if first:
            print(list, list2, list3)
        return min(list), str(list3[list.index(min(list))]) + list2[list.index(min(list))]


pole = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
check = input()
if check == '1':
    hod = 4
    print(hod, end='\n')
    pole[hod // 3][hod % 3] = 1
while True:
    hod = int(input())
    pole[hod // 3][hod % 3] = 2
    a = analis(True, pole, deth=5)
    hod = int(a[1][0])
    print(hod, end='\n')
    pole[hod // 3][hod % 3] = 1