#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const long long CANNOT_GO_BONUS = -81;
const long long PIECE = 100;
const long long CORNER_PIECE_BONUS = 397;
const long long BORDER_PIECE_BONUS = 482;
const long long ADJACENT_TO_CORNER_BONUS = 509;
const long long DIAGONAL_TO_CORNER_BONUS = -303;
struct field_type
{
    unsigned long long black, white;
    field_type(bool a)
    {
        if (a)
        {
        black = 1LL << 27 | 1LL << 36;
        white = 1LL << 28 | 1LL << 35;
        }
        else
        {
            white = 0;
            black = 0;
        }
    }

    short get(short point)
    {
        return -1 * ((black & (1LL << point)) >> point) + ((white & (1LL << point)) >> point);

    }

    void set(short point, short player)
    {
        if (player >= 0)
        {
            white |= 1LL << point;
            black &= ~(1LL << point);
        }
        else
        {
            black |= 1LL << point;
            white &= ~(1LL << point);
        }
    }

    void copy(field_type from)
    {
        black = from.black;
        white = from.white;
    }

    void null()
    {
        black = 0;
        white = 0;
    }
    bool operator!=(field_type other)
    {
        return black != other.black || white != other.white;
    }
    void print()
    {
        for (int i = 0; i < 64; i++)
        {
            cerr << (get(i) + 3) % 3;
            if ((i & 7) == 7)
            {
                cerr << endl;
            }
        }
        cerr << endl;
    }
};

field_type input()
{
    field_type f = field_type(false);
    char a;
    for (int i = 0; i < 64; i++)
    {
        cin >> a;
        if (a == '2')
        {
            f.black += 1;
        }
        else if (a == '1')
        {
            f.white += 1;
        }
        f.black *= 2;
        f.white *= 2;
    }
}

const field_type NULL_FIELD = field_type(false);

short placetogo = 0;

struct score_type
{
    long value;
    bool is_inf;
    score_type(long val)
    {
        value = val;
    }
    score_type()
    {
        value = 0;
    }

    score_type operator*(int other)
    {
        score_type a = score_type(value * other);
        a.is_inf = is_inf;
        return a;
    }

    void operator+=(int other)
    {
        value += other;
    }

    void operator-=(int other)
    {
        value -= other;
    }

    bool operator<(score_type other)
    {
        if (is_inf && (value < 0))
        {
            return (!other.is_inf) || (!(other.value >= 0));
        }
        if (is_inf)
        {
            return false;
        }
        return value < other.value;
    }
    bool operator>(score_type other)
    {
        return other < *this;
    }
    score_type operator-()
    {
        return *this * -1;
    }
    bool operator==(score_type other)
    {
        if (is_inf && other.is_inf)
        {
            return value * other.value > 0;
        }
        else if (is_inf || other.is_inf)
        {
            return false;
        }
        else
        {
            return value == other.value;
        }
    }
    bool operator<=(score_type other)
    {
        if (is_inf && other.is_inf)
        {
            return value > 0 || other.value < 0;
        }
        else if (is_inf)
        {
            return value < 0;
        }
        else if (other.is_inf)
        {
            return other.value > 0;
        }
        else
        {
            return value <= other.value;
        }
    }
};

string to18ah(short point)
{
    if (point < 0)
    {
        return "Skip";
    }
    string a = "";
    a += (int)'a' + (point >> 3);
    a += (char)((int)'1' + (char)(point & 7));
    return a;
}

short from18ah(string point)
{
    if (point == "Skip")
    {
        return -1;
    }
    return ((int)point[1] - (int)'1') + ((int)point[0] - (int)'a') * 8;
}

