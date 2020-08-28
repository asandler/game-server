#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>

typedef long long ll;
typedef unsigned long long ull;

#define cntones __builtin_popcountll
#define fs first
#define sc second

using namespace std;

bool debug = false;
const int INF = 1e9;

char my_colour, opp_colour;
ull p2[64];
vector <vector <char>> main_board(8, vector <char>(8, ' '));
vector <map <pair <ull, ull>, vector <pair <int, pair <int, int>>>>> possible_moves(65);
vector <map <pair <ull, ull>, int>> max_depth(65);
vector <map <pair <ull, ull>, int>> heurstic_value(65);
pair <ull, ull> mainh;

int skips = 0;

void print_board(vector <vector <char>> &board) {
  cout << ' ';
  for (int i = 0; i < 8; i++) cout << '-';
  cout << '\n';
  for (int i = 0; i < 8; i++) {
    cout << '|';
    for (int j = 0; j < 8; j++)
      cout << board[i][j];
    cout << '|';
    cout << '\n';
  }
  cout << ' ';
  for (int i = 0; i < 8; i++) cout << '-';
  cout << '\n';
}

void print_board() {
  print_board(main_board);
}

pair <ull, ull> bhash(vector <vector <char>> &board) {
  ull h0 = 0, h1 = 0;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == '1') h0 += p2[2 * (i * 8 + j) + 1];
      if (board[i][j] == '0') h0 += p2[2 * (i * 8 + j)];
      if (board[i + 4][j] == '1') h1 += p2[2 * (i * 8 + j) + 1];
      if (board[i + 4][j] == '0') h1 += p2[2 * (i * 8 + j)];
    }
  return {h0, h1};
}

void init() {
  p2[0] = 1;
  for (int i = 1; i < 64; i++)
    p2[i] = p2[i - 1] * 2;
  main_board[3][3] = main_board[4][4] = '1';
  main_board[3][4] = main_board[4][3] = '0';
  mainh = bhash(main_board);
}

int coefficient(int x, int y) {
  if (x > 3) x = 7 - x;
  if (y > 3) y = 7 - y;
  if (x == 0 && y == 0) return 50;
  if (x == 0 || y == 0) return 15;
  return 15 - (x + y);
}

int heuristic (vector <vector <char>> &board, char colour, int all) {
  if (all <= 15) {
    int sum = 0;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++) {
        if (board[i][j] == ' ') continue;
        if (board[i][j] == colour) sum += coefficient(i, j);
        if (board[i][j] != colour) sum -= coefficient(i, j);
      }
    return sum;
  }
  if (all >= 40) {
    int sum = 0;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++) {
        if (board[i][j] == ' ') continue;
        if (board[i][j] == colour) sum++;
        if (board[i][j] != colour) sum--;
      }
    return sum;
  }
  int sum = 0;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == ' ') continue;
      vector <vector <int>> tmp(3, vector <int>(3));
      for (int e1 = -1; e1 <= 1; e1++)
        for (int e2 = -1; e2 <= 1; e2++) {
          if (e1 == e2 && e2 == 0) continue;
          int ws = 0;
          for (int k = 1; i + k * e1 >= 0 && i + k * e1 < 8 && j + k * e2 >= 0 && j + k * e2 < 8; k++)
            ws += (board[i + k * e1][j + k * e2] == ' ');
          tmp[e1 + 1][e2 + 1] = ws;
        }
      int val = 20 - (tmp[0][0] * tmp[2][2] + tmp[1][0] * tmp[1][2] + tmp[0][1] * tmp[2][1] + tmp[0][2] * tmp[2][0]);
      if (board[i][j] == colour) sum += val;
      else sum -= val;
    }
  return sum;
}

void capture(int x, int y, int e1, int e2, vector <vector <char>> &board, vector <pair <int, int>> &captured) {
  int colour = board[x][y];
  bool tf = false;
  for (int k = 1; x + k * e1 >= 0 && x + k * e1 < 8 && y + k * e2 >= 0 && y + k * e2 < 8 && board[x + k * e1][y + k * e2] != ' '; k++) {
    if (board[x + k * e1][y + k * e2] == colour) {
      tf = true;
      break;
    }
  }
  if (!tf) return;
  for (int k = 1; x + k * e1 >= 0 && x + k * e1 < 8 && y + k * e2 >= 0 && y + k * e2 < 8 && board[x + k * e1][y + k * e2] != colour; k++)
    captured.push_back({x + k * e1, y + k * e2});
}

