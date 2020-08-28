#!/usr/bin/env python3
def vv(a,x,y,u):
	k=1
	while(a[x+k][y] == -u):
		k += 1
	if(a[x+k][y] == u):
		k = 1
		while(a[x+k][y] == -u):
			a[x+k][y] = u
			k += 1
	return(a)

def vn(a,x,y,u):
	k = 1
	while(a[x-k][y] == -u):
		k += 1
	if(a[x-k][y] == u):
		k = 1
		while(a[x-k][y] == -u):
			a[x-k][y] = u
			k += 1
	return(a)

def vp(a,x,y,u):
	k = 1
	while(a[x][y+k] == -u):
		k += 1
	if(a[x][y+k] == u):
		k =1
		while(a[x][y+k] == -u):
			a[x][y+k] = u
			k += 1
	return(a)
	
def vl(a,x,y,u):
	k = 1
	while(a[x][y-k] == -u):
		k += 1
	if(a[x][y-k] == u):
		k = 1
		while(a[x][y-k] == -u):
			a[x][y-k] = u
			k += 1
	return(a)

def vvvl(a,x,y,u):
	k = 1
	while(a[x-k][y-k]  == -u):
		k+=1
	if(a[x-k][y-k] == u):
		k = 1
		while(a[x-k][y-k] == -u):
			a[x-k][y-k] = u
			k += 1
	return(a)

def vnvp(a,x,y,u):
	k = 1
	while(a[x+k][y+k] == -u):
		k += 1
	if(a[x+k][y+k] == u):
		k = 1
		while(a[x+k][y+k] == -u):
			a[x+k][y+k] = u
			k += 1
	return(a)
	
def vvvp(a,x,y,u):
	k = 1
	while(a[x-k][y+k] == -u):
		k += 1
	if(a[x-k][y+k] == u):
		k = 1
		while(a[x-k][y+k] == -u):
			a[x-k][y+k] = u
			k += 1
	return(a)
	
def vnvl(a,x,y,u):
	k = 1
	while(a[x+k][y-k] == -u):
		k += 1
	if(a[x+k][y-k] == u):
		k = 1
		while(a[x+k][y-k] == -u):
			a[x+k][y-k] = u
			k += 1
	return(a)
	
def gor(a,u):
	m = 0
	mi = 0
	mj = 0
	for i in range(1,9):
		x = 0
		y = 0
		c = 0
		for j in range(1,9):
			if(x != 0)and(y != 0)and(c == 1)and(a[i][j]==0):
				if(max(abs(i-x),abs(j-y)) > m):
					m = max(abs(i-x),abs(j-y))
					mi = i
					mj = j
			else:
				if(x == 0)and(a[i][j] == u):
					x=i
					y=j
				else:
					if(x != 0)and(c == 0)and(a[i][j] == -u):
						c = 1
					else:
						if(x != 0)and(a[i][j] == u):
							x = 0
							y = 0
							c = 0
	for i in range(1,9):
		x = 0
		y = 0
		c = 0
		for j in range(8,0,-1):
			if(x != 0)and(y != 0)and(c == 1)and(a[i][j]==0):
				if(max(abs(i-x),abs(j-y)) > m):
					m = max(abs(i-x),abs(j-y))
					mi = i
					mj = j
			else:
				if(x == 0)and(a[i][j] == u):
					x=i
					y=j
				else:
					if(x != 0)and(c == 0)and(a[i][j] == -u):
						c = 1
					else:
						if(x != 0)and(a[i][j] == u):
							x = 0
							y = 0
							c = 0
	return(mi,mj)
	
def ver(a,u):
	for i in range(1,9):
		x = 0
		y = 0
		c = 0
		for j in range(1,9):
			if(x != 0)and(y != 0)and(c == 1)and(a[j][i]==0):
				if(max(abs(j-x),abs(i-y)) > m):
					m = max(abs(j-x),abs(i-y))
					mi = j
					mj = i
			else:
				if(x == 0)and(a[j][i] == u):
					x=j
					y=i
				else:
					if(x != 0)and(c == 0)and(a[j][i] == -u):
						c = 1
					else:
						if(a[j][i] == u)and(x != 0):
							x = 0
							y = 0
							c = 0
	for i in range(8,0,-1):
		x = 0
		y = 0
		c = 0
		for j in range(8,0,-1):
			if(x != 0)and(y != 0)and(c == 1)and(a[j][i]==0):
				if(max(abs(j-x),abs(i-y)) > m):
					m = max(abs(j-x),abs(i-y))
					mi = j
					mj = i
			else:
				if(x == 0)and(a[j][i] == u):
					x=j
					y=i
				else:
					if(x != 0)and(c == 0)and(a[j][i] == -u):
						c = 1
					else:
						if(a[j][i] == u)and(x != 0):
							x = 0
							y = 0
							c = 0							
	return(mi,mj)
	
