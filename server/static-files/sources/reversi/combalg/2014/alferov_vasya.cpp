#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct turn
{
    int a, b;
    turn(){}
    turn(std::string s);
    turn(int _a, int _b) : a(_a), b(_b) {}
    std::string get_s();
    bool operator==(const turn& other) const
    { return a == other.a && b == other.b; }
};

struct position
{
    enum val : char
    {
        NONE = 0,
        BLACK = 1,
        WHITE = -1
    };
    std::array<std::array<val, 8>, 8> arr;
    position();
    val next_turn;
    void do_turn(turn t);
};

std::ostream& operator<<(std::ostream& str, const position& pos);

turn::turn(string s)
{
    a = s[0] - 'a';
    b = s[1] - '1';
}

string turn::get_s()
{
    string res;
    res.push_back('a' + a);
    res.push_back('1' + b);
    return res;
}

position::position()
{
    next_turn = position::val::BLACK;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            arr[i][j] = position::val::NONE;
    arr[3][3] = position::val::WHITE;
    arr[4][4] = position::val::WHITE;
    arr[4][3] = position::val::BLACK;
    arr[3][4] = position::val::BLACK;
}

position::val rev(position::val v)
{
    assert(v != position::val::NONE);
    if (v == position::val::WHITE)
        return position::val::BLACK;
    else
        return position::val::WHITE;
}

void position::do_turn(turn t)
{
    if (t == turn(-1, -1))
    {
        next_turn = rev(next_turn);
        return;
    }
    //horisontal
    //::left
    int left = -1;
    bool f = false;
    for (int i = 0; i < t.b; i++)
    {
        if (arr[t.a][i] == next_turn)
        {
            f = true;
            left = max(left, i);
        }
        if (arr[t.a][i] == position::val::NONE)
            f = false;
    }
    for (int i = left + 1; i < t.b; i++)
    {
        if (f)
            arr[t.a][i] = rev(arr[t.a][i]);
    }
    //::right
    int right = 8;
    f = false;
    for (int i = 7; i > t.b; i--)
    {
        if (arr[t.a][i] == next_turn)
        {
            f = true;
            right = min(right, i);
        }
        if (arr[t.a][i] == position::val::NONE)
            f = false;
    }
    for (int i = right - 1; i > t.b; i--)
    {
        if (f)
            arr[t.a][i] = rev(arr[t.a][i]);
    }
    //vertical
    //::up
    int up = -1;
    f = false;
    for (int i = 0; i < t.a; i++)
    {
        if (arr[i][t.b] == next_turn)
        {
            f = true;
            up = max(up, i);
        }
        if (arr[i][t.b] == position::val::NONE)
            f = false;
    }
    for (int i = up + 1; i < t.a; i++)
    {
        if (f)
            arr[i][t.b] = rev(arr[i][t.b]);
    }
    //::down
    int down = 8;
    f = false;
    for (int i = 7; i > t.a; i--)
    {
        if (arr[i][t.b] == next_turn)
        {
            f = true;
            down = min(down, i);
        }
        if (arr[i][t.b] == position::val::NONE)
            f = false;
    }
    for (int i = down - 1; i > t.a; i--)
    {
        if (f)
            arr[i][t.b] = rev(arr[i][t.b]);
    }
    //left-up diagonal
    //lud::up
    ///it's much easier to organize it like that))
    int diff = t.a - t.b;
    bool br = false;
    f = false;
    int up_x = -1, up_y = -1;
    for (int i = 0; i < 8 && !br; i++)
        for (int j = 0; j < 8 && !br; j++)
        {
            if (i - j == diff)
            {
                if (i == t.a)
                    br = true;
                else
                {
                    if (arr[i][j] == next_turn)
                    {
                        f = true;
                        up_x = i;
                        up_y = j;
                    }
                    if (arr[i][j] == position::val::NONE)
                        f = false;
                }
            }
        }
    for (int i = up_x + 1; i < t.a; i++)
        for (int j = up_y + 1; j < t.b; j++)
        {
            if (i - j == diff && f)
            {
                arr[i][j] = rev(arr[i][j]);
            }
        }
    //lud::down
    br = false;
    f = false;
    int down_x = 8, down_y = 8;
    for (int i = 7; i >= 0 && !br; i--)
        for (int j = 7; j >= 0 && !br; j--)
        {
            if (i - j == diff)
            {
                if (i == t.a)
                    br = true;
                else
                {
                    if (arr[i][j] == next_turn)
                    {
                        f = true;
                        down_x = i;
                        down_y = j;
                    }
                    if (arr[i][j] == position::val::NONE)
                        f = false;
                }
            }
        }
    for (int i = down_x - 1; i > t.a; i--)
        for (int j = down_y - 1; j > t.b; j--)
        {
            if (i - j == diff && f)
            {
                arr[i][j] = rev(arr[i][j]);
            }
        }
    //right-up diagonal
    //rud::up
    int sum = t.a + t.b;
    br = false;
    f = false;
    up_x = -1; up_y = -1;
    for (int i = 0; i < 8 && !br; i++)
        for (int j = 7; j >= 0 && !br; j--)
        {
            if (i + j == sum)
            {
                if (i == t.a)
                    br = true;
                else
                {
                    if (arr[i][j] == next_turn)
                    {
                        f = true;
                        up_x = i;
                        up_y = j;
                    }
                    if (arr[i][j] == position::val::NONE)
                        f = false;
                }
            }
        }
    for (int i = up_x + 1; i < t.a; i++)
        for (int j = up_y - 1; j > t.b; j--)
        {
            if (i + j == sum && f)
            {
                arr[i][j] = rev(arr[i][j]);
            }
        }
    //rud::down
    br = false;
    f = false;
    down_x = 8; down_y = 8;
    for (int i = 7; i >= 0 && !br; i--)
        for (int j = 0; j < 8 && !br; j++)
        {
            if (i + j == sum)
            {
                if (i == t.a)
                    br = true;
                else
                {
                    if (arr[i][j] == next_turn)
                    {
                        f = true;
                        down_x = i;
                        down_y = j;
                    }
                    if (arr[i][j] == position::val::NONE)
                        f = false;
                }
            }
        }
    for (int i = down_x - 1; i > t.a; i--)
        for (int j = down_y + 1; j < t.b; j++)
        {
            if (i + j == sum && f)
            {
                arr[i][j] = rev(arr[i][j]);
            }
        }
    arr[t.a][t.b] = next_turn;
    next_turn = rev(next_turn);
}