void do_move (int x, int y, vector <vector <char>> &board, pair <ull, ull> &h, char colour) {
  board[x][y] = colour;
  vector <pair <int, int>> captured;
  for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++) {
      if (!i && !j) continue;
      capture(x, y, i, j, board, captured);
    }
    for (auto &nownow : captured) {
      int _x = nownow.fs, _y = nownow.sc;
      board[_x][_y] = colour;
      if (_x < 4) h.fs += p2[2 * (_x * 8 + _y) + colour - '0'] - p2[2 * (_x * 8 + _y) + '1' - colour];
      else h.sc += p2[2 * ((_x - 4) * 8 + _y) + colour - '0'] - p2[2 * ((_x - 4) * 8 + _y) + '1' - colour];
    }
}

int analysis(vector <vector <char>> &board, pair <ull, ull> h, char colour, int depth, int alpha, int beta) {
  if (depth == 0 || max_depth[cntones(h.fs) + cntones(h.sc)].find({h.fs, h.sc}) != max_depth[cntones(h.fs) + cntones(h.sc)].end() && max_depth[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}] == depth) {
    if (heurstic_value[cntones(h.fs) + cntones(h.sc)].find({h.fs, h.sc}) == heurstic_value[cntones(h.fs) + cntones(h.sc)].end())
      heurstic_value[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}] = heuristic(board, colour, cntones(h.fs) + cntones(h.sc)) - 4;
    return heurstic_value[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}];
  }
  max_depth[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}] = depth;
  int value = -INF;
  vector <pair <int, pair <int, int>>> pos_mov;
  if (possible_moves[cntones(h.fs) + cntones(h.sc)].find({h.fs, h.sc}) == possible_moves[cntones(h.fs) + cntones(h.sc)].end()) {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++) {
        if (board[x][y] != ' ') continue;
        board[x][y] = colour;
        vector <pair <int, int>> captured;
        for (int i = -1; i <= 1; i++)
          for (int j = -1; j <= 1; j++) {
            if (!i && !j) continue;
            capture(x, y, i, j, board, captured);
          }
        board[x][y] = ' ';
        if (captured.empty()) continue;
        pos_mov.push_back({-INF, {x, y}});
      }
  }
  else pos_mov = possible_moves[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}];
  for (auto &now : pos_mov) {
    int x = now.sc.fs, y = now.sc.sc;
    board[x][y] = colour;
    vector <pair <int, int>> captured;
    for (int i = -1; i <= 1; i++)
      for (int j = -1; j <= 1; j++) {
        if (!i && !j) continue;
        capture(x, y, i, j, board, captured);
      }
    pair <ull, ull> newh = h;
    if (x < 4) newh.fs += p2[2 * (x * 8 + y) + colour - '0'];
    else newh.sc += p2[2 * ((x - 4) * 8 + y) + colour - '0'];
    for (auto &nownow : captured) {
      int _x = nownow.fs, _y = nownow.sc;
      board[_x][_y] = colour;
      if (_x < 4) newh.fs += p2[2 * (_x * 8 + _y) + colour - '0'] - p2[2 * (_x * 8 + _y) + '1' - colour];
      else newh.sc += p2[2 * ((_x - 4) * 8 + _y) + colour - '0'] - p2[2 * ((_x - 4) * 8 + _y) + '1' - colour];
    }
    value = max(value, -analysis(board, newh, '1' - colour + '0', depth - 1, -beta, -alpha));
    for (auto &nownow : captured) {
      int _x = nownow.fs, _y = nownow.sc;
      board[_x][_y] = '1' - colour + '0';
    }
    board[x][y] = ' ';
    now.fs = value;
    alpha = max(alpha, value);
    if (alpha >= beta) break;
  }
  if (depth >= 4) {
    sort(pos_mov.rbegin(), pos_mov.rend());
    possible_moves[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}] = pos_mov;
  }
  if (value == -INF) {
    if (heurstic_value[cntones(h.fs) + cntones(h.sc)].find({h.fs, h.sc}) == heurstic_value[cntones(h.fs) + cntones(h.sc)].end())
      heurstic_value[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}] = heuristic(board, colour, cntones(h.fs) + cntones(h.sc)) - 4;
    return heurstic_value[cntones(h.fs) + cntones(h.sc)][{h.fs, h.sc}];
  }
  return value;
}

void recolor(vector <vector <char>> &board) {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == ' ') continue;
      board[i][j] = '1' - board[i][j] + '0';
    }
}

