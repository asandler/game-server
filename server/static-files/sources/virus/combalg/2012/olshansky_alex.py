#!/usr/bin/env python3

table = [['_'] * 10 for i in range(10)]
INF = 10 ** 4
moves = [(1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (1, -1), (-1, 1), (-1, -1)]

def enemy(virus):
    return ['X', 'O'][not ['X', 'O'].index(virus)]

def dead(virus):
    return ['Z', '0'][['X', 'O'].index(virus)]

def dist(point):
    try:
        return min(min(i for i in point), min(9 - i for i in point))
    except:
        return INF

def inside(point):
    return min(0 <= i <= 9 for i in point)

def surroundings(x, y):
    res = []

    for dx, dy in moves:
        newx, newy = x + dx, y + dy

        if inside((newx, newy)):
            res.append((newx, newy))
    return res

def find(target):
    for x in range(10):
        for y in range(10):
            if table[x][y] == target:
                return (x, y)

def kill(x, y, virus):
    for newx, newy in surroundings(x, y):
        if table[newx][newy] == virus:
            return (newx, newy)

def do(s, virus):
    res = None

    for i in range(0, len(s), 2):
        x, y = map(int, [s[i], s[i + 1]])

        if table[x][y] == '_':
            table[x][y] = virus
        else:
            table[x][y] = dead(enemy(virus))
            res = (x, y)
    return res

def bfs(point, target, allowed):
    try:
        x, y = point
    except:
        return (INF, None)
    mark = [[0] * 10 for i in range(10)]
    d = [[INF] * 10 for i in range(10)]
    p = [[(-1, -1)] * 10 for i in range(10)]
    queue = [(x, y)]
    d[x][y] = 0
    mark[x][y] = 1

    while len(queue) > 0:
        x, y = queue[0]
        queue.pop(0)
        
        for newx, newy in surroundings(x, y):
            if (table[newx][newy] in allowed or table[newx][newy] == target) and not mark[newx][newy]:
                mark[newx][newy] = 1
                d[newx][newy] = d[x][y] + 1
                p[newx][newy] = (x, y)
                queue.append((newx, newy))
                if table[newx][newy] == target:
                    return (d[newx][newy], (newx, newy, p))
    return (INF, None)

def path(x, y, p):
    res, s = [], ''

    while (x, y) != (-1, -1):
        res.append((x, y))
        x, y = p[x][y]
    res.reverse()

    for point in res:
        s += str(point[0]) + str(point[1])
    return s[2:]

def create(virus):
    res = None

    for x in range(10):
        for y in range(10):
            if table[x][y] == virus:
                for newx, newy in surroundings(x, y):
                    if table[newx][newy] == '_' and dist((newx, newy)) < dist(res):
                        res = (newx, newy)

    if res != None:
        return str(res[0]) + str(res[1])

def strike(virus):
    res = None

    for x in range(10):
        for y in range(10):
            if table[x][y] == virus or (table[x][y] == dead(enemy(virus)) and bfs((x, y), virus, [dead(enemy(virus))])[0] < INF):
                for newx, newy in surroundings(x, y):
                    if table[newx][newy] == enemy(virus) and bfs((newx, newy), virus, [dead(virus), enemy(virus), '_'])[0] <= bfs(res, virus, [dead(virus), enemy(virus), '_'])[0]:
                        res = (newx, newy)

    if res != None:
        return str(res[0]) + str(res[1])

def counter_strike(x, y, virus):
    res = None

    for newx, newy in surroundings(x, y):
        if inside((newx, newy)) and table[newx][newy] == virus and bfs((newx, newy), enemy(virus), ['_', dead(enemy(virus))])[0] < bfs(res, enemy(virus), ['_', dead(enemy(virus))])[0]:
            res = (newx, newy)

    if bfs(res, enemy(virus), ['_', dead(enemy(virus))])[1] != None:
        x, y, p = bfs(res, enemy(virus), ['_', dead(enemy(virus))])[1]
        s = path(x, y, p)
    
        if len(s) <= 6:
            return s

def print_table():
    for i in range(9, -1, -1):
        print(i, end = ' ')
        for j in range(10):
            print(table[j][i], end = '')
        print()
    print('  ' + ''.join(list(map(str, list(range(10))))))

n = int(input())
second = 0

if n == 1:
    print('000102')
    do('000102', 'X')
    virus = 'X'
else:
    virus = 'O'
    second = 1

while True:
    s = input()

    if s == 'p':
        print_table()
        continue
    res = ""
    point = do(s, enemy(virus))

    if second:
        print('999897')
        do('999897', 'O')
        second = 0
        continue

    if point != None:
        x, y = point

        if counter_strike(x, y, virus) != None:
            res += counter_strike(x, y, virus)
            do(counter_strike(x, y, virus), virus)

    it = 0

    try:
        while len(res) < 6 and it < 3:
            if strike(virus) == None:
                res += create(virus)
                do(create(virus), virus)
            else:
                res += strike(virus)
                do(strike(virus), virus)
            it += 1
    except:
        pass
    print(res)
