#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <queue>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int HEIGHT = 10;
const int WIDTH = 10;
const int POSITIVE_INFINITY = 100000000;
const int NEGATIVE_INFINITY = -100000000;
const int MINIMAX_DEPTH = 2;

class Coordinate
{
public:
    int x;
    int y;
    Coordinate(int x_val, int y_val)
    {
        this->x = x_val;
        this->y = y_val;
    }
};

inline bool operator<(const Coordinate& coord1, const Coordinate& coord2)
{
    if (coord1.x < coord2.x)
        return true;
    else if (coord1.x == coord2.x)
        return coord1.y < coord2.y;
    else
        return false;
}

inline bool operator==(const Coordinate& coord1, const Coordinate& coord2)
{
    if (coord1.x == coord2.x && coord1.y == coord2.y)
        return true;
    else
        return false;
}

inline bool operator>(const Coordinate& coord1, const Coordinate& coord2)
{
    if (coord1.x > coord2.x)
        return true;
    else if (coord1.x == coord2.x)
        return coord1.y > coord2.y;
    else
        return false;
}

inline bool operator>=(const Coordinate& coord1, const Coordinate& coord2)
{
    return !(coord1 < coord2);
}

inline bool operator<=(const Coordinate& coord1, const Coordinate& coord2)
{
    return !(coord1 > coord2);
}

enum Players
{
    PlayerX = 1, PlayerO = 2
};

enum CellValues
{
    CellValueEmpty = 0, CellValueX = 1, CellValueO = 2, CellValueKilledX = 3, CellValueKilledO = 4
};

enum MoveType
{
    MoveTypeMultiply, MoveTypeEat, MoveTypeAuto
};

enum GameStates
{
    GameStateGoing, GameStateWon, GameStateLost, GameStateTie
};

enum PossibleMoveTypes
{
    CellUnreachable, MoveEat, MoveEatOrMult
};

class Node
{
public:
    CellValues Table[WIDTH][HEIGHT];
    int TimeTable[WIDTH][HEIGHT];
    int Time;
    Players Player;
    bool isLeaf;
    long Value;
    long alfa;
    long beta;

    Node(CellValues table[WIDTH][HEIGHT])
    {
        isLeaf = false;
        Value = 0;
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                TimeTable[i][j] = 0;
                Table[i][j] = table[i][j];
            }

    }
};

CellValues get_killed_value(CellValues value)
{
    if (value == CellValueO) return CellValueKilledO;
    if (value == CellValueX) return CellValueKilledX;
    if (value == CellValueKilledO || value == CellValueKilledX)
        return value;
    return CellValueEmpty;
};

CellValues get_player_value(Players player)
{
    if (player == PlayerO)
        return CellValueO;
    else if (player == PlayerX)
        return CellValueX;
    return CellValueEmpty;
};

// Returns opponent of the specified player
Players get_opponent(Players player)
{
    if (player == PlayerO) return PlayerX;
    else if (player == PlayerX) return PlayerO;
}

CellValues get_corresponding_value(Players player)
{
    if (player == PlayerX)
        return CellValueX;
    else if (player == PlayerO)
        return CellValueO;
    else
        return CellValueEmpty;
};

// Get vector with coordinates of adjacent cells
vector<Coordinate> get_adjacent_cells(Coordinate coords)
{
    vector<Coordinate> ret = vector<Coordinate>();
    int x = coords.x;
    int y = coords.y;

    if (x > 0)
    {
        ret.push_back(Coordinate(x - 1, y));
        if (y > 0)
            ret.push_back(Coordinate(x - 1, y - 1));
        if (y < HEIGHT - 1)
            ret.push_back(Coordinate(x - 1, y + 1));
    }
    if (x < WIDTH - 1)
    {
        ret.push_back(Coordinate(x + 1, y));
        if (y > 0)
            ret.push_back(Coordinate(x + 1, y - 1));
        if (y < HEIGHT - 1)
            ret.push_back(Coordinate(x + 1, y + 1));
    }
    if (y > 0)
        ret.push_back(Coordinate(x, y - 1));
    if (y < HEIGHT - 1)
        ret.push_back(Coordinate(x, y + 1));
    return ret;
}

class Viruses
{
public:
    Players player;
    int heur_count;
    CellValues Table[WIDTH][HEIGHT];
    int TimeTable[WIDTH][HEIGHT];

    int heuristic(CellValues table[WIDTH][HEIGHT], Players player, int TimeTable[WIDTH][HEIGHT])
    {
        heur_count++;
        vector<Coordinate> PossibleOpponentMoves = possibleMoves(table, GetOpponentPlayer(player));
        vector<Coordinate> PossibleMoves = possibleMoves(table, player);
        if (PossibleMoves.empty() && PossibleOpponentMoves.empty())
            return 0;
        else if (PossibleMoves.empty())
            return NEGATIVE_INFINITY;
        else if (PossibleOpponentMoves.empty())
            return POSITIVE_INFINITY;

        vector< vector<int> > ConnectionTable = ConnectionsTable(table, player);
        vector< vector<int> > Opponent = OpponentTable(table, player);
        vector< vector<int> > CurrentPlayerTable = OpponentTable(table, GetOpponentPlayer(player));
        vector< vector<int> > ResultTable = SumTables(TimeTable, ConnectionTable, Opponent, CurrentPlayerTable);
        int accumulator = 0;
        for (int i = 0; i < PossibleMoves.size(); ++i)
        {
            accumulator += ResultTable[PossibleMoves[i].x][PossibleMoves[i].y];
        }
        if (player == this->player)
            accumulator = -accumulator;
        return accumulator;
    }

