#!/usr/bin/env python3
from copy import deepcopy
turn = -2*int(input()) + 3
#ход: 1 если первый, -1 иначе
my_snake = (1-turn)//2
#номер змейки: 0 если первый, 1 иначе
height=30
#высота поля
width=40
#ширина поля
a = [list(input()) for i in range(height)]
#a - еда на поле
b = [[i, j] for i in range(height) for j in range(width) if a[i][j] == '1']
#b - список еды
snake=[[[[0, 0], [1, 0], [2, 0], [3, 0]], 'S'],
       [[[height - 1, width - 1], [height - 2, width - 1], [height - 3, width - 1], [height - 4, width - 1]], 'N']]
#змейки в формате [[[координаты хвоста], [координаты следующей клетки], ..., [координаты головы]], направление головы относительно поля]
#N - север, S - юг, E - восток, W - запад


def f1(pos, direction):
    #принимает координаты точки и направление, куда идет точка, возвращает новое положение точки
    if direction == 'N':
        #если на север, то вверх
        return [pos[0] - 1, pos[1]]
    if direction == 'S':
        #если на юг, то вниз
        return [pos[0] + 1, pos[1]]
    if direction == 'E':
        #если на восток, то вправо
        return [pos[0], pos[1] + 1]
    if direction == 'W':
        #если на запад то влево
        return [pos[0], pos[1] - 1]


def f2(ex, direction):
    #принимает направление относительно головы, направление головы, возвращает направление относительно поля
    d = ['N', 'E', 'S', 'W']
    #направления по часовой стрелке
    if ex == 'F':
        #если прямо, то направление не меняется
        return direction
    if ex == 'R':
        #если вправо, то +1 по часовой стрелке
        return d[(d.index(direction) + 1)%4]
    if ex == 'L':
        #если влево, то -1 по часовой стрелке
        return d[(d.index(direction) - 1)%4]


def f2_2(direct, direction):
    #принимает направление относительно поля, направление головы, возвращает направление относительно головы
    #f2_2 - обратная к f2
    if f2('F', direction) == direct:
        return 'F'
    if f2('R', direction) == direct:
        return 'R'
    if f2('L', direction) == direct:
        return 'L'


def f3(my_snake, b, snake):
    #возвращает возможные ходы змейки my_snake
    res = []
    #результат относительно поля
    head = snake[my_snake][0][-1]
    #положение головы
    used = snake[my_snake][0][1:] + snake[(my_snake + 1)%2][0]
    #клетки, в которые нельзя ходить
    if [snake[my_snake][0][0][0], snake[my_snake][0][0][1]] in b:
        #если хвост не уползет
        used.append(snake[my_snake][0][0])
        #то туда нельзя ползти
    if (head[0] < height - 1) and [head[0] + 1, head[1]] not in used:
        #если на юге есть клетка и в неё можно пойти
        res.append('S')
        #то туда можно пойти
    if (head[0] > 0) and [head[0] - 1, head[1]] not in used:
        #если на севере есть клетка и в неё можно пойти
        res.append('N')
        #то туда можно пойти
    if (head[1] < width - 1) and [head[0], head[1] + 1] not in used:
        #если на востоке есть клетка и в неё можно пойти
        res.append('E')
        #то туда можно пойти
    if (head[1] > 0) and [head[0], head[1] - 1] not in used:
        #если на западе есть клетка и в неё можно пойти
        res.append('W')
        #то туда можно пойти
    rez = [f2_2(direct, snake[my_snake][1]) for direct in res]
    #результат относительно головы
    return rez

'''
def f4(my_snake):
    #получает ситуацию, возвращает рандомный возможный ход
    l = f3(my_snake, b, snake)
    #l - список возможных ходдов
    if l == []:
        #если l пусто, то возможных ходов нет
        return None
    else:
        #иначе, выберем ход рандомно
        return random.choice(l)
'''

