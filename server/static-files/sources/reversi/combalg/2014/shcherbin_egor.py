#!/usr/bin/env python
from __future__ import print_function
from copy import deepcopy
import itertools
import random
import sys

DEBUG = False

SKIP_STR = 'Skip'
BLACK_STONE_CHAR = '@'
WHITE_STONE_CHAR = 'O'
EMPTY_CHAR = '.'
BOARD_SIZE = 8

MINIMAX_DEPTH = 4
ALPHABETA_DEPTH = 8
INF = 2117117117


class ReversiGameState():
    def __init__(self):
        self.board = [[EMPTY_CHAR] * BOARD_SIZE for i in xrange(BOARD_SIZE)]
        self.board[3][3] = WHITE_STONE_CHAR
        self.board[4][4] = WHITE_STONE_CHAR
        self.board[3][4] = BLACK_STONE_CHAR
        self.board[4][3] = BLACK_STONE_CHAR
        self.turn = 0
        self.possible_moves = dict()

    def print_game_state(self, history_mode=False):
        print("Black's" if self.turn == 0 else "White's", 'turn')
        print(' ', end='')
        for i in xrange(BOARD_SIZE):
            print(chr(ord('1') + i), end='')
        print()
        for i in xrange(BOARD_SIZE):
            if history_mode:
                print('HISTORY: ', end='')
            print(chr(ord('a') + i), end='')
            for j in xrange(BOARD_SIZE):
                print(self.board[i][j], end='')
            print()
        print()

    def get_current_player_char(self):
        return BLACK_STONE_CHAR if self.turn == 0 \
            else WHITE_STONE_CHAR

    def get_opponent_char(self):
        return BLACK_STONE_CHAR if self.turn == 1 \
            else WHITE_STONE_CHAR

    def update_game_state(self):
        self.possible_moves = dict()
        current_player_char = self.get_current_player_char()
        opponent_char = self.get_opponent_char()
        deltas = [(0, -1), (0, 1), (-1, 0), (1, 0),
                  (-1, -1), (1, 1), (-1, 1), (1, -1)]
        for x, y in itertools.product(xrange(BOARD_SIZE), repeat=2):
            if self.board[x][y] != EMPTY_CHAR:
                continue
            self.possible_moves[x, y] = []
            for delta_x, delta_y in deltas:
                i = 1
                while 0 <= x + i * delta_x < BOARD_SIZE and \
                        0 <= y + i * delta_y < BOARD_SIZE and \
                        self.board[x + i * delta_x][y + i * delta_y] == \
                        opponent_char:
                    i += 1
                if i > 1 and \
                        0 <= x + i * delta_x < BOARD_SIZE and \
                        0 <= y + i * delta_y < BOARD_SIZE and \
                        self.board[x + i * delta_x][y + i * delta_y] == \
                        current_player_char:
                    self.possible_moves[x, y] \
                        .append((x + i * delta_x, y + i * delta_y))
            if not self.possible_moves[x, y]:
                del self.possible_moves[x, y]


class ReversiGame:
    def __init__(self):
        self.current_state = ReversiGameState()
        self.current_state.update_game_state()
        self.research_mode_on = False
        self.previous_move_skipped = False
        self.state_history = []

    @property
    def is_finished(self):
        return not self.current_state.possible_moves and \
            self.previous_move_skipped

    def toggle_research_mode(self):
        self.research_mode_on = not self.research_mode_on
        if self.research_mode_on:
            self.state_history = [deepcopy(self.current_state)]
        else:
            self.state_history = []

    def make_move(self, move_):
        if move_ == 'Skip':
            self.previous_move_skipped = True
            self.current_state.turn ^= 1
            self.current_state.update_game_state()
            if self.research_mode_on:
                self.state_history.append(deepcopy(self.current_state))
            return
        self.previous_move_skipped = False
        move_x, move_y = move_
        assert (move_x, move_y) in self.current_state.possible_moves.keys()
        current_player_char = self.current_state.get_current_player_char()
        self.current_state.board[move_x][move_y] = current_player_char
        for x, y in self.current_state.possible_moves[move_x, move_y]:
            if x == move_x:
                for i in xrange(min(y, move_y) + 1, max(y, move_y)):
                    self.current_state.board[x][i] = current_player_char
            elif y == move_y:
                for i in xrange(min(x, move_x) + 1, max(x, move_x)):
                    self.current_state.board[i][y] = current_player_char
            elif x + y == move_x + move_y:
                for i in xrange(1, abs(x - move_x)):
                    self.current_state \
                        .board[min(x, move_x) + i][max(y, move_y) - i] = \
                        current_player_char
            elif x - y == move_x - move_y:
                for i in xrange(1, abs(x - move_x)):
                    self.current_state \
                        .board[min(x, move_x) + i][min(y, move_y) + i] = \
                        current_player_char
        self.current_state.turn ^= 1
        self.current_state.update_game_state()
        if self.research_mode_on:
            self.state_history.append(deepcopy(self.current_state))

    def undo_move(self):
        assert len(self.state_history) > 1
        self.state_history.pop()
        self.current_state = deepcopy(self.state_history[-1])

    def get_score(self):
        black_score, white_score = 0, 0
        for x, y in itertools.product(xrange(BOARD_SIZE), repeat=2):
            if self.current_state.board[x][y] == BLACK_STONE_CHAR:
                black_score += 1
            elif self.current_state.board[x][y] == WHITE_STONE_CHAR:
                white_score += 1
        return black_score, white_score


