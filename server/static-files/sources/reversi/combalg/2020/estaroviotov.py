#!/usr/bin/env python3
from random import choice


class Player:
    def __init__(self, color, opp_color, strategy):
        self.color = color
        self.opp_color = opp_color
        board = [["_" for _ in range(8)] for _ in range(8)]
        board[3][3], board[4][4] = "*", "*"
        board[3][4], board[4][3] = "&", "&"
        self.board = board

        if strategy == "random":
            self.move = self.random_move

    @staticmethod
    def is_on_board(x, y):
        return 0 <= x <= 7 and 0 <= y <= 7

    def _available_moves(self):
        directions = ((-1, -1), (-1, 0), (0, -1), (-1, 1), (1, -1), (1, 0), (0, 1), (1, 1))
        legal_moves = []
        for y in range(8):
            for x in range(8):
                if self.board[y][x] == "_":
                    for y_dir, x_dir in directions:
                        cur_y, cur_x = y+y_dir, x+x_dir
                        while self.is_on_board(cur_x, cur_y) and self.board[cur_y][cur_x] == self.opp_color:
                            cur_y += y_dir
                            cur_x += x_dir
                            if self.is_on_board(cur_x, cur_y) and self.board[cur_y][cur_x] == self.color:
                                legal_moves.append((y, x))
        return legal_moves

    def _process_move(self, move_y, move_x, color1, color2):
        directions = ((-1, -1), (-1, 0), (0, -1), (-1, 1), (1, -1), (1, 0), (0, 1), (1, 1))

        self.board[move_y][move_x] = color2
        for y_dir, x_dir in directions:
            cur_y, cur_x = move_y + y_dir, move_x + x_dir
            positions = []
            while self.is_on_board(cur_x, cur_y) and self.board[cur_y][cur_x] != "_":
                if self.board[cur_y][cur_x] == color1:
                    positions.append((cur_y, cur_x))
                    cur_y += y_dir
                    cur_x += x_dir
                elif self.board[cur_y][cur_x] == color2:
                    for y, x in positions:
                        self.board[y][x] = color2
                    break

    def process_opp_move(self, opp_move):
        vertical = "abcdefgh"
        move_y = vertical.find(opp_move[0])
        move_x = int(opp_move[1]) - 1
        self._process_move(move_y, move_x, self.color, self.opp_color)

    def random_move(self):
        legal_moves = self._available_moves()
        if len(legal_moves) == 0:
            return "Skip"
        move = choice(legal_moves)
        self._process_move(move[0], move[1], self.opp_color, self.color)

        return move


def main():
    color = int(input())
    if color == 1:
        player = Player("*", "&", "random")
        our_move = player.move()
        print("abcdefgh"[our_move[0]] + str(our_move[1]+1), flush=True)
    else:
        player = Player("&", "*", "random")

    while True:
        opp_move = input()
        if opp_move != "Skip":
            player.process_opp_move(opp_move)
        our_move = player.move()
        if our_move == "Skip":
            if opp_move == "Skip":
                return
            print("Skip", flush=True)
        else:
            print("abcdefgh"[our_move[0]] + str(our_move[1]+1), flush=True)


if __name__ == "__main__":
    main()