    long long minimax(Node& node, int depth, Players player)
    {
        long long answer = 0;
        if (player == this->player)
            answer = NEGATIVE_INFINITY;
        else
            answer = POSITIVE_INFINITY;
        if (depth == 0 || node.isLeaf)
        {
            return heuristic(node.Table, player, node.TimeTable);
        }
        vector<Coordinate> Children = FindBestMoves(node.Table, node.TimeTable, player);

        stack<CellValues> LastValues;
        vector<Coordinate> Children2;
        vector<Coordinate> Children3;
        for (int i = 0; i < Children.size(); ++i)
        {
            CellValues cell = node.Table[Children[i].x][Children[i].y];
            LastValues.push(cell);
            node.Table[Children[i].x][Children[i].y] = GetKilledVal(cell);
            Children2 = FindBestMoves(node.Table, node.TimeTable, player);
            for (int i2 = 0; i2 < Children2.size(); ++i2)
            {
                cell = node.Table[Children2[i2].x][Children2[i2].y];
                LastValues.push(cell);
                node.Table[Children2[i2].x][Children2[i2].y] = GetKilledVal(cell);
                Children3 = FindBestMoves(node.Table, node.TimeTable, player);
                for (int i3 = 0; i3 < Children3.size(); ++i3)
                {
                    cell = node.Table[Children3[i3].x][Children3[i3].y];
                    LastValues.push(cell);
                    node.Table[Children3[i3].x][Children3[i3].y] = GetKilledVal(cell);
                    if (player == this->player)
                        answer = max (answer, minimax(node, depth-1, GetOpponentPlayer(player)));
                    else
                        answer = min (answer, minimax(node, depth-1, GetOpponentPlayer(player)));
                    node.Table[Children3[i3].x][Children3[i3].y] = LastValues.top();
                    LastValues.pop();
                }
                node.Table[Children2[i2].x][Children2[i2].y] = LastValues.top();
                LastValues.pop();
            }
            node.Table[Children[i].x][Children[i].y] = LastValues.top();
            LastValues.pop();
        }
        return answer;
    }



    static bool isMoveValid(Players player, CellValues table[][HEIGHT], Coordinate coords, MoveType moveType)
    {
        int x = coords.x;
        int y = coords.y;
        vector<Coordinate> Adjacent = get_adjacent_cells(coords);
        if (!doesCellValueCorrespond(table[x][y], player, moveType))
            return false;
        for (int i = 0; i < Adjacent.size(); ++i)
        {
            Coordinate AdjacentCell = Adjacent[i];
            if (Equals(table[AdjacentCell.x][AdjacentCell.y], player))
                return true;
        }
        if ((moveType == MoveTypeEat || (moveType == MoveTypeAuto && table[x][y] != CellValueEmpty))
            && doesCellValueCorrespond(table[x][y], player, moveType))
        {
            if (isKilledConnected(table, coords, GetCorrespondingVal(player)))
                return true;
        }
        return false;
    }

    static bool isKilledConnected(CellValues table[][HEIGHT], Coordinate coords, CellValues searchFor)
    {
        bool visited[WIDTH][HEIGHT];
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
                visited[i][j] = false;
        queue<Coordinate> q;
        q.push(coords);
        while (!q.empty())
        {
            Coordinate current = q.front();
            q.pop();
            visited[current.x][current.y] = true;
            vector<Coordinate> AdjacentCells = get_adjacent_cells(current);
            for (int i = 0; i < AdjacentCells.size(); ++i)
            {
                Coordinate AdjacentCell = AdjacentCells[i];
                CellValues Adjacent = table[AdjacentCell.x][AdjacentCell.y];
                if (Adjacent == searchFor)
                    return true;
                if (((searchFor == CellValueX && Adjacent == CellValueKilledO) ||
                        (searchFor == CellValueO && Adjacent == CellValueKilledX)) &&
                        !visited[AdjacentCell.x][AdjacentCell.y])
                    q.push(AdjacentCell);
            }
        }
        return false;
    }

    static bool doesCellValueCorrespond(CellValues cell, Players& player, MoveType& moveType)
    {
        if ((moveType == MoveTypeMultiply && cell == CellValueEmpty) ||
            (moveType == MoveTypeEat && cell == GetOppositeVal(player)))
            return true;
        if (moveType == MoveTypeAuto && (cell == CellValueEmpty || cell == GetOppositeVal(player)))
            return true;
        return false;
    }

    static CellValues GetKilledVal(CellValues c)
    {
        if (c == CellValueO) return CellValueKilledO;
        if (c == CellValueX) return CellValueKilledX;
        if (c == CellValueKilledO || c == CellValueKilledX)
            return c;
        return CellValueEmpty;
    }

    static CellValues GetCorrespondingVal(Players c)
    {
        if (c == PlayerX)
            return CellValueX;
        if (c == PlayerO)
            return CellValueO;
        return CellValueEmpty;
    }

    static CellValues GetOppositeVal(Players p)
    {
        if (p == PlayerX)
            return CellValueO;
        else if (p == PlayerO)
            return CellValueX;
        else
            throw new exception();
    }

    static bool Equals(CellValues c, Players p)
    {
        return Equals(p, c);
    }

    static bool Equals(Players p, CellValues c)
    {
        if (p == PlayerX && c == CellValueX)
            return true;
        else if (p == PlayerO && c == CellValueO)
            return true;
        return false;
    }