int main () {
  init();
  int first_or_second;
  cin >> first_or_second;
  if (first_or_second == 3) debug = true;
  if (first_or_second == 2) {
    my_colour = '1', opp_colour = '0';
    string s;
    cin >> s;
    char c1 = s[0], c2 = s[1];
    int x = c1 - 'a', y = c2 - '1';
    char colour = opp_colour;
    main_board[x][y] = colour;
    vector <pair <int, int>> captured;
    for (int i = -1; i <= 1; i++)
      for (int j = -1; j <= 1; j++) {
        if (!i && !j) continue;
        capture(x, y, i, j, main_board, captured);
      }
      if (x < 4) mainh.fs += p2[2 * (x * 8 + y) + colour - '0'];
      else mainh.sc += p2[2 * ((x - 4) * 8 + y) + colour - '0'];
      for (auto &nownow : captured) {
        int _x = nownow.fs, _y = nownow.sc;
        main_board[_x][_y] = colour;
        if (_x < 4) mainh.fs += p2[2 * (_x * 8 + _y) + colour - '0'] - p2[2 * (_x * 8 + _y) + '1' - colour];
        else mainh.sc += p2[2 * ((_x - 4) * 8 + _y) + colour - '0'] - p2[2 * ((_x - 4) * 8 + _y) + '1' - colour];
      }
  }
  else my_colour = '0', opp_colour = '1';
  while (true) {
    int a = clock();
    for (int i = 0; i < cntones(mainh.fs) + cntones(mainh.sc); i++) {
      possible_moves[i] = map <pair <ull, ull>, vector <pair <int, pair <int, int>>>>();
      max_depth[i] = map <pair <ull, ull>, int>();
      heurstic_value[i] = map <pair <ull, ull>, int>();
    }
    int d = 7;
    if (skips > 0) {
      skips = 0;
      d = 5;
    }
    analysis(main_board, mainh, my_colour, d, -INF, INF);
    if (possible_moves[cntones(mainh.fs) + cntones(mainh.sc)][{mainh.fs, mainh.sc}].empty()) {
      cout << "Skip" << endl;
      skips++;
      recolor(main_board);
      mainh = bhash(main_board);
      swap(my_colour, opp_colour);
    }
    else {
      char colour = my_colour;
      if (debug) for (auto &now : possible_moves[cntones(mainh.fs) + cntones(mainh.sc)][{mainh.fs, mainh.sc}])
        cout << now.fs << ' ' << now.sc.fs << ' ' << now.sc.  sc << endl;
      auto now = possible_moves[cntones(mainh.fs) + cntones(mainh.sc)][{mainh.fs, mainh.sc}][0];
      int x = now.sc.fs, y = now.sc.sc;
      main_board[x][y] = colour;
      vector <pair <int, int>> captured;
      for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
          if (!i && !j) continue;
          capture(x, y, i, j, main_board, captured);
        }
        for (auto &nownow : captured) {
          int _x = nownow.fs, _y = nownow.sc;
          main_board[_x][_y] = colour;
          if (_x < 4) mainh.fs += p2[2 * (_x * 8 + _y) + colour - '0'] - p2[2 * (_x * 8 + _y) + '1' - colour];
          else mainh.sc += p2[2 * ((_x - 4) * 8 + _y) + colour - '0'] - p2[2 * ((_x - 4) * 8 + _y) + '1' - colour];
        }
      if (x < 4) mainh.fs += p2[2 * (x * 8 + y) + colour - '0'];
      else mainh.sc += p2[2 * ((x - 4) * 8 + y) + colour - '0'];
      cout << (char)(x + 'a') << (y + 1) << endl;
      if (debug) cout << now.fs << ' ' << clock() - a << endl;
    }
    if (debug) for (auto &now : possible_moves[cntones(mainh.fs) + cntones(mainh.sc)][{mainh.fs, mainh.sc}])
      cout << now.fs << ' ' << now.sc.fs << ' ' << now.sc.sc << endl;
    string s;
    cin >> s;
    if (s == "Skip") {
      recolor(main_board);
      mainh = bhash(main_board);
      swap(my_colour, opp_colour);
      skips++;
      continue;
    }
    char c1 = s[0], c2 = s[1];
    int x = c1 - 'a', y = c2 - '1';
    char colour = opp_colour;
    main_board[x][y] = colour;
    vector <pair <int, int>> captured;
    for (int i = -1; i <= 1; i++)
      for (int j = -1; j <= 1; j++) {
        if (!i && !j) continue;
        capture(x, y, i, j, main_board, captured);
      }
      if (x < 4) mainh.fs += p2[2 * (x * 8 + y) + colour - '0'];
      else mainh.sc += p2[2 * ((x - 4) * 8 + y) + colour - '0'];
      for (auto &nownow : captured) {
        int _x = nownow.fs, _y = nownow.sc;
        main_board[_x][_y] = colour;
        if (_x < 4) mainh.fs += p2[2 * (_x * 8 + _y) + colour - '0'] - p2[2 * (_x * 8 + _y) + '1' - colour];
        else mainh.sc += p2[2 * ((_x - 4) * 8 + _y) + colour - '0'] - p2[2 * ((_x - 4) * 8 + _y) + '1' - colour];
      }
  }
  return 0;
}
