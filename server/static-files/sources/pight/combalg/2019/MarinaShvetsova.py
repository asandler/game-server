#!/usr/bin/env python3

def move(num, snakes, mv, fld):
    snake = snakes[num]
    sec = snakes[1 - num]
    curr = -1
    for x, y, char in mv:
        if ((-1 < snake[-1][0] + x < 40) and (-1 < snake[-1][-1] + y < 30) 
            and (not (snake[-1][0] + x, snake[-1][-1] + y) in sec)
            and (not (snake[-1][0] + x, snake[-1][-1] + y) in snake)):
            curr = char
            break
    if fld[snake[0][0]][snake[0][1]] == '1':
        fld[snake[0][0]][snake[0][1]] = '0'
    else:
        snake = snake.pop(0)    
    return (snake, curr)

def prev(snake, mv, fld, curr):
    if fld[snake[0][0]][snake[0][1]] == '1':
        fld[snake[0][0]][snake[0][1]] = '0'
    if curr == 'F':
        snake.append((snake[-1][0] + mv[0][0], snake[-1][-1] + mv[0][1]))
    elif curr == 'R':
        snake.append((snake[-1][0] + mv[1][0], snake[-1][-1] + mv[1][1]))
    else:
        snake.append((snake[-1][0] + mv[-1][0], snake[-1][-1] + mv[-1][1]))
    if fld[snake[0][0]][snake[0][1]] == '1':
        fld[snake[0][0]][snake[0][1]] = '0'
    else:
        snake = snake.pop(0)
    return snake

num = int(input())
num -= 1
snakes = [[(0, 29), (0, 28), (0, 27), (0, 26)], [(39, 0), (39, 1), (39, 2), (39, 3)]]
mv = [[(0, 1, 'F'), (-1, 0, 'R'), (1, 0, 'L')], [(0, -1, 'F'), (1, 0, 'R'), (-1, 0, 'L')]]

fld = []
for i in range(30):
    s = input()
    fld.append(s)
if num > 0:
    snakes[1 - num] = prev(snakes[1 - num], mv[1 - num], fld, input())
cnt = 0
while (cnt < 600):
    cnt += 1
    lst = move(num, snakes, mv[num], fld)
    snakes[num] = lst[0]
    print(lst[1])
    snakes[1 - num] = prev(snakes[1 - num], mv[1 - num], fld, input())