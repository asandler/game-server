#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <set>
#include <list>
#include <time.h>
using namespace std;

#define EMPTY 5
#define PLAYER1 0
#define PLAYER2 1
#define KILLED_PLAYER1 2
#define KILLED_PLAYER2 3
#define TIME 100000000

const int INF = 100000001;

int desk[10][10], my_player, start_of_game = 1;
set < pair < int, int > > viruses[2];
pair < int, int > optimal_turn[3];

void print_desk()
{
    cout << "   ";
    for (int i = 0; i < 10; i++)
        cout << i << " ";
    cout << endl << endl ;
    for (int i = 0; i < 10; i++)
    {
        cout << i << "  ";
        for (int j = 0; j < 10; j++)
        {
            if (desk[j][i] == PLAYER1)
                cout << "X";
            if (desk[j][i] == PLAYER2)
                cout << "0";
            if (desk[j][i] == EMPTY)
                cout << "*";
            if (desk[j][i] == KILLED_PLAYER1)
                cout << "#";
            if (desk[j][i] == KILLED_PLAYER2)
                cout << "@";
            cout << " ";
        }
        cout << endl;
    }
    cout << endl << "********************************************" << endl;
}

void init()
{
    srand(time(0));
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            desk[i][j] = EMPTY;
}

int in_desk(int x, int y)
{
    return x >= 0 && y >= 0 && x < 10 && y < 10;
}

int bfs(int player)
{
    int distance[10][10];
    list < pair < int, int > > deck;
    int dx, dy;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            distance[i][j] = -1;
    set < pair < int, int > > :: iterator k;
    for (k = viruses[player].begin(); k != viruses[player].end(); k++)
    {
        deck.push_back(*k);
        distance[(*k).first][(*k).second] = 0;
    }

    while (deck.size())
    {
        pair < int, int > frst;
        frst = *deck.begin();
        deck.erase(deck.begin());

        for (dx = -1; dx <= 1; dx++)
            for (dy = -1; dy <= 1; dy++)
            {
                int x = frst.first + dx;
                int y = frst.second + dy;
                if (!(in_desk(x, y) && (dx || dy)))
                    continue;
                if (distance[x][y] == -1)
                {
                    if (desk[x][y] == EMPTY && desk[frst.first][frst.second] != !player + 2)
                    {
                        distance[x][y] = distance[frst.first][frst.second] + 1;
                        deck.push_back(make_pair(x, y));
                    }
                    if (desk[x][y] == !player + 2)
                    {
                        distance[x][y] = distance[frst.first][frst.second];
                        deck.push_front(make_pair(x, y));
                    }
                    if (desk[x][y] == !player)
                        return distance[frst.first][frst.second] + 1;
                }
            }
    }
    return 100000;
}

void trans_bfs(int player, int distance[10][10])    //shortest way from friends to enemy
{
    list < pair < int, int > > deck;
    int dx, dy;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            distance[i][j] = INF;
    set < pair < int, int > > :: iterator k;
    for (k = viruses[!player].begin(); k != viruses[!player].end(); k++)
    {
        deck.push_back(*k);
        distance[(*k).first][(*k).second] = 0;
    }

    while (deck.size())
    {
        pair < int, int > frst;
        frst = *deck.begin();
        deck.erase(deck.begin());

        for (dx = -1; dx <= 1; dx++)
            for (dy = -1; dy <= 1; dy++)
            {
                int x = frst.first + dx;
                int y = frst.second + dy;
                if (!(in_desk(x, y) && (dx || dy)))
                    continue;
                if (distance[frst.first][frst.second] + 1 < distance[x][y])
                    if ((desk[frst.first][frst.second] == player && desk[x][y] == EMPTY) || (desk[frst.first][frst.second] == EMPTY && desk[x][y] == player) || (desk[frst.first][frst.second] == player && desk[x][y] == player) || (desk[frst.first][frst.second] == EMPTY && desk[x][y] == EMPTY) || (desk[frst.first][frst.second] == !player && desk[x][y] == EMPTY)  || (desk[frst.first][frst.second] == !player && desk[x][y] == !player + 2) || (desk[frst.first][frst.second] == !player && desk[x][y] == player))
                    {
                        distance[x][y] = distance[frst.first][frst.second] + 1;
                        deck.push_back(make_pair(x, y));
                    }
                if (distance[frst.first][frst.second] < distance[x][y])
                    if ((desk[frst.first][frst.second] == !player + 2 && desk[x][y] == player) || (desk[frst.first][frst.second] == !player + 2 && desk[x][y] == !player + 2) || (desk[frst.first][frst.second] == !player + 2 && desk[x][y] == EMPTY))
                    {
                        distance[x][y] = distance[frst.first][frst.second];
                        deck.push_front(make_pair(x, y));
                    }
            }
    }
}