def f5(ex, my_snake):
    #получает ход (F, R, или L) и номер змейки, изменяет ситуацию
    global a
    global snake
    #буду изменять массив еды и змейку
    ex = f2(ex, snake[my_snake][1])
    #заменим ход относительно головы на ход относительно поля
    new_head = f1(snake[my_snake][0][-1], ex)
    #новое положение головы
    snake[my_snake][0].append(new_head)
    #добавим положение головы в тело змейки
    if [snake[my_snake][0][0][0], snake[my_snake][0][0][1]] in b:
        #если есть пища в хвосте
        b.remove([snake[my_snake][0][0][0], snake[my_snake][0][0][1]])
        #переварим её
    else:
        #иначе
        snake[my_snake][0] = snake[my_snake][0][1:]
        #уползем
    snake[my_snake][1] = ex
    #меняем направление


def f6(ex, my_snake, b, snake):
    #возвращает изменившуюся ситуацию
    b, snake = deepcopy(b), deepcopy(snake)
    #копируем данные
    ex = f2(ex, snake[my_snake][1])
    #заменим ход относительно головы на ход относительно поля
    new_head = f1(snake[my_snake][0][-1], ex)
    #новое положение головы
    snake[my_snake][0].append(new_head)
    #добавим положение головы в тело змейки
    if [snake[my_snake][0][0][0], snake[my_snake][0][0][1]] in b:
        #если есть пища в хвосте
        b.remove([snake[my_snake][0][0][0], snake[my_snake][0][0][1]])
        #переварим её
    else:
        #иначе
        snake[my_snake][0] = snake[my_snake][0][1:]
        #уползем
    snake[my_snake][1] = ex
    #меняем направление
    return [b, snake]


def f7(situation, turn):
    #возвращает эвристическую оценку ситуации
    b, snake = situation
    #распакуем данные
    if b == []:
        #если еды нет
        return (len(snake[my_snake][0]) - len(snake[(my_snake + 1)%2][0]))*10**5
        #за победу - 10**5 очков
    if turn == 1 and f3(my_snake, b, snake) == []:
        #если наш ход, а сходить нельзя, то мы проиграем
        return -10**5
    rez = (10*len(snake[my_snake][0]) - 2*len(snake[(my_snake + 1)%2][0]))
    #чем длиннее мы и короче противник, тем лучше
    head = snake[my_snake][0][-1]
    #положение головы
    used = snake[my_snake][0] + snake[(my_snake + 1)%2][0]
    #клетки, в которых еда уже поглощена
    for apple in b:
        #для каждого апельсина на поле
        if apple not in used:
            #если апельсин не поглощен
            #if abs(head[0] - apple[0]) <= height//2 and abs(head[1] - apple[1]) <= width//2:
            rez += 1/(abs(head[0] - apple[0]) + abs(head[1] - apple[1]))
            #else:
            #    rez -= 1/50
            #чем апельсин дальше, тем хуже
        else:
            #иначе
            if apple in snake[my_snake][0]:
                #если ее поглотили мы
                rez += 10
                #то хорошо
            else:
                #иначе
                rez -= 2
                #плохо
    return rez*turn


def f8(situation, depth, turn, my_snake):
    #minimax
    b, snake = situation
    b = deepcopy(b)
    snake = deepcopy(snake)
    if depth == 0 or b == []:
        #print(snake, b, f7(situation, turn), file = sys.stderr)
        return [turn * f7(situation, turn), None]
    rez = f3(my_snake, b, snake)
    if rez == []:
        return [-10**8, None]
    value = -10**9
    best_turn = 'B'
    for ch in rez:
        val = f8(f6(ch, (my_snake + (1-turn)//2)%2, b, snake), depth-1, -turn, my_snake)[0]
        if val > value:
            value = val
            best_turn = ch
    return [value, best_turn]
    

def f9(my_snake):
    #получает ситуацию, возвращает возможный ход исходя их minimax
    bs, snakes = deepcopy(b), deepcopy(snake)
    ex = f8([bs, snakes], 7, 1, my_snake)[1]
    #print(f8([bs, snakes], 5, 1, my_snake), file = sys.stderr)
    #if ex == None:
    #    ex = f4(my_snake)
    return ex


while True:
    #Всегда, до того момента, пока тестирующая система не остановит программу
    if turn == -1:
        #если ход противника
        ex = input()
        #считаем его
        f5(ex, (my_snake+1)%2)
        #изменим состояние
        turn *= -1
        #изменим ход
    else:
        #наш ход
        ex = f9(my_snake)
        #вычислим возможный ход
        print(ex)
        #выведем его
        f5(ex, my_snake)
        #изменим состояние
        turn *= -1
        #изменим ход
