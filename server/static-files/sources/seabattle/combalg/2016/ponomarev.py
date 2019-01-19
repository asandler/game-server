#!/usr/bin/env python3

import random

SECOND = "1"

def shuffle(array):
    for i in range(len(array)):
        pos = random.randint(0, i)
        array[i], array[pos] = array[pos], array[i]


class Client:
    def __init__(self, desk):
        self.desk = desk
        self.attacks = set([])
        self.go = []
        
        self.points = [chr(i + ord('A')) + str(j) for i in range(10) for j in range(10)]
        shuffle(self.points)
    
    
    def make_shoot(self):
        for i in range(len(self.go)):
            if not self.go[i] in self.attacks:
                res = self.go[i]
                self.attacks.add(res)
                self.go = self.go[i + 1:]
                
                return res
            
        for i in range(len(self.points)):
            if not self.points[i] in self.attacks:
                res = self.points[i]
                self.attacks.add(res)
                self.points = self.points[i + 1:]
                
                return res
    
    
    def destroy(self, pos):
        for i in range(-1, 2):
            for j in range(-1, 2):
                self.attacks.add(chr(ord(pos[0]) + i) + chr(ord(pos[1]) + j))
        
    
    def send(self, case):
        if self.desk.lives == 0:
            print("K")
            return
        
        if case == "attack":
            where = self.make_shoot()
            print("0X", self.desk.get_random_ship() + where)
            self.recieve()
            
            if len(self.result) == 1:
                if self.result == 'K':
                    self.destroy(where)
                    
                self.send("attack")
            else:
                self.send("deffence")
        else:
            check = self.desk.shoot(self.result[2:])
            self.go = [self.result[:2]] + self.go
            
            if check != "None":
                print("0X", check)
                self.recieve()
                self.send("deffence")
            else:
                self.send("attack")
    
    
    def recieve(self):
        self.result = input()


class Desk:
    def __init__(self):
        self.array = []
        self.lives = 0
        
        
    def add(self, obj):
        self.array.append(obj)
        self.lives += 1
       
        
    def shoot(self, pos):
        for board in self.array:
            if board.pos == pos and board.status == "Alive":
                board.status = "Dead"
                board.parent.lives -= 1
                self.lives -= 1
                
                if board.parent.lives == 0:
                    return "K"
                else:
                    return "H"
                
        return "None"
    
    
    def get_random_ship(self):
        for board in self.array:
            if board.status == "Alive":
                return board.pos
            

class Board:
    def __init__(self, pos, parent):
        self.pos = pos
        self.parent = parent
        self.status = "Alive"
    

class Ship:
    def __init__(self, position, boards):
        self.lives = 0
        
        s1 = position[0:2]
        s2 = position[2:4]
        if s1 > s2:
            position = s2 + s1
        
        for i in range(ord(position[0]), ord(position[2]) + 1):
            for j in range(ord(position[1]), ord(position[3]) + 1):
                self.lives += 1
                
                new_board = Board(chr(i) + chr(j), self)
                boards.add(new_board)
      
boards = Desk()
ships = [Ship(x, boards) for x in input().split()]

game = Client(boards)
position = input()


if position == SECOND:
    game.recieve()
    game.send("deffence")
else:
    game.send("attack")