void print_dist(int player)
{
    int dist[10][10];
    printf("\n");
    trans_bfs(!player, dist);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            if (dist[j][i] == INF)
                printf("%2d", -1);
            else
                printf("%2d", dist[j][i]);
        printf("\n");
    }
}

void cut_available(int player, vector < pair < int, int > > &available)
{
    int distance[10][10], i, j;
    vector < pair < int, int > > old_available;
    for (i = 0; i < available.size(); i++)
        old_available.push_back(available[i]);
    available.clear();
    trans_bfs(!player, distance);
    int min_dist_from_enemy = INF + 1, max_dist_from_enemy = -1;
    for (i = 0; i < old_available.size(); i++)
        if (desk[old_available[i].first][old_available[i].second] == !player)
        {
            max_dist_from_enemy = max(max_dist_from_enemy, distance[old_available[i].first][old_available[i].second]);
            min_dist_from_enemy = min(min_dist_from_enemy, distance[old_available[i].first][old_available[i].second]);
        }
    for (i = 0; i < old_available.size(); i++)
        if (desk[old_available[i].first][old_available[i].second] == !player && distance[old_available[i].first][old_available[i].second] == min_dist_from_enemy)
            available.push_back(old_available[i]);

    int min_dist_to_enemy = INF + 1;
    trans_bfs(player, distance);
    for (i = 0; i < old_available.size(); i++)
        if (desk[old_available[i].first][old_available[i].second] == EMPTY)
            min_dist_to_enemy = min(min_dist_to_enemy, distance[old_available[i].first][old_available[i].second]);
    for (i = 0; i < old_available.size(); i++)
        if (desk[old_available[i].first][old_available[i].second] == EMPTY && distance[old_available[i].first][old_available[i].second] <= min_dist_to_enemy + 1)
        {
            if (available.size() > 30)
                break;
            available.push_back(old_available[i]);
        }
}

void add_to_available(vector < pair < int, int > > &available, int used[10][10], int x, int y)
{
    if (!used[x][y])
        available.push_back(make_pair(x, y));
    used[x][y] = 1;
}

void dfs(int i, int j, vector < pair < int, int > > &available, int player, int used_dfs[10][10], int used[10][10])
{
    vector < pair < int, int > > stk;
    int dx, dy;
    if (!used_dfs[i][j])
        stk.push_back(make_pair(i, j));
    pair < int, int > last;
    while (stk.size())
    {
        used_dfs[stk[stk.size() - 1].first][stk[stk.size() - 1].second] = 1;
        last = stk[stk.size() - 1];
        stk.erase(--stk.end());
        for (dx = -1; dx <= 1; dx++)
            for (dy = -1; dy <= 1; dy++)
            {
                int x = last.first + dx;
                int y = last.second + dy;
                if (!(in_desk(x, y) && (dx || dy)))
                    continue;
                if (!used_dfs[x][y])
                    if (desk[x][y] == !player + 2)
                        stk.push_back(make_pair(x, y));
                if (desk[x][y] == !player)
                    add_to_available(available, used, x, y);
            }
    }
}

int count_chains(int player)
{
    int used_dfs[10][10] = {};
    int chains = 0;
    vector < pair < int, int > > stk;
    int dx, dy;
    set < pair < int, int > > :: iterator k;
    for (k = viruses[player].begin(); k != viruses[player].end(); k++)
    {
        if (!used_dfs[(*k).first][(*k).second])
            stk.push_back(*k);
        pair < int, int > last;
        while (stk.size())
        {
            used_dfs[stk[stk.size() - 1].first][stk[stk.size() - 1].second] = 1;
            last = stk[stk.size() - 1];
            stk.erase(--stk.end());
            for (dx = -1; dx <= 1; dx++)
                for (dy = -1; dy <= 1; dy++)
                {
                    int x = last.first + dx;
                    int y = last.second + dy;
                    if (!(in_desk(x, y) && (dx || dy)))
                        continue;
                    if (!used_dfs[x][y])
                        if (desk[x][y] == !player + 2)
                        {
                            ++chains;
                            stk.push_back(make_pair(x, y));
                        }
                }
        }
    }
    return chains;
}

