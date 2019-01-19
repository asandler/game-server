#include <iostream>
#include <string>
#include <vector>

#define EMPTY   0
#define DECK    1
#define FIRE    2
#define MINE    3
#define OUT     4
#define HIDDEN  10

using namespace std;

struct Coord {
    Coord(int r, int c)
        : Row(r)
        , Col(c)
    {}

    int Row;
    int Col;
};

Coord StringToCoord(const string& s) {
    return Coord(s[0] - 'A', s[1] - '0');
}

string CoordToString(const Coord& c) {
    string answer = "";
    answer.push_back((char)(c.Row + 'A'));
    answer.push_back((char)(c.Col + '0'));
    return answer;
}

class Player {
public:
    Player(const string& initPos)
        //: N({4, 10, 1})
    {
        for (size_t i = 0; i < 10; ++i) {
            Field.push_back(vector<int>(10, 0));
        }
        for (size_t i = 0; i < 10; ++i) {
            Ships.push_back(Ship(initPos.substr(5 * i, 4)));
        }
        for (size_t i = 0; i < 10; ++i) {
            for (size_t j = 0; j < Ships[i].Decks.size(); ++j) {
                Field[Ships[i].Decks[j].first.Row][Ships[i].Decks[j].first.Col] = DECK;
            }
        }
    }

    string MoveShip(Coord shootCoord) {
        vector<pair<double, vector<double> > > mobility = GetShipsMobility(shootCoord);
        int movingShipIndex = -1, maxMoveIndex = -1, shipInDangerIndex = -1;
        string moves = "NSWE";

        if (shootCoord.Row != -1 && shootCoord.Col != -1) {
            if (Field[shootCoord.Row][shootCoord.Col] == DECK) {
                shipInDangerIndex = GetShipIndexByCoord(shootCoord);
                if (mobility[shipInDangerIndex].first > -1) {
                    double maxMobility = -0.3;
                    for (size_t i = 0; i < 4; ++i) {
                        if (mobility[shipInDangerIndex].second[i] > maxMobility) {
                            movingShipIndex = shipInDangerIndex;
                            maxMobility = mobility[shipInDangerIndex].second[i];
                            maxMoveIndex = i;
                        }
                    }
                }
            }
        }

        if (movingShipIndex != -1) {
            for (size_t i = 0; i < Ships[movingShipIndex].Decks.size(); ++i) {
                Field[Ships[movingShipIndex].Decks[i].first.Row][Ships[movingShipIndex].Decks[i].first.Col] = EMPTY;
            }
            MoveShipDirection(Ships[movingShipIndex], moves[maxMoveIndex]);
            for (size_t i = 0; i < Ships[movingShipIndex].Decks.size(); ++i) {
                Field[Ships[movingShipIndex].Decks[i].first.Row][Ships[movingShipIndex].Decks[i].first.Col] = DECK;
            }
            Field[shootCoord.Row][shootCoord.Col] = MINE;

            string answer = "";
            answer.push_back((char)(movingShipIndex + '0'));
            answer.push_back(moves[maxMoveIndex]);
            return answer + " ";

        } else {
            string wound = "";

            if (shipInDangerIndex != -1) {
                wound = WoundShip(Ships[shipInDangerIndex], shootCoord);
                Field[shootCoord.Row][shootCoord.Col] = FIRE;
            } else {
                if (shootCoord.Row != -1 && shootCoord.Col != -1) {
                    Field[shootCoord.Row][shootCoord.Col] = MINE;
                }
            }

            return "0X " + wound;
        }

        return "0X ";
    }

    string ChooseShipToShot() {
        vector<pair<double, vector<double> > > mobility = GetShipsMobility(Coord(-1, -1));
        double maxMobility = -1;
        string answer = "";

        for (size_t i = 0; i < mobility.size(); ++i) {
            if (mobility[i].first > maxMobility) {
                maxMobility = mobility[i].first;
                for (size_t j = 0; j < Ships[i].Decks.size(); ++j) {
                    if (Ships[i].Decks[j].second) {
                        answer = CoordToString(Ships[i].Decks[j].first);
                    }
                }
            }
        }

        return answer;
    }

    void UpdateOppField(const string& from) {
        PrevOppShoots.push_back(StringToCoord(from));
    }

    string RandomShot() {
        int r = rand() % 10;
        int c = rand() % 10;
        string answer = "";
        answer += (char)('A' + r);
        answer += (char)('0' + c);
        return answer;
    }

    string MakeShot() {
        if (PrevOppShoots.size() == 0) {
            return RandomShot();
        } else {
            string answer = CoordToString(PrevOppShoots.back());
            PrevOppShoots.pop_back();
            return answer;
        }
    }