    vector<Coordinate> ValidMoves(CellValues table[][HEIGHT], Players player)
    {
        vector<Coordinate> ret;
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                MoveType moveType = MoveTypeMultiply;
                if (table[i][j] != CellValueEmpty)
                    moveType = MoveTypeEat;
                if (isMoveValid(player, table, Coordinate(i, j), moveType))
                    ret.push_back(Coordinate(i, j));
            }
        return ret;
    }

    static vector<Coordinate> possibleMoves(CellValues table[][HEIGHT], Players player)
    {
        vector<Coordinate> ret;
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                if (isMoveValid(player, table, Coordinate(i, j), MoveTypeAuto))
                    ret.push_back(Coordinate(i, j));
            }
        return ret;
    }

    static vector< vector<int> > ConnectionsTable(CellValues table[][HEIGHT], Players player)
    {
        vector< vector<int> > ConnectionsTable = vector< vector<int> >(WIDTH, vector<int>(HEIGHT));
        CellValues ToFind = GetCorrespondingVal(player);
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                if (table[i][j] != CellValueEmpty)
                    continue;
                vector<Coordinate> lst = get_adjacent_cells(Coordinate(i, j));
                for (int i = 0; i < lst.size(); ++i)
                {
                    Coordinate coord = lst[i];
                    if (table[coord.x][coord.y] == ToFind || table[coord.x][coord.y] == GetKilledVal(GetOppositeVal(player)))
                        ConnectionsTable[coord.x][coord.y]++;
                }
            }
        return ConnectionsTable;
    }

    static vector< vector<int> > OpponentTable(CellValues table[][HEIGHT], Players currPlayer)
    {
        CellValues ToFind = GetOppositeVal(currPlayer);
        vector< vector<int> > OpponentTable = vector< vector<int> >(WIDTH, vector<int>(HEIGHT));
        bool visited[WIDTH][HEIGHT];
        queue<Coordinate> Queue;
        queue<Coordinate> QueueOfKilled;
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                visited[i][j] = false;
                if (table[i][j] == ToFind)
                    Queue.push(Coordinate(i, j));
            }
        while (!Queue.empty())
        {
            Coordinate current = Queue.front();
            Queue.pop();
            vector<Coordinate> lst = get_adjacent_cells(current);
            for (int i = 0; i < lst.size(); ++i)
            {
                Coordinate coord = lst[i];
                if (table[coord.x][coord.y] == CellValueEmpty || table[coord.x][coord.y] == GetCorrespondingVal(currPlayer))
                {
                    if ((OpponentTable[coord.x][coord.y] > OpponentTable[current.x][current.y] + 1
                        && OpponentTable[current.x][current.y] != 3)
                        || OpponentTable[coord.x][coord.y] == 0)
                        OpponentTable[coord.x][coord.y] = OpponentTable[current.x][current.y] + 1;
                    if (!visited[coord.x][coord.y] && table[coord.x][coord.y] != GetCorrespondingVal(currPlayer))
                    {
                        visited[coord.x][coord.y] = true;
                        Queue.push(coord);
                    }
                }
                else if (table[coord.x][coord.y] == GetOppositeVal(currPlayer))
                {
                    OpponentTable[coord.x][coord.y] = 0;
                }
                else if (table[coord.x][coord.y] == GetKilledVal(GetCorrespondingVal(currPlayer)))
                    QueueOfKilled.push(coord);
            }
        }
        while (!QueueOfKilled.empty())
        {
            Coordinate current = QueueOfKilled.front();
            QueueOfKilled.pop();
            vector<Coordinate> lst = get_adjacent_cells(current);
            for (int i = 0; i < lst.size(); ++i)
            {
                Coordinate coord = lst[i];
                if (table[coord.x][coord.y] == CellValueEmpty)
                    OpponentTable[coord.x][coord.y] = 1;
                else if (table[coord.x][coord.y] == GetCorrespondingVal(currPlayer))
                    OpponentTable[coord.x][coord.y] = 0;
            }
        }
        return OpponentTable;
    }

    static vector<Coordinate> FindBestMoves(CellValues table[][HEIGHT], int TimeTable[][HEIGHT], Players player)
    {
        vector< vector<int> > ConnectionTable = ConnectionsTable(table, player);
        vector< vector<int> > Opponent = OpponentTable(table, player);
        vector< vector<int> > CurrentPlayerTable = OpponentTable(table, GetOpponentPlayer(player));
        vector< vector<int> > ResultTable = SumTables(TimeTable, ConnectionTable, Opponent, CurrentPlayerTable);
        vector<Coordinate> SmallVector;
        int max = NEGATIVE_INFINITY;
        int max_val = NEGATIVE_INFINITY;
        int min_val = POSITIVE_INFINITY;
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                if (!isMoveValid(player, table, Coordinate(i, j), MoveTypeAuto))
                {
                    continue;
                }
                if (ResultTable[i][j] > max)
                {
                    max = ResultTable[i][j];
                    SmallVector.clear();
                    SmallVector.push_back(Coordinate(i, j));
                }
                else if (ResultTable[i][j] == max)
                {
                    SmallVector.push_back(Coordinate(i, j));
                }
            }
        return SmallVector;
    }

    static Players GetOpponentPlayer(Players player)
    {
        if (player == PlayerX)
            return PlayerO;
        else if (player == PlayerO)
            return PlayerX;
        else
            return PlayerX;
    }

private:
    static vector< vector<int> > SumTables(int TimeTable[][HEIGHT],
                                           vector< vector<int> >& ConnectionTable,
                                           vector< vector<int> >& Opponent,
                                           vector< vector<int> >& CurrentPlayer)
    {
        vector< vector<int> > ret = vector< vector<int> >(WIDTH, vector<int>(HEIGHT));
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
            {
                ret[i][j] = (- ConnectionTable[i][j]) + Opponent[i][j] + TimeTable[i][j];
                if (Opponent[i][j] == 1 && CurrentPlayer[i][j] == 1)
                    ret[i][j] += 5;
                if (Opponent[i][j] == 0 && CurrentPlayer[i][j] == 1)
                    ret[i][j] += 30;
                if (Opponent[i][j] == 1 && CurrentPlayer[i][j] == 0)
                    ret[i][j] += 15;
                if (Opponent[i][j] == 0 && CurrentPlayer[i][j] == 0)
                    ret[i][j] += 15;
            }
        return ret;
    }
};

int bfs_calls = 0;

class VirusesTable
{
private:
    vector< vector<CellValues> > table; // Current position
    vector< vector<PossibleMoveTypes> > possible_moves; // Table of all possible moves
    set<Coordinate> possible_moves_set; // set of player's possible moves
    vector< vector<PossibleMoveTypes> > possible_opponent_moves; // Possible opponent's moves
    set<Coordinate> possible_opponent_moves_set; // set of opponent's possible moves
    vector< vector<int> > cell_prices_table; // table of all cell prices
    vector< vector<int> > opponent_cell_prices_table; // prices table of all opponent's cells
    vector< vector<int> > lengths_table; // how far is each cell from the opponent's
    vector< vector<int> > time_table; // table showing "time" when each move was made
    int time; // current game time
    Players current_player; // Player that we compute price for
    int price; // Current "price" of the whole table
    GameStates game_state; // State of game being played

    void kill_cell(Coordinate coord);
    void update_price();
    void update_possible_moves();
    void mark_possible_moves(Coordinate start, vector< vector<bool> >& visited, Players player);
    bool does_cell_worth_it(Coordinate coord, Players player);
    vector< vector<int> > get_connections_table();

public:
    // Getters and setters

    vector<Coordinate> get_best_moves();
    Players get_current_player();
    void set_current_player(Players player);
    GameStates get_game_state();
    int get_hash();
    int get_price();
    CellValues get_cell_value(Coordinate coord);
    set<Coordinate>& get_possible_moves_set(Players player);

    // Methods

    void update_prices_table(int);
    bool is_move_possible(Coordinate coord, Players player);
    void make_move(Coordinate coord, Players player);
    void clear();

    // Constructors

    VirusesTable();
    VirusesTable(vector< vector<CellValues> >& table, Players current_player);
    VirusesTable(const VirusesTable& viruses_table);
};

// Getters and setters

CellValues VirusesTable::get_cell_value(Coordinate coord)
{
    return this->table[coord.x][coord.y];
}