int getpot(field_type field, short player, short point)
{
    short pot = 0;
    if (field.get(point) != 0)
    {
        return 0;
    }
    short newpoint = point;
    vector<pair<short, short> > moves;
    vector<pair<short, short> > finmoves;
    for (short i = -1; i < 2; i++)
    {
        for (short j = -1; j < 2; j++)
        {
            if (i * i + j * j != 0)
            {
                if (point + 8 * i + j >= 0 && point + 8 * i + j <= 63 && field.get(point + 8 * i + j) == -player)
                {
                    moves.push_back(make_pair(i, j));
                }
            }
        }
    }
    for (int i = 0; i < moves.size(); i++)
    {
        newpoint = point;
        pair<short, short> nowmove = moves[i];
        short x = nowmove.first;
        short y = nowmove.second;
        while (newpoint + 8 * x + y >= 0 && newpoint + 8 * x + y <= 63 && field.get(newpoint + 8 * x + y) == -player)
        {
            newpoint = newpoint + 8 * x + y;
        }
        if (newpoint + 8 * x + y >= 0 && newpoint + 8 * x + y <= 63 && field.get(newpoint + 8 * x + y) == player)
        {
            finmoves.push_back(moves[i]);
        }
    }
    if (finmoves.size() == 0)
    {
        return 0;
    }
    for (int i = 0; i < finmoves.size(); i++)
    {
        newpoint = point;
        pair<short, short> nowmove = moves[i];
        short x = nowmove.first;
        short y = nowmove.second;
        while (newpoint + 8 * x + y >= 0 && newpoint + 8 * x + y <= 63 && field.get(newpoint + 8 * x + y) == -player)
        {
            pot++;
            newpoint = newpoint + 8 * x + y;
        }
    }
    pot -= 7;
    return pot;
}

field_type cango(field_type field, short player, short point)
{
    //cerr << "CANGO LINE IN PROGRESS" << endl;
    if (field.get(point) != 0)
    {
        field_type f = field_type(false);
        return f;
    }
    short newpoint = point;
    vector<pair<short, short> > moves;
    vector<pair<short, short> > finmoves;
    for (short i = -1; i < 2; i++)
    {
        for (short j = -1; j < 2; j++)
        {
            if (i * i + j * j != 0)
            {
                short to = newpoint + 8 * i + j;
                if (abs((newpoint >> 3) - (to >> 3)) <= 1 && abs((newpoint & 7) - (to & 7)) <= 1 && to >= 0 && to <= 63 && field.get(to) == -player)
                {
                    //cerr << i << " " << j << " in moves now!" << endl;
                    moves.push_back(make_pair(i, j));
                }
            }
        }
    }
    for (int i = 0; i < moves.size(); i++)
    {
        newpoint = point;
        pair<short, short> nowmove = moves[i];
        short x = nowmove.first;
        short y = nowmove.second;
        //cout << x << ' ' << y << endl;
        short to = newpoint + 8 * x + y;
        do
        {
            newpoint = to;
            to = newpoint + 8 * x + y;
        }
        while (abs((newpoint >> 3) - (to >> 3)) <= 1 && abs((newpoint & 7) - (to & 7)) <= 1 && (to >= 0) && (to <= 63) && field.get(newpoint) == -player);
        if (field.get(newpoint) == player)
        {
            //cerr << x << " " << y << " in finmoves now!" << endl;
            finmoves.push_back(moves[i]);
        }
    }
    if (finmoves.size() == 0)
    {
        return field_type(false);
    }
    field_type newfield = field_type(true);
    newfield.copy(field);
    newfield.set(point, player);
    for (int i = 0; i < finmoves.size(); i++)
    {
        newpoint = point;
        pair<short, short> nowmove = finmoves[i];
        short x = nowmove.first;
        short y = nowmove.second;
        short to = newpoint + 8 * x + y;
        do
        {
            newpoint = to;
            newfield.set(newpoint, player);
            to = newpoint + 8 * x + y;
        }
        while (abs((newpoint >> 3) - (to >> 3)) <= 1 && abs((newpoint & 7) - (to & 7)) <= 1 && to >= 0 && to <= 63 && field.get(to) == -player);
        newfield.set(newpoint, player);
    }
    //cout << endl;
    return newfield;
}


bool hasmove(field_type position, short player)
{
    for (short i = 0; i < 64; i++)
    {
        if (cango(position, player, i) != NULL_FIELD)
        {
            return true;
        }
    }
    return false;
}