    void DumpField() {
        cout << "   0 1 2 3 4 5 6 7 8 9" << endl;
        cout << "______________________" << endl;
        for (size_t i = 0; i < 10; ++i) {
            cout << (char)('A' + i) << "| ";
            for (size_t j = 0; j < 10; ++j) {
                cout << Field[i][j] << " ";
            }
            cout << endl;
        }
    }

    void DumpMobility() {
        for (size_t i = 0; i < 10; ++i) {
            cout << i << " " << Ships[i].Init << ": " << ShipMobility(Ships[i], Coord(-1, -1), false) << endl;
        }
    }

private:
    struct Ship {
        Ship(const string& s)
            : Init(s)
        {
            Coord c1 = StringToCoord(s.substr(0, 2));
            Coord c2 = StringToCoord(s.substr(2, 2));

            for (int r = min(c1.Row, c2.Row); r <= max(c1.Row, c2.Row); ++r) {
                for (int c = min(c1.Col, c2.Col); c <= max(c1.Col, c2.Col); ++c) {
                    Decks.push_back(make_pair(Coord(r, c), true));
                }
            }
        }

        Ship(const Ship& s, pair<int, int> dir) {
            for (size_t i = 0; i < s.Decks.size(); ++i) {
                Decks.push_back(make_pair(Coord(s.Decks[i].first.Row + dir.first, s.Decks[i].first.Col + dir.second), s.Decks[i].second));
            }
            Init = "";
            Init.push_back((char)('A' + Decks[0].first.Row));
            Init.push_back((char)('0' + Decks[0].first.Col));
            Init.push_back((char)('A' + Decks[Decks.size() - 1].first.Row));
            Init.push_back((char)('0' + Decks[Decks.size() - 1].first.Col));
        }

        Ship(const Ship& s)
            : Decks(s.Decks)
            , Init(s.Init)
        {
        }

        vector<pair<Coord, bool> > Decks;
        string Init;
    };

    int GetShipIndexByCoord(Coord c) {
        for (size_t i = 0; i < Ships.size(); ++i) {
            for (size_t j = 0; j < Ships[i].Decks.size(); ++j) {
                if (Ships[i].Decks[j].first.Row == c.Row && Ships[i].Decks[j].first.Col == c.Col) {
                    return i;
                }
            }
        }
        return -1;
    }

    bool DeckAround(const int row, const int col) {
        for (int i = max(row - 1, 0); i <= min(row + 1, 9); ++i) {
            for (int j = max(col - 1, 0); j <= min(col + 1, 9); ++j) {
                if (Field[i][j] == DECK || Field[i][j] == FIRE) {
                    return true;
                }
            }
        }
        return false;
    }

    int CheckFieldForShip(const Ship& s, Coord shootCoord) {
        for (size_t i = 0; i < s.Decks.size(); ++i) {
            if (!s.Decks[i].second) {
                return FIRE;
            }
            if (s.Decks[i].first.Row < 0 || s.Decks[i].first.Row > 9) {
                return OUT;
            }
            if (s.Decks[i].first.Col < 0 || s.Decks[i].first.Col > 9) {
                return OUT;
            }
            if (s.Decks[i].first.Row == shootCoord.Row && s.Decks[i].first.Col == shootCoord.Col) {
                return MINE + HIDDEN;
            }
            if (Field[s.Decks[i].first.Row][s.Decks[i].first.Col] == MINE) {
                return MINE;
            }
            if (Field[s.Decks[i].first.Row][s.Decks[i].first.Col] == MINE + HIDDEN) {
                return MINE;
            }
            if (DeckAround(s.Decks[i].first.Row, s.Decks[i].first.Col)) {
                return DECK;
            }
        }
        return EMPTY;
    }

    void HideShip(const Ship& ship) {
        for (size_t i = 0; i < ship.Decks.size(); ++i) {
            Field[ship.Decks[i].first.Row][ship.Decks[i].first.Col] += HIDDEN;
        }
    }

    void UnhideShip(const Ship& ship) {
        for (size_t i = 0; i < ship.Decks.size(); ++i) {
            Field[ship.Decks[i].first.Row][ship.Decks[i].first.Col] -= HIDDEN;
        }
    }

    void MoveShipDirection(Ship& ship, const char direction) {
        for (size_t i = 0; i < ship.Decks.size(); ++i) {
            switch (direction) {
                case 'N':
                    ship.Decks[i].first.Col++;
                    break;
                case 'S':
                    ship.Decks[i].first.Col--;
                    break;
                case 'W':
                    ship.Decks[i].first.Row--;
                    break;
                case 'E':
                    ship.Decks[i].first.Row++;
                    break;
            }
        }
    }