def diag1(a,u):
	m = 0
	mi = 0
	mj = 0
	for i in range(1,9):
		for j in range(1,9):
			if(i == 1)or(j == 1):
				k = 0
				x = 0
				y = 0
				c = 0
				while((i+k<9)and(j+k<9)):
					if(x != 0)and(y != 0)and(c == 1)and(a[i+k][j+k]==0):
						if(k > m):
							m = k
							mi = i+k
							mj = j+k
					else:
						if(x == 0)and(a[i+k][j+k] == u):
							x=j
							y=i
						else:
							if(x != 0)and(c == 0)and(a[i+k][j+k] == -u):
								c = 1
							else:
								if(a[i+k][j+k] == u)and(x != 0):
									x = 0
									y = 0
									c = 0
					k += 1
				k = 0
				while((i+k>0)and(j+k>0)):
					if(x != 0)and(y != 0)and(c == 1)and(a[i+k][j+k]==0):
						if(k > m):
							m = k
							mi = i+k
							mj = j+k
					else:
						if(x == 0)and(a[i+k][j+k] == u):
							x=j
							y=i
						else:
							if(x != 0)and(c == 0)and(a[i+k][j+k] == -u):
								c = 1
							else:
								if(a[i+k][j+k] == u)and(x != 0):
									x = 0
									y = 0
									c = 0
					k -= 1
	return(mi,mj)
	
def diag2(a,u):
	m = 0
	mi = 0
	mj = 0
	for i in range(1,9):
		for j in range(1,9):
			x = 0
			y = 0
			c = 0
			k = 0
			while((i+k<9)and(j- k>0)):
				if(x != 0)and(y != 0)and(c == 1)and(a[i+k][j-k]==0):
					if(k > m):
						m = k
						mi = i+k
						mj = j-k
				else:
					if(x == 0)and(a[i+k][j-k] == u):
						x=j
						y=i
					else:
						if(x != 0)and(c == 0)and(a[i+k][j-k] == -u):
							c = 1
						else:
							if(a[i+k][j-k] == u)and(x != 0):
								x = 0
								y = 0
								c = 0
				k += 1
	for i in range(1,9):
		for j in range(1,9):
			x = 0
			y = 0
			c = 0
			k = 0
			while((i-k>0)and(j+k<9)):
				if(x != 0)and(y != 0)and(c == 1)and(a[i-k][j+k]==0):
					if(k > m):
						m = k
						mi = i-k
						mj = j+k
				else:
					if(x == 0)and(a[i-k][j+k] == u):
						x=j
						y=i
					else:
						if(x != 0)and(c == 0)and(a[i-k][j+k] == -u):
							c = 1
						else:
							if(a[i-k][j+k] == u)and(x != 0):
								x = 0
								y = 0
								c = 0
				k += 1
	return(mi,mj)
			
	
	
n = int(input())
a = [[0 for i in range(10)] for j in range(10)]
d = {'a':1, 'b':2,'c':3,'d':4,'e':5,'f':6,'g':7,'h':8}
od = {1:'a',2:'b',3:'c',4:'d',5:'e',6:'f',7:'g',8:'h'}
a[4][4]=-1
a[5][5]=-1
a[4][5]=1
a[5][4]=1
if(n == 2):
	s = input()
	x=d[s[0]]
	y=int(s[1])
	a[x][y]=1
	t = 1
	a = vv(a,x,y,t)
	a = vn(a,x,y,t)
	a = vp(a,x,y,t)
	a = vl(a,x,y,t)
	a = vvvp(a,x,y,t)
	a = vvvl(a,x,y,t)
	a = vnvp(a,x,y,t)
	a = vnvl(a,x,y,t)
else:
	t = -1
while True:
	t *= -1
	aa,b = diag1(a,t)
	if(aa != 0):
		x = aa
		y = b
		a[x][y] = t
		a = vv(a,x,y,t)
		a = vn(a,x,y,t)
		a = vp(a,x,y,t)
		a = vl(a,x,y,t)
		a = vvvp(a,x,y,t)
		a = vvvl(a,x,y,t)
		a = vnvp(a,x,y,t)
		a = vnvl(a,x,y,t)
		print(od[aa] + str(b))
	else:
		aa,b = diag2(a,t)
		if(aa != 0):
			x = aa
			y = b
			a[x][y] = t
			a = vv(a,x,y,t)
			a = vn(a,x,y,t)
			a = vp(a,x,y,t)
			a = vl(a,x,y,t)
			a = vvvp(a,x,y,t)
			a = vvvl(a,x,y,t)
			a = vnvp(a,x,y,t)
			a = vnvl(a,x,y,t)
			print(od[aa] + str(b))
		else:
			aa,b = gor(a,t)
			if(aa != 0):
				x = aa
				y = b
				a[x][y] = t
				a = vv(a,x,y,t)
				a = vn(a,x,y,t)
				a = vp(a,x,y,t)
				a = vl(a,x,y,t)
				a = vvvp(a,x,y,t)
				a = vvvl(a,x,y,t)
				a = vnvp(a,x,y,t)
				a = vnvl(a,x,y,t)
				print(od[aa] + str(b))
			else:
				aa,b = ver(a,t)
				print(aa,b)
				if(aa != 0):
					x = aa
					y = b
					a[x][y] = t
					a = vv(a,x,y,t)
					a = vn(a,x,y,t)
					a = vp(a,x,y,t)
					a = vl(a,x,y,t)
					a = vvvp(a,x,y,t)
					a = vvvl(a,x,y,t)
					a = vnvp(a,x,y,t)
					a = vnvl(a,x,y,t)
					print(od[aa] + str(b))
				else:
					print('Skip')
	s = input()
	x=d[s[0]]
	y=int(s[1])
	t *= -1
	a[x][y]=t
	a = vv(a,x,y,t)
	a = vn(a,x,y,t)
	a = vp(a,x,y,t)
	a = vl(a,x,y,t)
	a = vvvp(a,x,y,t)
	a = vvvl(a,x,y,t)
	a = vnvp(a,x,y,t)
	a = vnvl(a,x,y,t)
			
		
	

	