set<Coordinate>& VirusesTable::get_possible_moves_set(Players player)
{
    if (player == this->current_player)
        return this->possible_moves_set;
    else
        return this->possible_opponent_moves_set;
}

Players VirusesTable::get_current_player()
{
    return this->current_player;
}

void VirusesTable::set_current_player(Players player)
{
    this->current_player = player;
}

GameStates VirusesTable::get_game_state()
{
    return this->game_state;
}

int VirusesTable::get_price()
{
    return this->price;
}

// Public methods

// returns list of best moves
vector<Coordinate> VirusesTable::get_best_moves()
{
    // we go through all the reachable cells and remember the best ones
    vector<Coordinate> best_moves;
    int max_value = NEGATIVE_INFINITY;
    for (set<Coordinate>::iterator it = this->possible_moves_set.begin(); it != this->possible_moves_set.end(); ++it)
    {
        int x = it->x;
        int y = it->y;
        if (this->cell_prices_table[x][y] > max_value)
        {
            best_moves.clear();
            best_moves.push_back(Coordinate(x, y));
            max_value = this->cell_prices_table[x][y];
        }
        else if (this->cell_prices_table[x][y] == max_value)
        {
            best_moves.push_back(Coordinate(x, y));
        }
    }
    return best_moves;
}

// updates table of prices for each cell
void VirusesTable::update_prices_table(int move_num = 0)
{
    update_possible_moves();
    // table with cells showing number of player's cells around
    vector< vector<int> > connections_table = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // for each cell that is connected with current
            vector<Coordinate> adjacent_cells = get_adjacent_cells(Coordinate(i, j));
            for (int c = 0; c < adjacent_cells.size(); ++c)
                // if cell is player's we increase number of connections in this cell
                if (this->table[adjacent_cells[c].x][adjacent_cells[c].y] == get_corresponding_value(this->current_player))
                    connections_table[i][j]++;
        }
    // table with cells showing number of opponent's cells around
    vector< vector<int> > opponent_connections_table = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // for each cell that is connected with current
            vector<Coordinate> adjacent_cells = get_adjacent_cells(Coordinate(i, j));
            for (int c = 0; c < adjacent_cells.size(); ++c)
                // if cell is opponent's we increase number of connections in this cell
                if (this->table[adjacent_cells[c].x][adjacent_cells[c].y] == get_corresponding_value(get_opponent(this->current_player)))
                    opponent_connections_table[i][j]++;
        }
    // Launch BFS for counting lengths from opponet's nearest cells
    queue<Coordinate> bfs_queue;
    vector< vector<int> > lengths = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    vector< vector<bool> > visited = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
    for (set<Coordinate>::iterator it = possible_opponent_moves_set.begin(); it != possible_opponent_moves_set.end(); ++it)
    {
        visited[it->x][it->y] = true;
        lengths[it->x][it->y] = 1;
        bfs_queue.push(*it);
    }
    while (!bfs_queue.empty())
    {
        Coordinate current = bfs_queue.front();
        bfs_queue.pop();
        vector<Coordinate> adjacent_cells = get_adjacent_cells(current);
        for (vector<Coordinate>::iterator it = adjacent_cells.begin(); it != adjacent_cells.end(); ++it)
        {
            int x = it->x;
            int y = it->y;
            // if cell is empty or it is player's cell
            if (this->table[x][y] == CellValueEmpty || this->table[x][y] == get_corresponding_value(this->current_player))
            {
                // if value in the cell is less than we can get or we didn't go there yet
                if (lengths[x][y] > lengths[current.x][current.y] + 1 || lengths[x][y] == 0)
                {
                    lengths[x][y] = lengths[current.x][current.y] + 1;
                }
                // if we haven't visited this cell and this cell is not player's
                if (!visited[x][y] && this->table[x][y] != get_corresponding_value(this->current_player))
                {
                    // we mark it as visited
                    visited[x][y] = true;
                    // and add it to the queue
                    bfs_queue.push(*it);
                }
            }
        }
    }
    // find nearest killed opponents
    queue<Coordinate> killed_opponents;
    visited = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
    vector< vector<int> > killed_opponents_dists = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
            if (this->table[i][j] == get_killed_value(get_corresponding_value(get_opponent(this->current_player))))
                killed_opponents.push(Coordinate(i, j));
    while (!killed_opponents.empty())
    {
        Coordinate current = killed_opponents.front();
        killed_opponents.pop();
        visited[current.x][current.y] = true;
        vector<Coordinate> adjacent_cells = get_adjacent_cells(current);
        for (int i = 0; i < adjacent_cells.size(); ++i)
        {
            int x = adjacent_cells[i].x;
            int y = adjacent_cells[i].y;
            if (this->table[x][y] == CellValueEmpty)
            {
                if (killed_opponents_dists[x][y] > killed_opponents_dists[current.x][current.y] + 1
                    || killed_opponents_dists[x][y] == 0)
                    killed_opponents_dists[x][y] = killed_opponents_dists[current.x][current.y] + 1;
                if (!visited[x][y])
                {
                    visited[x][y] = true;
                    killed_opponents.push(Coordinate(x, y));
                }
            }
        }
    }
    // sum all the just counted tables
    vector< vector<int> > results_table = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            results_table[i][j] += lengths[i][j] + this->time_table[i][j] - opponent_connections_table[i][j]
                                - connections_table[i][j] - killed_opponents_dists[i][j];
            if (this->time >= 30 && this->table[i][j] == get_corresponding_value(get_opponent(this->current_player))
                && !does_cell_worth_it(Coordinate(i, j), this->current_player))
                results_table[i][j] -= 300;
            vector<Coordinate> adjacent_cells = get_adjacent_cells(Coordinate(i, j));
            bool has_killed_opponent_connection = false;
            int player_killed_cells_num = 0;
            for (int c = 0; c < adjacent_cells.size(); ++c)
            {
                if (this->table[adjacent_cells[c].x][adjacent_cells[c].y]
                    == get_killed_value(get_corresponding_value(get_opponent(this->current_player))))
                    has_killed_opponent_connection = true;
                else if (this->table[adjacent_cells[c].x][adjacent_cells[c].y] == get_killed_value(get_corresponding_value(this->current_player)))
                    player_killed_cells_num++;
            }
            results_table[i][j] -= 2*player_killed_cells_num;
            if (has_killed_opponent_connection && this->possible_moves[i][j] == MoveEatOrMult)
                results_table[i][j] += 10;
            else if (has_killed_opponent_connection && this->possible_moves[i][j] != CellUnreachable
                     && this->table[i][j] == get_corresponding_value(get_opponent(this->current_player)))
                results_table[i][j] += 15;
            if (lengths[i][j] >= 4 && lengths[i][j] <= 8)
                results_table[i][j] -= 2*lengths[i][j];
            else if (lengths[i][j] == 3)
                results_table[i][j] += 5;
            else if (lengths[i][j] == 0)
                results_table[i][j] += 5;
            if (move_num >= 1 && lengths[i][j] == 2)
                results_table[i][j] -= 5;
            if (this->time >= 30 && this->possible_moves[i][j] != CellUnreachable && this->table[i][j] == CellValueEmpty)
                if (!does_cell_worth_it(Coordinate(i, j), get_opponent(this->current_player)))
                    results_table[i][j] -= 200;
            // if cell is opponent's
            if (this->table[i][j] == get_corresponding_value(get_opponent(this->current_player)))
            {
                results_table[i][j] += 30;
                results_table[i][j] += this->time_table[i][j];
                //results_table[i][j] -= opponent_connections_table[i][j];
            }
            // if cell is connected with the opponent's
            else if (lengths[i][j] == 1 && opponent_connections_table[i][j] < 4 && move_num < 2)
            {
                vector<Coordinate> adjacent_cells = get_adjacent_cells(Coordinate(i, j));
                int sum_plus = -1;
                int opponent_cells_num = 0;
                for (int c = 0; c < adjacent_cells.size(); ++c)
                    if (this->table[adjacent_cells[c].x][adjacent_cells[c].y] == get_corresponding_value(get_opponent(this->current_player)))
                    {
                        opponent_cells_num++;
                    }
                results_table[i][j] += 10 + sum_plus;
            }
            // if opponent can eat viruses in this cell
            if (this->possible_opponent_moves[i][j] == MoveEat)
                results_table[i][j] -= 10;
        }
    this->cell_prices_table = results_table;
}