    string WoundShip(Ship& ship, Coord shootCoord) {
        bool has_alive_decks = false;

        for (size_t i = 0; i < ship.Decks.size(); ++i) {
            if (ship.Decks[i].first.Row == shootCoord.Row && ship.Decks[i].first.Col == shootCoord.Col) {
                ship.Decks[i].second = false;
            }
            has_alive_decks |= ship.Decks[i].second;
        }
        if (has_alive_decks) {
            return "H";
        } else {
            return "K";
        }
    }

    double ShipMobility(const Ship& ship, Coord shootCoord, bool phantomShip) {
        if (CheckFieldForShip(ship, Coord(-1, -1)) == OUT) {
            return -1;
        }

        HideShip(ship);

        if (phantomShip) {
            if (CheckFieldForShip(ship, shootCoord) != EMPTY) {
                UnhideShip(ship);
                return -1;
            }
        } else {
            if (CheckFieldForShip(ship, Coord(-1, -1)) != EMPTY) {
                UnhideShip(ship);
                return -1;
            }
        }

        size_t ans = 0;
        Ship n(ship, make_pair(0, 1));  // move north
        Ship s(ship, make_pair(0, -1)); // move south
        Ship w(ship, make_pair(-1, 0)); // move west
        Ship e(ship, make_pair(1, 0));  // move east

        if (CheckFieldForShip(n, shootCoord) == EMPTY) {
            ans++;
        }
        if (CheckFieldForShip(s, shootCoord) == EMPTY) {
            ans++;
        }
        if (CheckFieldForShip(w, shootCoord) == EMPTY) {
            ans++;
        }
        if (CheckFieldForShip(e, shootCoord) == EMPTY) {
            ans++;
        }

        UnhideShip(ship);

        if (ans == 0) {
            for (size_t i = 0; i < ship.Decks.size(); ++i) {
                if (ship.Decks[i].first.Row == shootCoord.Row && ship.Decks[i].first.Col == shootCoord.Col) {
                    if (!phantomShip) {
                        return -1;
                    } else {
                        return -0.5;
                    }
                }
            }
        }

        return (double)ans / ship.Decks.size();
    }

    bool ShipIsWounded(const int shipIndex) {
        if (shipIndex == -1) {
            return false;
        }
        for (size_t i = 0; i < Ships[shipIndex].Decks.size(); ++i) {
            if (!Ships[shipIndex].Decks[i].second) {
                return true;
            }
        }
        return false;
    }

    vector<pair<double, vector<double> > > GetShipsMobility(Coord shootCoord) {
        vector<pair<double, vector<double> > > answer;

        for (size_t i = 0; i < Ships.size(); ++i) {
            if (!ShipIsWounded(i)) {
                answer.push_back(make_pair(ShipMobility(Ships[i], shootCoord, false), vector<double>(4, 0)));

                HideShip(Ships[i]);

                Ship n(Ships[i], make_pair(0, 1));  // move north
                Ship s(Ships[i], make_pair(0, -1)); // move south
                Ship w(Ships[i], make_pair(-1, 0)); // move west
                Ship e(Ships[i], make_pair(1, 0));  // move east

                answer[i].second[0] = ShipMobility(n, shootCoord, true);
                answer[i].second[1] = ShipMobility(s, shootCoord, true);
                answer[i].second[2] = ShipMobility(w, shootCoord, true);
                answer[i].second[3] = ShipMobility(e, shootCoord, true);

                UnhideShip(Ships[i]);
            } else {
                answer.push_back(make_pair(-1, vector<double>(4, -1)));
            }
        }
        return answer;
    }

    vector<Coord> PrevOppShoots;
    vector<vector<double> > PrevOppShipPositions;
    vector<vector<int> > Field;

    vector<Ship> Ships;

    //NeuralNet N;
};

int main() {
    string input, output;
    getline(cin, input);

    Player P(input);

    int turn;
    cin >> turn;
    if (turn == 0) {
        output = P.MoveShip(Coord(-1, -1));
        output += P.ChooseShipToShot();
        output += P.MakeShot();
        cout << output << endl;
    }
    while (true) {
        cin >> input;
        if (input == "H" || input == "K") {
            output = P.MoveShip(Coord(-1, -1));
        } else {
            P.UpdateOppField(input.substr(0, 2));
            output = P.MoveShip(StringToCoord(input.substr(2, 2)));
        }
        if (output != "0X H" && output != "0X K") {
            output += P.ChooseShipToShot();
            output += P.MakeShot();
        }
        cout << output << endl;
        //P.DumpField();
    }

    return 0;
}
