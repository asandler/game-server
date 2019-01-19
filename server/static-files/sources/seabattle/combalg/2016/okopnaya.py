#!/usr/bin/env python3

def define(strike): 
	for i in range(10):
		if field[i][0] == field[i][2]:
			j = min(int(field[i][1]), int(field[i][3]))
			while j <= max(int(field[i][3]), int(field[i][1])):
				if strike == field[i][0] + str(j):
					return(i)
				j += 1
		else:
			j = min(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])) 
			while j <= max(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])):
				if strike == LETTERS[j] + field[i][1]:
					return(i)
				j += 1

def if_dead(i):
	if field[i][0] == field[i][2]:
			j = min(int(field[i][1]), int(field[i][3]))
			while j <= max(int(field[i][3]), int(field[i][1])):
				if ships[j][LETTERS.find(field[i][0])] == 1:
					return False
				j += 1
	else:
			j = min(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])) 
			while j <= max(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])):
				if ships[int(field[i][1])][j] == 1:
					return False
				j += 1
	return True

	


def turn(t):
	my_ship = ''
	i = 0
	while not my_ship:
		j = 0
		while not my_ship and j <= 9:
			if ships[i][j]:
				my_ship = LETTERS[j] + str(i)
			j += 1
		i += 1
	if players_ship[0]:
		print('1X ' + my_ship + players_ship[0])
	else:	
		print('1X ' + my_ship + LETTERS[t//10] + str(t%10))

def answer():
	player = input()
	if player[-1] != 'K' and player[-1] != 'H':
		players_ship[0] = player[:2]
		if ships[int(player[-1])][LETTERS.find(player[-2])]:
			ship = define(player[-2:])
			ships[int(player[-1])][LETTERS.find(player[-2])] = 0
			if if_dead(ship):
				print('1X K')	
			else:
				print('1X H')
			answer()
	return


LETTERS = 'ABCDEFGHIJ'

ships = [[0] * 10 for i in range(10)]

field = list(input().split())

players_ship = ['']


for i in range(10):
	if field[i][0] == field[i][2]:
		j = min(int(field[i][1]), int(field[i][3]))
		while j <= max(int(field[i][3]), int(field[i][1])):
			ships[j][LETTERS.find(field[i][0])] = 1
			j += 1
	else:
		j = min(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])) 
		while j <= max(LETTERS.find(field[i][0]), LETTERS.find(field[i][2])) :
			ships[int(field[i][1])][j] = 1
			j += 1

pl = input()

if pl =='1':
	answer()
i = 0
while True:
	turn(i)
	answer()
	i += 1