// Checks whether specified player can get to the specified cell
bool VirusesTable::is_move_possible(Coordinate coord, Players player)
{
    if (player == this->current_player)
        return (this->possible_moves[coord.x][coord.y] != CellUnreachable);
    else
        return (this->possible_opponent_moves[coord.x][coord.y] != CellUnreachable);
}

// Clears all the tables and values
void VirusesTable::clear()
{
    this->table = vector< vector<CellValues> >(WIDTH, vector<CellValues>(HEIGHT, CellValueEmpty));
    this->possible_moves.clear();
}

// Computes hash for current position
int VirusesTable::get_hash()
{
    return -1;
}

// Changes cell state
void VirusesTable::make_move(Coordinate coord, Players player)
{
    MoveType move_type = MoveTypeEat;
    if (table[coord.x][coord.y] == CellValueEmpty)
    {
        table[coord.x][coord.y] = get_corresponding_value(player);
        move_type = MoveTypeMultiply;
    }
    else
    {
        kill_cell(coord);
        move_type = MoveTypeEat;
    }

    // update time table and current game time
    this->time_table[coord.x][coord.y] = this->time++;
    // if cell was eaten
    if (move_type == MoveTypeEat)
    {
        // find cells "through" which this move was made
        vector<Coordinate> killers_cells;
        // searching for such cells with bfs
        queue<Coordinate> killers_queue;
        vector< vector<bool> > visited = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
        killers_queue.push(coord);
        while (!killers_queue.empty())
        {
            Coordinate current = killers_queue.front();
            killers_queue.pop();
            visited[current.x][current.y] = true;
            vector<Coordinate> adjacent_cells = get_adjacent_cells(current);
            for (int i = 0; i < adjacent_cells.size(); ++i)
            {
                int x = adjacent_cells[i].x;
                int y = adjacent_cells[i].y;

                if (!visited[x][y] && this->table[x][y] == get_corresponding_value(player))
                {
                    visited[x][y] = true;
                    killers_cells.push_back(adjacent_cells[i]);
                }
                else if (!visited[x][y] && this->table[x][y] == get_killed_value(get_corresponding_value(get_opponent(player))))
                    killers_queue.push(adjacent_cells[i]);
            }
        }
        // and update time table for them
        for (int i = 0; i < killers_cells.size(); ++i)
            this->time_table[killers_cells[i].x][killers_cells[i].y] = time;
    }
    vector< vector<PossibleMoveTypes> >* pos_moves = &this->possible_moves;
    set<Coordinate>* pos_moves_set = &this->possible_moves_set;
    if (this->current_player == player)
    {
        pos_moves = &this->possible_moves;
        pos_moves_set = &this->possible_moves_set;
    }
    else
    {
        pos_moves = &this->possible_opponent_moves;
        pos_moves_set = &this->possible_opponent_moves_set;
    }
    // mark possible moves and unmark impossible
    // We cannot go to the specified cell anymore
    (*pos_moves)[coord.x][coord.y] = CellUnreachable;
    pos_moves_set->erase(coord);
    vector<Coordinate> adjacent_cells = get_adjacent_cells(coord);
    // For all adjacent cells
    for (int i = 0; i < adjacent_cells.size(); ++i)
    {
        int x = adjacent_cells[i].x;
        int y = adjacent_cells[i].y;
        CellValues cell = this->table[x][y];
        // If cell is empty or it's opponent's cell
        if (cell == CellValueEmpty || cell == get_corresponding_value(get_opponent(player)))
        {
            // We can go there
            if ((*pos_moves)[x][y] == MoveEat && move_type == MoveTypeMultiply)
                (*pos_moves)[x][y] = MoveEatOrMult;
            pos_moves_set->insert(Coordinate(x, y));
        }
        // If cell is opponent's killed cell or player's killed cell
        else if (cell == get_killed_value(get_corresponding_value(get_opponent((player))))
                 || cell == get_killed_value(get_corresponding_value(player)))
        {
            // launch breadth-first search from this cell
            vector< vector<bool> > empty_table = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
            mark_possible_moves(adjacent_cells[i], empty_table, player);
        }
    }
    update_price();
}

// Private methods