ostream& operator<<(ostream& str, const position& pos)
{
    str << " ";
    for (int i = 1; i < 9; i++)
        str << i << " ";
    str << endl;
    for (int i = 0; i < 8; i++)
    {
        str << (char)('a' + i);
        for (int j = 0; j < 8; j++)
        {
            if (pos.arr[i][j] == position::val::NONE)
                str << '.' << ' ';
            else if (pos.arr[i][j] == position::val::WHITE)
                str << '*' << ' ';
            else
                str << '&' << ' ';
        }
        str << endl;
    }
    return str;
}

std::vector<turn> get_turns(const position& pos);

vector<turn> get_turns(const position& pos)
{
    array<array<bool, 8>, 8> res;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            res[i][j] = false;
    //horisontals
    bool f = false;
    turn lst(-1, -1);
    for (int i = 0; i < 8; i++)
    {
        f = false;
        for (int j = 0; j < 8; j++)
        {
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (f && j != lst.b + 1)
                    res[i][j] = true;
                f = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                lst = turn(i, j);
                f = true;
            }
        }
    }
    lst = turn(-1, -1);
    for (int i = 7; i >= 0; i--)
    {
        f = false;
        for (int j = 7; j >= 0; j--)
        {
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (f && j != lst.b - 1)
                    res[i][j] = true;
                f = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                lst = turn(i, j);
                f = true;
            }
        }
    }
    //verticals
    lst = turn(-1, -1);
    f = false;
    for (int i = 0; i < 8; i++)
    {
        f = false;
        for (int j = 0; j < 8; j++)
        {
            if (pos.arr[j][i] == position::val::NONE)
            {
                if (f && j != lst.a + 1)
                    res[j][i] = true;
                f = false;
            }
            if (pos.arr[j][i] == pos.next_turn)
            {
                lst = turn(j, i);
                f = true;
            }
        }
    }
    lst = turn(-1, -1);
    for (int i = 7; i >= 0; i--)
    {
        f = false;
        for (int j = 7; j >= 0; j--)
        {
            if (pos.arr[j][i] == position::val::NONE)
            {
                if (f && j != lst.a - 1)
                    res[j][i] = true;
                f = false;
            }
            if (pos.arr[j][i] == pos.next_turn)
            {
                lst = turn(j, i);
                f = true;
            }
        }
    }
    //diff-diagonal
    unordered_map<int, bool> diff_diag;
    unordered_map<int, turn> diff_diag_lst;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!diff_diag.count(i - j))
                diff_diag[i - j] = false;
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (diff_diag[i - j] && (!diff_diag_lst.count(i - j) || diff_diag_lst[i - j].a + 1 != i))
                    res[i][j] = true;
                diff_diag[i - j] = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                diff_diag_lst[i - j] = turn(i, j);
                diff_diag[i - j] = true;
            }
        }
    }
    diff_diag.clear();
    diff_diag_lst.clear();
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 7; j >= 0; j--)
        {
            if (!diff_diag.count(i - j))
                diff_diag[i - j] = false;
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (diff_diag[i - j] && (!diff_diag_lst.count(i - j) || diff_diag_lst[i - j].a - 1 != i))
                    res[i][j] = true;
                diff_diag[i - j] = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                diff_diag_lst[i - j] = turn(i, j);
                diff_diag[i - j] = true;
            }
        }
    }
    //sum-diagonal
    unordered_map<int, bool> sum_diag;
    unordered_map<int, turn> sum_diag_lst;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            if (!sum_diag.count(i + j))
                sum_diag[i + j] = false;
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (sum_diag[i + j] && (!sum_diag_lst.count(i + j) || sum_diag_lst[i + j].a + 1 != i))
                    res[i][j] = true;
                sum_diag[i + j] = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                sum_diag_lst[i + j] = turn(i, j);
                sum_diag[i + j] = true;
            }
        }
    }
    sum_diag.clear();
    sum_diag_lst.clear();
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!sum_diag.count(i + j))
                sum_diag[i + j] = false;
            if (pos.arr[i][j] == position::val::NONE)
            {
                if (sum_diag[i + j] && (!sum_diag_lst.count(i + j) || sum_diag_lst[i + j].a - 1 != i))
                    res[i][j] = true;
                sum_diag[i + j] = false;
            }
            if (pos.arr[i][j] == pos.next_turn)
            {
                sum_diag_lst[i + j] = turn(i, j);
                sum_diag[i + j] = true;
            }
        }
    }
    vector<turn> ans;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (res[i][j])
                ans.push_back(turn(i, j));
    return ans;
}

