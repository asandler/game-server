#!/usr/bin/env python3

import sys

def move(snake, meals, mov):
    dv, dh = snake[0][0] - snake[1][0], snake[0][1] - snake[1][1]
    if mov == 'F':
        snake = ((snake[0][0] + dv, snake[0][1] + dh),) + snake
    elif mov == 'L':
        snake = ((snake[0][0] - dh, snake[0][1] + dv),) + snake
    elif mov == 'R':
        snake = ((snake[0][0] + dh, snake[0][1] - dv),) + snake
    if snake[-1] in meals:
        meals.discard(snake[-1])
    else:
        snake = snake[:-1]
    return (snake, meals)

def move_res(snake1, snake2):
    global b_min_x, b_min_y, b_max_x, b_max_y
    if snake1[0] in snake1[1:] or snake1[0] in snake2:
        return -1
    v, h = snake1[0][0], snake1[0][1]
    if b_min_y > v or b_max_y < v or b_min_x > h or b_max_x < h:
        return -1
    return 1
    

turn = int(input())
meals = set()
b_min_y, b_max_y, b_min_x, b_max_x = 0, 29, 0, 39
snake1, snake2 = ((3, 0), (2, 0), (1, 0), (0, 0)), ((26, 39), (27, 39), (28, 39), (29, 39))
for i in range(30):
    line = list(input())
    for j in range(40):
        if line[j] == '1':
            meals.add((i, j))
if turn == 2:
    snake1, snake2 = snake2, snake1
    mov = input()
    snake2, meals = move(snake2, meals, mov)
poss_mov = ['F', 'L', 'R']
turn_cnt = 0
for i in range(2000):
    best_mov = ''
    best_val = -10
    for mov in poss_mov:
        val = move_res(move(snake1, meals, mov)[0], snake2)
        print(val, mov, move(snake1, meals, mov)[0], file=sys.stderr)
        if val > best_val:
            best_val = val
            best_mov = mov
    print(best_mov)
    if best_mov != 'F':
        turn_cnt += 1
    if turn_cnt > 0 and turn_cnt % 4 == 0:
        b_min_x += 1
        b_min_y += 1
        b_max_x -= 1
        b_max_y -= 1
    snake1, meals = move(snake1, meals, best_mov)
    mov = input()
    snake2, meals = move(snake2, meals, mov)