// Checks wheteher or not cell can make any damage to the (player) in future
bool VirusesTable::does_cell_worth_it(Coordinate coord, Players player)
{
    // BFS again
    queue<Coordinate> reachable_cells;
    vector< vector<bool> > visited = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
    reachable_cells.push(coord);
    while(!reachable_cells.empty())
    {
        Coordinate current = reachable_cells.front();
        reachable_cells.pop();
        visited[current.x][current.y] = true;
        if (this->table[current.x][current.y] == get_corresponding_value(player)
            || this->table[current.x][current.y] == get_killed_value(get_corresponding_value(player)))
            return true;
        vector<Coordinate> adjacent_cells = get_adjacent_cells(current);
        for (int i = 0; i < adjacent_cells.size(); ++i)
        {
            int x = adjacent_cells[i].x;
            int y = adjacent_cells[i].y;
            if (this->table[x][y] == get_corresponding_value(player)
                || this->table[x][y] == get_killed_value(get_corresponding_value(player)))
                return true;
            else if (this->table[x][y] == CellValueEmpty && !visited[x][y])
                reachable_cells.push(adjacent_cells[i]);
        }
    }
    return false;
}

// Computes "price" for current position and stores it in price variable
// Also updates game state
void VirusesTable::update_price()
{
    // price to be returned
    int price = 0;
    // compute number of cells occupied by different cell types
    int num_player_cells = 0;
    int num_killed_player_cells = 0;
    int num_opponent_cells = 0;
    int num_killed_opponent_cells = 0;
    CellValues player_cell = get_corresponding_value(this->current_player);
    CellValues opponent_cell = get_corresponding_value(get_opponent(this->current_player));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            if (table[i][j] == player_cell)
                num_player_cells++;
            else if (table[i][j] == get_killed_value(player_cell))
                num_killed_player_cells++;
            else if (table[i][j] == opponent_cell)
                num_killed_opponent_cells++;
            else if (table[i][j] == get_killed_value(opponent_cell))
                num_killed_opponent_cells++;
        }
    price += num_player_cells;
    price += num_killed_opponent_cells;
    price -= num_killed_player_cells;
    price -= num_opponent_cells;
    Players player = this->current_player;
    Players opponent = get_opponent(player);
    int player_possible_moves = 0;
    int opponent_possible_moves = 0;
    // for each cell in player's possible moves
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // increase number of player's possible moves
            player_possible_moves++;
            // if cell is opponent's alive cell
            if (this->table[i][j] == get_corresponding_value(opponent))
                // plus price value 3
                price += 3;
            else
                // plus price value 1
                price += 1;
        }
    // for each cell in opponent's possible moves
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // increase number of opponent's possible moves
            opponent_possible_moves++;
            // if cell is player's alive cell
            if (this->table[i][j] == get_corresponding_value(player))
                // minus price value 4
                price -= 4;
            else
                // minus price value 1
                price -= 2;
        }
    // price is 0 if no one can do a move (tie)
    if (player_possible_moves == 0 && opponent_possible_moves == 0)
    {
        this->price = 0;
        this->game_state = GameStateTie;
    }
    // if we cannot do a move, we lose
    else if (player_possible_moves == 0)
    {
        this->price = NEGATIVE_INFINITY;
        this->game_state = GameStateLost;
    }
    // if we can do a move, we win
    else if (opponent_possible_moves == 0)
    {
        this->price = POSITIVE_INFINITY;
        this->game_state = GameStateWon;
    }
    // else - price is our computed price and the game is going on
    else
    {
        this->price = price;
        this->game_state = GameStateGoing;
    }
}

// For each cell count number of cells it is connected to
vector< vector<int> > VirusesTable::get_connections_table()
{
    // vector to be returned
    vector< vector<int> > connections = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // We don't look at cells we can't get to
            if (this->possible_moves[i][j] == CellUnreachable)
                continue;
            // for each adjacent cell
            vector<Coordinate> adjacents = get_adjacent_cells(Coordinate(i, j));
            for (int i = 0; i < adjacents.size(); ++i)
                // if it is player's cell
                if (this->table[adjacents[i].x][adjacents[i].y] == get_corresponding_value(this->current_player))
                    connections[i][j]++;
        }
    return connections;
}

// Launch breadth-first search from (start) cell, mark visited cells on the (visited) table
// and save cells that (player) can get to
void VirusesTable::mark_possible_moves(Coordinate start, vector< vector<bool> >& visited, Players player)
{
    bfs_calls++;
    vector< vector<PossibleMoveTypes> >* pos_moves = &this->possible_moves;
    set<Coordinate>* pos_moves_set = &this->possible_moves_set;
    if (player == this->current_player)
    {
        pos_moves = &this->possible_moves;
        pos_moves_set = &this->possible_moves_set;
    }
    else
    {
        pos_moves = &this->possible_opponent_moves;
        pos_moves_set = &this->possible_opponent_moves_set;
    }
    queue<Coordinate> Queue;
    Queue.push(start);
    while (!Queue.empty())
    {
        Coordinate current = Queue.front();
        Queue.pop();
        visited[current.x][current.y] = true;
        vector<Coordinate> adjacent_cells = get_adjacent_cells(current);
        for (int i = 0; i < adjacent_cells.size(); ++i)
        {
            int x = adjacent_cells[i].x;
            int y = adjacent_cells[i].y;
            // exit if we already visited this cell
            if (visited[x][y])
                continue;
            CellValues selected_value = this->table[x][y];
            // If cell is empty
            if (selected_value == CellValueEmpty)
            {
                // We mark it as possible move if we can multiply there
                if (this->table[current.x][current.y] == CellValueKilledO || this->table[current.x][current.y] == CellValueKilledX)
                    (*pos_moves)[x][y] = CellUnreachable;
                else
                {
                    (*pos_moves)[x][y] = MoveEatOrMult;
                    pos_moves_set->insert(Coordinate(x, y));
                }
            }
            // If cell is opponent's killed cell then
            else if (selected_value == get_killed_value(get_corresponding_value(get_opponent(player))))
            {
                // we cannot go there
                (*pos_moves)[x][y] = CellUnreachable;
                pos_moves_set->erase(Coordinate(x, y));
                // but we can kill connected cells so we add it to the queue
                Queue.push(Coordinate(x, y));
            }
            // if cell is player's cell
            else if (selected_value == get_corresponding_value(player))
            {
                // we mark it as not allowed for the move
                (*pos_moves)[x][y] = CellUnreachable;
                pos_moves_set->erase(Coordinate(x, y));
                // and add it to the queue
                Queue.push(Coordinate(x, y));
            }
            // if cell is opponent's cell
            else if (selected_value == get_corresponding_value(get_opponent(player)))
            {
                // We mark it as possible move
                (*pos_moves)[x][y] = MoveEat;
                pos_moves_set->insert(Coordinate(x, y));
            }
        }
    }
}