score_type tally(field_type position)
{
    int score = 0;
    for (short i = 0; i < 64; i++)
    {
        score += position.get(i);
    }
    score_type tally = score_type(score);
    tally.is_inf = (score != 0);
}

pair<short, short> gentally(field_type position)
{
    short black = 0, white = 0;
    for (short i = 0; i < 64; i++)
    {
        if (position.get(i) > 0)
        {
            white += position.get(i);
        }
        else
        {
            black -= position.get(i);
        }
    }
    return make_pair(white, black);
}

score_type getscore(field_type position, short player)
{
    score_type score(0);
    if (!hasmove(position, -player))
    {
        if (!hasmove(position, player))
        {
            return tally(position) * player;
        }
        else
        {
            score += CANNOT_GO_BONUS;
        }
    }
    for (short i = 0; i < 64; i++)
    {
        score += position.get(i) * PIECE;
        if (i < 8 || i >= 56 || ((i & 7) == 0) || ((i & 7) == 7))
        {
            score += position.get(i) * BORDER_PIECE_BONUS;
        }
        if (i == 0 || i == 7 || i == 56 || i == 63)
        {
            score += position.get(i) * CORNER_PIECE_BONUS;
        }
        if (i == 1 || i == 6 || i == 8 || i == 15 || i == 48 || i == 55 || i == 57 || i == 62)
        {
            score += position.get(i) * ADJACENT_TO_CORNER_BONUS;
        }
        if (i == 9 || i == 14 || i == 49 || i == 54)
        {
            score += position.get(i) * DIAGONAL_TO_CORNER_BONUS;
        }
    }
    //for (short i = 0; i < 64; i++)
    //{
    //    score -= getpot(position, -1, i);
    //    score += getpot(position, 1, i);
    //}
    return score * player;
}


score_type alpha_beta(field_type position, short turn, short depth, score_type alpha, score_type beta)
{
    if (depth == 0)
    {
        return getscore(position, turn);
    }
    if (!hasmove(position, turn))
    {
        if (!hasmove(position, -turn))
        {
            placetogo = -1;
            return tally(position) * turn;
        }
        score_type sc = -alpha_beta(position, -turn, depth, -beta, -alpha);
        placetogo = -1;
        return sc;
    }
    score_type bestval = score_type(-1);
    bestval.is_inf = true;
    short myplace = -1;
    for (int i = 0; i < 64; i++)
    {
        field_type a = cango(position, turn, i);
        if (a != NULL_FIELD)
        {
            score_type nbestval = -alpha_beta(a, -turn, depth - 1, -beta, -alpha);
            if (bestval < nbestval)
            {
                myplace = i;
                bestval = nbestval;
            }
            if (alpha < nbestval)
            {
                alpha = nbestval;
            }
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    placetogo = myplace;
    return bestval;
}

field_type position_now = field_type(true);

int main()
{
    for (int i = 0; i < 64; i++)
    {
        from18ah(to18ah(i));
    }
    bool player;
    string input;
    cin >> input;
    player = '2' - input[0];
    //position_now.print();
    if (!player)
    {
        cin >> input;
        position_now = cango(position_now, 1, from18ah(input));
    }
    score_type minimum = score_type(-1);
    score_type maximum = score_type(1);
    while (true)
    {
        minimum.is_inf = true;
        maximum.is_inf = true;
        alpha_beta(position_now, 2 * (int)player - 1, 6, minimum, maximum);
        //cerr << "going" << endl;
        //position_now.print();
        //cerr << "to" << endl;
        if (!hasmove(position_now, 2 * (int)player - 1) && !hasmove(position_now, 1 - 2 * (int)player))
        {
            cout << "STOP " << gentally(position_now).first << ":" << gentally(position_now).second;
            return 0;
        }
        if (placetogo != -1)
        {
            position_now = cango(position_now, 2 * (int)player - 1, placetogo);
        }
        //position_now.print();
        cout << to18ah(placetogo) << endl;
        cin >> input;
        if (from18ah(input) != -1)
        {
            position_now = cango(position_now, 1 - 2 * (int)player, from18ah(input));
        }
        //player = !player;

    }
    return 0;
}