template<class bl_engine, class wh_engine>
class reversi_game
{
  public:
    position cur_pos;
    bl_engine black_player;
    wh_engine white_player;
    reversi_game() {}
    turn next_turn();
    position::val who_is_a_winner();
};

template<class bl_engine, class wh_engine>
turn reversi_game<bl_engine, wh_engine>::next_turn()
{
    if (cur_pos.next_turn == position::val::WHITE)
    {
        turn t = white_player.eng_turn();
        cur_pos.do_turn(t);
        black_player.opponent_turn(t);
        return t;
    }
    else
    {
        turn t = black_player.eng_turn();
        cur_pos.do_turn(t);
        white_player.opponent_turn(t);
        return t;
    }
}

template<class bl_engine, class wh_engine>
position::val reversi_game<bl_engine, wh_engine>::who_is_a_winner()
{
    int cnt_white = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (cur_pos.arr[i][j] == position::val::WHITE)
                cnt_white++;
    int cnt_black = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (cur_pos.arr[i][j] == position::val::BLACK)
                cnt_black++;
    int cnt_none = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (cur_pos.arr[i][j] == position::val::NONE)
                cnt_none++;
    if (!cnt_white)
        return position::val::BLACK;
    if (!cnt_black)
        return position::val::WHITE;
    if (!cnt_none)
        return (cnt_black > cnt_white ? position::val::BLACK : position::val::WHITE);
    return position::val::NONE;
}

class human_engine
{
  public:
    turn eng_turn();
    void opponent_turn(const turn& t) {}
    human_engine() {}
};

turn human_engine::eng_turn()
{
    string s;
    cin >> s;
    if (s == "Skip")
        return turn(-1, -1);
    else
        return turn(s);
}


bool ch_nxt_player(position& pos);

bool ch_nxt_player(position& pos)
{
    assert(pos.next_turn != position::val::NONE);
    vector<turn> nt = get_turns(pos);
    if (nt.size())
        return false;
    if (pos.next_turn == position::val::WHITE)
        pos.next_turn = position::val::BLACK;
    else
        pos.next_turn = position::val::WHITE;
    return true;
}


