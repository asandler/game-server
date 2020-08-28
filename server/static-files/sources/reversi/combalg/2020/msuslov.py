#!/usr/bin/env python3
import random
import sys
import copy
from datetime import datetime

limit = 100

def is_possible(field, my_color, cord):
  result = []
  i = cord[0]
  j = cord[1]
  k = 1
  enemy_found = False
  possible = False
  while i - k >= 0 and field[i - k][j] != 0 and not possible: # up field[i - k][j]
    enemy_found = enemy_found or (field[i - k][j] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i - k][j] == my_color)
    k+=1
  if possible: result.append("up")
  k = 1
  enemy_found = False
  possible = False
  while i + k < 8 and field[i + k][j] != 0 and not possible: # down field[i + k][j]
    enemy_found = enemy_found or (field[i + k][j] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i + k][j] == my_color)
    k+=1
  if possible: result.append("down")
  k = 1
  enemy_found = False
  possible = False
  while j - k >= 0 and field[i][j - k] != 0 and not possible: # left field[i][j - k]
    enemy_found = enemy_found or (field[i][j - k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i][j - k] == my_color)
    k+=1
  if possible: result.append("left")
  k = 1
  enemy_found = False
  possible = False
  while j + k < 8 and field[i][j + k] != 0 and not possible: # right field[i][j + k]
    enemy_found = enemy_found or (field[i][j + k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i][j + k] == my_color)
    k+=1
  if possible: result.append("right")
  k = 1
  enemy_found = False
  possible = False
  while i - k >= 0 and j - k >= 0 and field[i - k][j - k] != 0 and not possible: # ul field[i - k][j - k]
    enemy_found = enemy_found or (field[i - k][j - k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i - k][j - k] == my_color)
    k+=1
  if possible: result.append("ul")
  k = 1
  enemy_found = False
  possible = False
  while i + k < 8 and j - k >= 0 and field[i + k][j - k] != 0 and not possible: # dl field[i + k][j - k]
    enemy_found = enemy_found or (field[i + k][j - k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i + k][j - k] == my_color)
    k+=1
  if possible: result.append("dl")
  k = 1
  enemy_found = False
  possible = False
  while i - k >= 0 and j + k < 8 and field[i - k][j + k] != 0 and not possible: # ur field[i - k][j + k]
    enemy_found = enemy_found or (field[i - k][j + k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i - k][j + k] == my_color)
    k+=1
  if possible: result.append("ur")
  k = 1
  enemy_found = False
  possible = False
  while i + k < 8 and j + k < 8 and field[i + k][j + k] != 0 and not possible: # dr field[i + k][j + k]
    enemy_found = enemy_found or (field[i + k][j + k] == -my_color)
    if not enemy_found: break
    possible = possible or (field[i + k][j + k] == my_color)
    k+=1
  if possible: result.append("dr")
  return result

def find_possible_turns(field, my_color):
  result = [];
  for i, row in enumerate(field):
    for j, elem in enumerate(row):
      if elem != 0: continue
      if len(is_possible(field, my_color, [i, j])) != 0:
        result.append((i, j))
  return result

def get_turn(field, color, turn):
  direction = is_possible(field, color, turn)
  i = turn[0]
  j = turn[1]
  field[i][j] = color
  for dir in direction:
    if dir == "up":
      k = 1
      while i - k >= 0 and field[i - k][j] == -color:
        field[i - k][j] = color
        k += 1
    elif dir == "down":
      k = 1
      while i + k < 8 and field[i + k][j] == -color:
        field[i + k][j] = color
        k += 1
    elif dir == "left":
      k = 1
      while j - k >= 0 and field[i][j - k] == -color:
        field[i][j - k] = color
        k += 1
    elif dir == "right":
      k = 1
      while j + k < 8 and field[i][j + k] == -color:
        field[i][j + k] = color
        k += 1
    elif dir == "ur":
      k = 1
      while i - k >= 0 and j + k < 8 and field[i - k][j + k] == -color:
        field[i - k][j + k] = color
        k += 1
    elif dir == "ul":
      k = 1
      while i - k >= 0 and j - k >= 0 and field[i - k][j - k] == -color:
        field[i - k][j - k] = color
        k += 1
    elif dir == "dr":
      k = 1
      while i + k < 8 and j + k < 8 and field[i + k][j + k] == -color:
        field[i + k][j + k] = color
        k += 1
    elif dir == "dl":
      k = 1
      while i + k < 8 and j - k >= 0 and field[i + k][j - k] == -color:
        field[i + k][j - k] = color
        k += 1
  return field

def score(field, my_color):
  domination = 0
  for i, row in enumerate(field):
    for j, elem in enumerate(row):
      domination += elem * my_color * abs(4 - i) * abs(4 - j)
  return domination

def minimax(field, depth, color, upperbound):
  possible_turns = find_possible_turns(field, color)
  if depth == 0 or len(possible_turns) == 0:
    return score(field, color)
  value = -1000000
  for turn in possible_turns:
    if value >= upperbound: break
    value = max(value, -minimax(get_turn(copy.deepcopy(field), color, turn), depth-1, -color, -value))
  return value

def make_turn(field, my_color, minimax_depth):
  possible_turns = find_possible_turns(field, my_color)
  if len(possible_turns) == 0: return [-1, -1]
  max_value = -1000000
  result_turn = None
  for i, turn in enumerate(possible_turns):
    value = minimax(get_turn(copy.deepcopy(field), my_color, turn), minimax_depth, my_color, -max_value)
    if max_value < value:
      max_value = value
      result_turn = turn
  return result_turn

def play():
  white_color = 1
  black_color = -1
  field = [[0 for j in range(8)] for i in range(8)]
  field[3][3] = white_color
  field[3][4] = black_color
  field[4][4] = white_color
  field[4][3] = black_color
  head = int(input())
  my_color = white_color if head == 2 else black_color
  if (my_color == white_color):
    turn = input()
    turn = (ord(turn[0]) - ord('a'), ord(turn[1]) - ord('1'))
    get_turn(field, -my_color, turn)
  while True:
    start_time = datetime.now()
    my_turn = make_turn(copy.deepcopy(field), my_color, 4)
    print(datetime.now() - start_time, file=sys.stderr)
    if my_turn[0] == -1:
      print("Skip")
    else:
      print(field, sep=" ", file=sys.stderr)
      get_turn(field, my_color, my_turn)
      print(chr(my_turn[0]+ord('a')), my_turn[1] + 1, sep='')
    turn = input()
    if turn != "Skip":
      turn = (ord(turn[0]) - ord('a'), ord(turn[1]) - ord('1'))
      get_turn(field, -my_color, turn)
play()