void find_available(int player, vector < pair < int, int > > &available)
{
    int i, j, dx, dy, used_dfs[10][10], used[10][10];
    available.clear();
    if (start_of_game && (desk[0][9] == EMPTY || desk[9][0] == EMPTY))
    {
        trans_bfs(player, used);
        if (player == 0)
        {
            if (viruses[0].size() == 2)
            {
                if (desk[0][1] == EMPTY)
                    available.push_back(make_pair(0, 1));
                if (desk[1][0] == EMPTY)
                    available.push_back(make_pair(1, 0));
            }
            else
            {
                for (i = 0; i < 10; i++)
                    if (desk[i][0] == EMPTY)
                    {
                        available.push_back(make_pair(i, 0));
                        break;
                    }
                for (i = 0; i < 10; i++)
                    if (desk[0][i] == EMPTY)
                    {
                        if (available.size())
                        {

                            if (used[available[0].first][available[0].second] > used[0][i] || (used[available[0].first][available[0].second] == used[0][i] && i < available[0].first))
                            {
                                available.erase(--available.end());
                                available.push_back(make_pair(0, i));
                            }
                        }
                        else
                            available.push_back(make_pair(0, i));
                        break;
                    }
            }
        }
        if (player == 1)
        {
            for (i = 9; i >= 0; i--)
                if (desk[i][9] == EMPTY)
                {
                    available.push_back(make_pair(i, 9));
                    break;
                }
            for (i = 9; i >= 0; i--)
                if (desk[9][i] == EMPTY)
                {
                    if (available.size())
                    {

                        if (used[available[0].first][available[0].second] > used[9][i] || (used[available[0].first][available[0].second] == used[9][i] && i < available[0].first))
                        {
                            available.erase(--available.end());
                            available.push_back(make_pair(9, i));
                        }
                    }
                    else
                        available.push_back(make_pair(9, i));
                    break;
                }
        }
        random_shuffle(available.begin(), available.end());
    }
    else
    {
        if (viruses[player].size() == 0)
        {
            if (player == 0)
                available.push_back(make_pair(0, 0));
            if (player == 1)
                available.push_back(make_pair(9, 9));
        }

        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
            {
                used[i][j] = 0;
                used_dfs[i][j] = 0;
            }
        set < pair < int, int > > :: iterator k;
        for (k = viruses[player].begin(); k != viruses[player].end(); k++)
        {
            i = (*k).first;
            j = (*k).second;
            for (dx = -1; dx <= 1; dx++)
                for (dy = -1; dy <= 1; dy++)
                    if (in_desk(i + dx, j + dy) && desk[i + dx][j + dy] == EMPTY)
                        add_to_available(available, used, i + dx, j + dy);
            dfs(i, j, available, player, used_dfs, used);
        }
        random_shuffle(available.begin(), available.end());
        cut_available(player, available);
    }
}

void turn_to(int player, int x, int y)
{
    if (desk[x][y] == EMPTY)
    {
        viruses[player].insert(make_pair(x, y));
        desk[x][y] = player;
    }
    if (desk[x][y] == !player)
    {
        viruses[!player].erase(make_pair(x, y));
        desk[x][y] = !player + 2;
    }
}

void turn_out(int player, int x, int y)
{
    if (desk[x][y] == player)
    {
        viruses[player].erase(make_pair(x, y));
        desk[x][y] = EMPTY;
    }
    if (desk[x][y] == !player + 2)
    {
        viruses[!player].insert(make_pair(x, y));
        desk[x][y] = !player;
    }
}

void inc_func(int player, int &func, int a)
{
    if (player == 1)
        func += a;
    if (player == 0)
        func -= a;
}

void dec_func(int player, int &func, int a)
{
    if (player == 1)
        func -= a;
    if (player == 0)
        func += a;
}

int f(int player)
{
    int ans = 0;
    int dist_player = bfs(player);
    int dist_another_player = bfs(!player);

    int chains_player = count_chains(player);
    int chains_another_player = count_chains(!player);


    if (viruses[!player].size() == 0)
    {
        inc_func(player, ans, INF - 5);
        return ans;
    }
    if (viruses[player].size() == 0)
    {
        dec_func(player, ans, INF - 5);
        return ans;
    }
    inc_func(player, ans, (desk[0][9] == player + desk[9][0] == player) * 7);
    dec_func(player, ans, (desk[0][9] == !player + desk[9][0] == !player) * 7);
    if (chains_player > 1)
        inc_func(player, ans, chains_player * 8);
    if (chains_another_player > 1)
        dec_func(player, ans, chains_another_player * 8);
    if (dist_player <= 2)
        inc_func(player, ans, 7);
    if (dist_player < 3 && dist_another_player >= 3)
        inc_func(player, ans, 25 * dist_another_player);
    if (dist_another_player == 1 && dist_player >= 3)
        dec_func(player, ans, 30 * dist_player);
    if (start_of_game && dist_player <= 3)
    {
        if (dist_player == 1)
            inc_func(player, ans, 60);
        if (dist_player == 2)
            inc_func(player, ans, 40);
    }

    return ans;
}