// Updates possible moves and opponent's moves tables
void VirusesTable::update_possible_moves()
{
    this->possible_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->possible_moves_set = set<Coordinate>();
    this->possible_opponent_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->possible_opponent_moves_set = set<Coordinate>();
    vector< vector<bool> > visited_player = vector< vector<bool> >(WIDTH, vector<bool>(HEIGHT, false));
    vector< vector<bool> > visited_opponent = vector< vector<bool> >(HEIGHT, vector<bool>(HEIGHT, false));
    // for each cell in table
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
        {
            // if cell has not been visited yet
            if (!visited_player[i][j])
            {
                // if this cell is player's
                if (this->table[i][j] == get_corresponding_value(this->current_player))
                    // launch search for all cells reachable from this cell
                    mark_possible_moves(Coordinate(i, j), visited_player, this->current_player);
            }
            if (!visited_opponent[i][j])
            {
                // if this cell is opponent's
                if (this->table[i][j] == get_corresponding_value(get_opponent(this->current_player)))
                    // launch search for all cells reachable from this cell
                    mark_possible_moves(Coordinate(i,j), visited_opponent, get_opponent(this->current_player));
            }
        }
}

// Set cell's state to the killed one.
// (Does nothing if cell specified is empty or already killed)
void VirusesTable::kill_cell(Coordinate coord)
{
    if (table[coord.x][coord.y] == CellValueO)
    {
        this->table[coord.x][coord.y] = CellValueKilledO;
    }
    else if (table[coord.x][coord.y] == CellValueX)
    {
        this->table[coord.x][coord.y] = CellValueKilledX;
    }
}

// Constructors

VirusesTable::VirusesTable(vector< vector<CellValues> >& table, Players current_player)
{
    this->current_player = current_player;
    this->table = vector< vector<CellValues> >(table);
    this->time = 0;
    this->time_table = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    this->possible_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->possible_opponent_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->update_possible_moves();
    this->update_price();
}

VirusesTable::VirusesTable()
{
    this->current_player = PlayerX;
    this->time = 0;
    this->table = vector< vector<CellValues> >(WIDTH, vector<CellValues>(HEIGHT, CellValueEmpty));
    this->possible_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->possible_opponent_moves = vector< vector<PossibleMoveTypes> >(WIDTH, vector<PossibleMoveTypes>(HEIGHT, CellUnreachable));
    this->time_table = vector< vector<int> >(WIDTH, vector<int>(HEIGHT, 0));
    this->update_possible_moves();
    this->update_price();
}

VirusesTable::VirusesTable(const VirusesTable& viruses_table)
{
    this->price = viruses_table.price;
    this->time = viruses_table.time;
    this->current_player = viruses_table.current_player;
    this->game_state = viruses_table.game_state;

    this->table = vector< vector<CellValues> >(viruses_table.table);
    this->time_table = vector< vector<int> >(viruses_table.time_table);
    this->possible_moves = vector< vector<PossibleMoveTypes> >(viruses_table.possible_moves);
    this->possible_moves_set = set<Coordinate>(viruses_table.possible_moves_set);
    this->possible_opponent_moves = vector< vector<PossibleMoveTypes> >(viruses_table.possible_opponent_moves);
    this->possible_opponent_moves_set = set<Coordinate>(viruses_table.possible_opponent_moves_set);
}

int num_mini = 0;

void sort_triple(deque<Coordinate>& triple)
{
    deque<Coordinate> temp_triple = triple;
    if (triple[0] > triple[1])
    {
        if (triple[0] > triple[2])
            if (triple[1] < triple[2])
            {
                // s[0] > s[2] > s[1]
                triple[1] = temp_triple[2];
                triple[2] = temp_triple[1];
            }
        else
        {
            // s[2] > s[0] > s[1]
            triple[0] = temp_triple[2];
            triple[1] = temp_triple[0];
            triple[2] = temp_triple[1];
        }
    }
    else
    {
        if (triple[1] < triple[2])
        {
            // s[2] > s[1] > s[0]
            triple[0] = temp_triple[2];
            triple[1] = temp_triple[1];
            triple[2] = temp_triple[0];
        }
        else
        {
            if (triple[2] > triple[0])
            {
                // s[1] > s[2] > s[0]
                triple[0] = temp_triple[1];
                triple[1] = temp_triple[2];
                triple[2] = temp_triple[0];
            }
            else
            {
                // s[1] > s[0] > s[2]
                triple[0] = temp_triple[1];
                triple[1] = temp_triple[0];
                triple[2] = temp_triple[2];
            }
        }
    }
}

int minimax(VirusesTable& table, int depth, int alfa, int beta, Players player)
{
    // set of all visited triples
    set< deque<Coordinate> > visited_triples;
    if (depth <= 0 || table.get_game_state() != GameStateGoing)
    {
        num_mini++;
        return table.get_price();
    }
    bool stop_cycle = false;
    // Go through all the possible triple-moves
    set<Coordinate> pos_moves = table.get_possible_moves_set(player);
    //set< deque<Coordinate> >::iterator triples_iterator = visited_triples.begin();
    deque<Coordinate> current_triple;
    for (set<Coordinate>::iterator first = pos_moves.begin(); first != pos_moves.end() && !stop_cycle; ++first)
    {
        // add first number to the deque
        current_triple.push_back(*first);
        VirusesTable table1 = VirusesTable(table);
        table1.make_move(*first, player);
        set<Coordinate>& pos_moves1 = table1.get_possible_moves_set(player);
        for (set<Coordinate>::iterator second = pos_moves1.begin(); second != pos_moves1.end() && !stop_cycle; ++second)
        {
            // add second number to the deque
            current_triple.push_back(*second);
            VirusesTable table2 = VirusesTable(table1);
            table2.make_move(*second, player);
            set<Coordinate>& pos_moves2 = table2.get_possible_moves_set(player);
            for (set<Coordinate>::iterator third = pos_moves2.begin(); third != pos_moves2.end() && !stop_cycle; ++third)
            {
                // add third number to the deque
                current_triple.push_back(*third);
                deque<Coordinate> sorted_current_triple = current_triple;
                // sort triple
                sort_triple(sorted_current_triple);
                // if we already visited this triple, don't go there again
                if (visited_triples.find(sorted_current_triple) != visited_triples.end())
                {
                    current_triple.pop_back();
                    continue;
                }
                else
                    visited_triples.insert(current_triple);
                VirusesTable table3 = VirusesTable(table2);
                table3.make_move(*third, player);

                if (player == table.get_current_player())
                {
                    alfa = max(alfa, minimax(table3, depth-1, alfa, beta, get_opponent(player)));
                    if (alfa > beta)
                    {
                        stop_cycle = true;
                        current_triple.pop_back();
                        break;
                    }
                }
                else
                {
                    beta = min(beta, minimax(table3, depth-1, alfa, beta, get_opponent(player)));
                    if (alfa > beta)
                    {
                        stop_cycle = true;
                        current_triple.pop_back();
                        break;
                    }
                }
                //pop third number from the deque
                current_triple.pop_back();
            }
            //pop second number from the deque
            current_triple.pop_back();
        }
        //pop first number from the deque
        current_triple.pop_back();
    }
    if (player == table.get_current_player())
        return alfa;
    else
        return beta;
}