class clever_nyan
{
  public:
    int operator()(const position& pos);
};

int count_full_nyan_nc(int i, int j)
{
    i = min(i, 7 - i);
    j = min(j, 7 - j);
    if (i == 3 || j == 3)
        return 1;
    if (i == 0 && j == 0)
        return 8;
    if (i + j == 3)
        return 2;
    if (i + j == 1)
        return -4;
    return -1;
}

int count_full_nyan_c(int i, int j)
{
    i = min(i, 7 - i);
    j = min(j, 7 - j);
    return 8 - (i + j);
}

int clever_nyan::operator()(const position& pos)
{
    int r = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (i < 4 && j < 4 && pos.arr[0][0] == position::val::NONE
             || i > 3 && j < 4 && pos.arr[7][0] == position::val::NONE
             || i > 3 && j > 3 && pos.arr[7][7] == position::val::NONE
             || i < 4 && j > 3 && pos.arr[0][7] == position::val::NONE)
                r += count_full_nyan_nc(i, j) * pos.arr[i][j];
            else
                r += count_full_nyan_c(i, j) * pos.arr[i][j];
    return r;
}

//black player is max-player

template<class nyan, int max_k>
class minimax_engine
{
    position cur_pos;
    nyan count_nyan_of_pos;
    int dfs(const position& pos, int k, bool cur_black);
  public:
    turn eng_turn();
    void opponent_turn(const turn& t);
    minimax_engine() {}
};

template<class nyan, int max_k>
int minimax_engine<nyan, max_k>::dfs(const position& pos, int k, bool cur_black)
{
    if (!k)
        return count_nyan_of_pos(pos);
    position pos_next = pos;
    std::vector<turn> nxt = get_turns(pos);
    if (!nxt.size())
    {
        ch_nxt_player(pos_next);
        return dfs(pos_next, k - 1, !cur_black);
    }
    int cur_res = 1e9;
    if (cur_black)
        cur_res = -cur_res;
    for (turn t: nxt)
    {
        pos_next = pos;
        pos_next.do_turn(t);
        int k_next = dfs(pos_next, k - 1, !cur_black);
        if (cur_black)
            cur_res = std::max(cur_res, k_next);
        else
            cur_res = std::min(cur_res, k_next);
    }
    return cur_res;
}

template<class nyan, int max_k>
turn minimax_engine<nyan, max_k>::eng_turn()
{
    std::vector<turn> nxt = get_turns(cur_pos);
    if (!nxt.size())
    {
        cur_pos.do_turn(turn(-1, -1));
        return turn(-1, -1);
    }
    turn mx = nxt[0];
    position pos_next = cur_pos;
    pos_next.do_turn(mx);
    int res = count_nyan_of_pos(pos_next);
    for (int i = 1; i < (int)nxt.size(); i++)
    {
        pos_next = cur_pos;
        pos_next.do_turn(nxt[i]);
        int k = dfs(pos_next, max_k - 1, cur_pos.next_turn == position::val::WHITE);
        if (cur_pos.next_turn == position::val::BLACK && k > res)
        {
            mx = nxt[i];
            res = k;
        }
        if (cur_pos.next_turn == position::val::WHITE && k < res)
        {
            mx = nxt[i];
            res = k;
        }
    }
    cur_pos.do_turn(mx);
    return mx;
}

template<class nyan, int max_k>
void minimax_engine<nyan, max_k>::opponent_turn(const turn& t)
{
    cur_pos.do_turn(t);
}

int main()
{
    int a;
    cin >> a;
    if (a == 1)
    {
        reversi_game<minimax_engine<clever_nyan, 3>, human_engine> rv;
        while (true)
        {
            turn t = rv.next_turn();
            if (t == turn(-1, -1))
            {
                cout << "Skip" << endl;
            }
            else
                cout << t.get_s() << endl;
            cout.flush();
            rv.next_turn();
        }
    }
    else
    {
        reversi_game<human_engine, minimax_engine<clever_nyan, 3> > rv;
        while (true)
        {
            rv.next_turn();
            turn t = rv.next_turn();
            if (t == turn(-1, -1))
            {
                cout << "Skip" << endl;
            }
            else
                cout << t.get_s() << endl;
            cout.flush();
        }
    }
    return 0;
}