void dls(int player, int depth)
{
    vector < pair < int, int > > available1, available2, available3;
    pair < int, int > turn1, turn2, turn3;
    int optimal_func, can_make_three_turns = 0;
    if (player == 0)
        optimal_func = INF;
    if (player == 1)
        optimal_func = -INF;
    find_available(player, available1);
    for (int i = 0; i < available1.size(); i++)
    {
        turn1 = available1[i];
        turn_to(player, turn1.first, turn1.second);
        find_available(player, available2);
        for (int j = 0; j < available2.size(); j++)
        {
            turn2 = available2[j];
            turn_to(player, turn2.first, turn2.second);
            find_available(player, available3);
            for (int k = 0; k < available3.size(); k++)
            {
                turn3 = available3[k];
                turn_to(player, turn3.first, turn3.second);
                can_make_three_turns = 1;
                int buf;
                if (depth == 0)
                {
                    if (my_player == 0)
                    {
                        if ((buf = f(!player)) < optimal_func)
                        {
                            optimal_turn[0] = turn1;
                            optimal_turn[1] = turn2;
                            optimal_turn[2] = turn3;
                            optimal_func = buf;
                        }
                    }
                    if (my_player == 1)
                    {
                        if ((buf = f(!player)) > optimal_func)
                        {
                            optimal_turn[0] = turn1;
                            optimal_turn[1] = turn2;
                            optimal_turn[2] = turn3;
                            optimal_func = buf;
                        }
                    }
                }
                if (depth)
                    dls(!player, depth - 1);
                turn_out(player, turn3.first, turn3.second);
            }
            turn_out(player, turn2.first, turn2.second);
        }
        turn_out(player, turn1.first, turn1.second);
    }
    if (!can_make_three_turns)
    {
        optimal_turn[0].first = -1;
        optimal_turn[1].first = -1;
        optimal_turn[2].first = -1;
        find_available(player, available1);
        if (available1.size())
            optimal_turn[0] = available1[0];
        turn_to(player, optimal_turn[0].first, optimal_turn[0].second);
        find_available(player, available2);
        if (available2.size())
            optimal_turn[1] = available2[0];
        turn_to(player, optimal_turn[1].first, optimal_turn[1].second);
        optimal_func = f(player);
        turn_out(player, optimal_turn[1].first, optimal_turn[1].second);
        turn_out(player, optimal_turn[0].first, optimal_turn[0].second);
    }
}

void make_first_turn(int player)
{
    if (player == 0)
    {
        optimal_turn[0] = make_pair(0, 0);
        optimal_turn[1] = make_pair(1, 1);
        optimal_turn[2] = make_pair(2, 2);
    }
    if (player == 1)
    {
        optimal_turn[0] = make_pair(9, 9);
        optimal_turn[1] = make_pair(8, 8);
        optimal_turn[2] = make_pair(7, 7);
    }
}

void make_turn(int player, int depth, int turn)
{
    int i;

    if (start_of_game)
        if (bfs(0) <= 3 || bfs(1) <= 3)
            start_of_game = 0;
    for (i = 0; i <= depth; i++)
        dls(player, depth);
    for (i = 0; i < 3; i++)
        if (optimal_turn[i].first != -1)
            turn_to(player, optimal_turn[i].first, optimal_turn[i].second);
//    print_desk();
//    print_dist(player);
//    cout << endl << "dist " << bfs(0) << " " << bfs(1) << endl;
//    cout << "chains " << count_chains(0) << " " << count_chains(1) << endl;
//    cout << "function " << f(!player) << endl;
//    cout << "flag" << start_of_game << endl;
    for (i = 0; i < 3; i++)
        if (optimal_turn[i].first != -1)
            cout << optimal_turn[i].first << optimal_turn[i].second;
    cout << endl;
}

void enemy_turn(int my_player)
{
    char x, y;
    for (int i = 0; i < 3; i++)
    {
        cin >> x >> y;
        turn_to(!my_player, x - '0', y - '0');
    }
}

int main()
{
//    freopen("intput.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);

    int i = 0, j = 0, k, turn = 0;
    init();
    cin >> my_player;

    if (my_player == 2)
        enemy_turn(my_player);
    --my_player;
    while (true)
    {
        make_turn(my_player, 0, ++turn);
        enemy_turn(my_player);
    }
    return 0;
}