class ReversiBot():
    def __init__(self):
        pass

    def make_move(self, game):
        pass

    def heuristic(self, game):
        black_score, white_score = game.get_score()
        return black_score - white_score


class RandomBot(ReversiBot):
    def make_move(self, game):
        if not game.current_state.possible_moves.keys():
            return SKIP_STR
        return random.choice(game.current_state.possible_moves.keys())


class MinimaxBot(ReversiBot):
    def make_move(self, game):
        if not game.current_state.possible_moves.keys():
            return SKIP_STR
        game.toggle_research_mode()
        move_ = self.minimax(game, MINIMAX_DEPTH, return_move=True)[1]
        game.toggle_research_mode()
        return move_

    def minimax(self, game, depth, return_move=False):
        if depth == 0 or not game.current_state.possible_moves:
            return self.heuristic(game), None
        result = -INF if game.current_state.turn == 0 else INF
        best_move = None
        for move_ in game.current_state.possible_moves.keys()[:]:
            game.make_move(move_)
            move_result = self.minimax(game, depth - 1)[0]
            game.undo_move()
            if (game.current_state.turn == 0 and move_result > result) or \
               (game.current_state.turn == 1 and move_result < result):
                result = move_result
                if return_move:
                    best_move = move_
        return result, best_move


def input_move():
    move_str = raw_input()
    while move_str != SKIP_STR and \
            (len(move_str) != 2 or not
             'a' <= move_str[0] <= chr(ord('a') + BOARD_SIZE - 1) or not
             '1' <= move_str[1] <= chr(ord('1') + BOARD_SIZE - 1)):
        print('Input valid move!')
        move_str = raw_input()
    if move_str == SKIP_STR:
        return move_str
    return ord(move_str[0]) - ord('a'), ord(move_str[1]) - ord('1')


def print_move(move_):
    if move_ == SKIP_STR:
        print(move_)
        return
    print(chr(ord('a') + move_[0]), chr(ord('1') + move_[1]), sep='')


if __name__ == '__main__':
    if DEBUG:
        game_type = raw_input('Input type of the game (pvp, pvb, bvp, bvb): ')
    else:
        start_turn = input()
        if start_turn == 3:
            game_type = 'bvb'
        elif start_turn == 2:
            game_type = 'pvb'
        else:
            game_type = 'bvp'
    assert game_type in ['pvp', 'pvb', 'bvp', 'bvb']

    reversi_game = ReversiGame()
    if DEBUG:
        reversi_game.current_state.print_game_state()
    reversi_bot = RandomBot()
    # reversi_bot = MinimaxBot()

    while not reversi_game.is_finished:
        if reversi_game.current_state.turn == 0:
            if game_type[0] == 'p':
                move = input_move()
            else:
                move = reversi_bot.make_move(reversi_game)
                print_move(move)
                sys.stdout.flush()
        else:
            if game_type[2] == 'p':
                move = input_move()
            else:
                move = reversi_bot.make_move(reversi_game)
                print_move(move)
                sys.stdout.flush()
        reversi_game.make_move(move)
        if DEBUG:
            reversi_game.current_state.print_game_state()

    if DEBUG:
        score = reversi_game.get_score()
        print(*score, sep=':')
