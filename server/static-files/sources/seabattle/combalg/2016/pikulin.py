#!/usr/bin/env python3
import random
from sys import stderr

alive = set()
of_which = dict()
alives = dict()

i = 0
for ship in input().split():
	begin, end = ship[:2], ship[2:]
	if begin > end: begin, end = end, begin
	alives[i] = 0
	if begin[0] == end[0]:  # vertical
		x = ord(begin[0])
		for y in range(int(begin[1]), int(end[1]) + 1):
			alive.add((x, y))
			of_which[(x, y)] = i
			alives[i] += 1
	else:  # horizontal ship
		y = int(begin[1])
		for x in range(ord(begin[0]), ord(end[0]) + 1):
			alive.add((x, y))
			of_which[(x, y)] = i
			alives[i] += 1
	i += 1

if int(input()) == 0:
	f = random.sample(alive, 1)[0]
	print('0X ' + chr(f[0]) + str(f[1]) + 'A0')

while True:
	inp = input()
	if len(inp) == 1:
		to = random.sample(alive, 1)[0]
		print('0X ' + chr(to[0]) + str(to[1]) + f)
		continue

	f, t = inp[:2], inp[2:]

	stderr.write('alive = ' + str(alive) + '\n')
	stderr.write('t = ' + str(t) + '\n')	

	print('0X', end=' ')
	pair = ord(t[0]), int(t[1])
	
	killed = True	
	if pair in alive:
		alive.remove(pair)
		alives[of_which[pair]] -= 1
		if alives[of_which[pair]]:
			print('H')
		else:
			print('K')	
	else:
		to = random.sample(alive, 1)[0]
		print(chr(to[0]) + str(to[1]) + f)