bool isDigit(const char& c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
        return true;
    else
        return false;
}

void printTable(vector< vector<CellValues> >& table, Players player)
{
    char me, you, kme, kyou;
    if (player == PlayerX)
    {
        me = 'X';
        you = 'O';
        kme = 'Z';
        kyou = '0';
    }
    else
    {
        me = 'O';
        you = 'X';
        kme = '0';
        kyou = 'Z';
    }
    cout << "me(computer) - " << me << "; you(player) - " << you << endl;
    cout << "killed me(computer) - " << kme << "; killed you(player) - " << kyou << endl;
    cout << " ";
    for (int c = 0; c < WIDTH; ++c)
        cout << c;
    cout << endl;
    for (int i = HEIGHT-1; i >=0; --i)
    {
        cout << i;
        for (int j = 0; j < WIDTH; ++j)
        {
            if (table[j][i] == CellValueEmpty)
                cout << "_";
            else if (table[j][i] == CellValueX)
                cout << "X";
            else if (table[j][i] == CellValueO)
                cout << "O";
            else if (table[j][i] == CellValueKilledO)
                cout << "0";
            else if (table[j][i] == CellValueKilledX)
                cout << "Z";
        }
        cout << i;
        cout << endl;
    }
    cout << " ";
    for (int i = 0; i < WIDTH; ++i)
        cout << i;
    cout << endl;
}

string parse_args(string args, vector< vector<CellValues> >& table, Players player)
{
    while(!isDigit(args[0]) && args[0] != 'e')
    {
        if (args[0] == 'o' || args == "output")
        {
            printTable(table, player);
        }
        else if (args[0] == 'p')
        {
            int i = 0;
            for (i = 0; i < args.size()-1; ++i)
            {
                if (args[i] == ' ')
                    break;
            }
            int depth = MINIMAX_DEPTH;
            cout << "set minimax depth: ";
            cin >> depth;
            VirusesTable vir_table = VirusesTable(table, player);
            cout << "table price: " << vir_table.get_price() << endl;
            cout << "minimax value with depth = " << depth << ": " << endl;
            cout << "for computer: " << endl;
            cout << minimax(vir_table, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, player) << endl;
            cout << "number of bfs calls: " << bfs_calls << endl;
            cout << "number of minimax calls: " << num_mini << endl;
            bfs_calls = 0;
            num_mini = 0;
            cout << "for player: " << endl;
            cout << minimax(vir_table, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, get_opponent(player)) << endl;
            cout << "number of bfs calls: " << bfs_calls << endl;
            cout << "number of minimax calls: " << num_mini << endl;
            bfs_calls = 0;
            num_mini = 0;
        }
        cin >> args;
    }
    return args;
}

int main(int argc, char* argv[])
{
    VirusesTable vir_table;
    bool debug = false;
    #ifdef DEBUG
    debug = true;
    #endif
    if (argc == 2 && strcmp(argv[1], "--debug") == 0)
        debug = true;
    int player_char = 0;
    Players currPlayer;
    int TimeTable[WIDTH][HEIGHT];
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j)
            TimeTable[i][j] = 0;
    int time = 0;
    cin >> player_char;
    if (player_char == 2)
    {
        currPlayer = PlayerO;
        vir_table.set_current_player(PlayerO);
        string moves;
        cin >> moves;
        for (int i = 0; i < 3; ++i)
        {
            Coordinate coords = Coordinate(moves[2*i] - '0', moves[2*i+1] - '0');
            vir_table.make_move(coords, PlayerX);
            TimeTable[coords.x][coords.y] = time++;
        }
    }
    else
        currPlayer = PlayerX;
    vir_table.set_current_player(currPlayer);
    bool first = true;
    while (true)
    {
        stringstream ss;
        if (first)
        {
            if (currPlayer == PlayerX)
            {
                ss << "00";
                vir_table.make_move(Coordinate(0, 0), PlayerX);
            }
            else
            {
                ss << "99";
                vir_table.make_move(Coordinate(9, 9), PlayerO);
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (first)
            {
                first = false;
                continue;
            }
            vir_table.update_prices_table(i);
            vector<Coordinate> NextMoves = vir_table.get_best_moves();
            //random_shuffle(NextMoves.begin(), NextMoves.end());
            if (NextMoves.empty()) break;
            int x = NextMoves[0].x;
            int y = NextMoves[0].y;
            vir_table.make_move(NextMoves[0], vir_table.get_current_player());
            ss << x << y;
        }
        vector< vector<CellValues> > temp_table = vector< vector<CellValues> >(WIDTH, vector<CellValues>(HEIGHT, CellValueEmpty));
        for (int i = 0; i < WIDTH; ++i)
            for (int j = 0; j < HEIGHT; ++j)
                temp_table[i][j] = vir_table.get_cell_value(Coordinate(i, j));
        if (debug)
        {
            printTable(temp_table, currPlayer);
        }
        cout << ss.str() << endl;
        string moves;
        cin >> moves;
        moves = parse_args(moves, temp_table, currPlayer);
        if (moves[0] == 'e')
            return 0;
        for (int i = 0; i < moves.length()/2; ++i)
        {
            Coordinate coords = Coordinate(moves[2*i] - '0', moves[2*i+1] - '0');
            vir_table.make_move(coords, get_opponent(vir_table.get_current_player()));
            TimeTable[coords.x][coords.y] = time++;
        }
    }
    return 0;
}
