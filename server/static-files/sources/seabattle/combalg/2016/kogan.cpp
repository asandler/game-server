//#include "stdafx.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <vector>
#include <utility>

using std::cin;
using std::cout;

typedef long double ld;

//std::ifstream cin("in");
//std::ofstream cout("out");

class Cell {
private:
	int x, y;

public:
	Cell(int x, int y) : x(x), y(y) {}
	Cell() : x(-1), y(-1) {}

	int GetX() {
		return x;
	}

	int GetY() {
		return y;
	}
};

bool operator< (Cell a, Cell b) {
	return a.GetX() < b.GetX() || (a.GetX() == b.GetX() && a.GetY() < b.GetY());
}

bool operator> (Cell a, Cell b) {
	return b < a;
}

bool operator<= (Cell a, Cell b) {
	return !(a > b);
}

bool operator>= (Cell a, Cell b) {
	return b <= a;
}

std::istream& operator>> (std::istream& in, Cell& p) {
	char a, b;
	in >> a >> b;
	p = Cell(a - 'A', b - '0');
	return in;
}

std::ostream& operator<< (std::ostream& out, Cell p) {
	return out << (char)('A' + p.GetX()) << p.GetY();
}

typedef long double ld;
typedef std::pair<Cell, Cell> Ship;

const int MAXNUMTURNS = 157;
const int FIELDSIZE = 10;
const int NUMSHIPS = 10;

int field[FIELDSIZE][FIELDSIZE];
int enemyField[FIELDSIZE][FIELDSIZE];

void Validate(int& c) {
	c = std::max(0, c);
	c = std::min(FIELDSIZE - 1, c);
}

bool IsValid(int c) {
	int tmp = c;
	Validate(c);
	return c == tmp;
}

bool IsValid(Cell p) {
	return IsValid(p.GetX()) && IsValid(p.GetY());
}

bool canShoot = true;
Cell enemyFrom;
Cell enemyShot;
bool damagedThisTurn = true;

bool called[FIELDSIZE][FIELDSIZE];

Cell SelectFrom() {
	for (int x = 0; x < FIELDSIZE; ++x)
		for (int y = 0; y < FIELDSIZE; ++y)
			if (field[x][y] == 1 && called[x][y])
				return Cell(x, y);
	while (true) {
		int x = rand() % FIELDSIZE;
		int y = rand() % FIELDSIZE;
		if (field[x][y] == 1)
			return Cell(x, y);
	}
}

bool IsGreatShot(int x, int y) {
	if (enemyField[x][y]) return false;
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			if (!IsValid(Cell(x + i, y + j))) continue;
			if (i && j && enemyField[x + i][y + j] == 2) return false;
			if (enemyField[x + i][y + j] == 3) return false;
		}
	}
	return true;
}

Cell SelectVictim() {
	while (true) {
		int x = rand() % FIELDSIZE;
		int y = rand() % FIELDSIZE;
		if (IsGreatShot(x, y))
			return Cell(x, y);
	}
}

void MarkAsKilled(int x, int y) {
	if (!IsValid(x) || !IsValid(y) || enemyField[x][y] != 2)
		return;
	enemyField[x][y] = 3;
	MarkAsKilled(x - 1, y);
	MarkAsKilled(x + 1, y);
	MarkAsKilled(x, y - 1);
	MarkAsKilled(x, y + 1);
}

void UpdateEnemyField(Cell shot, int answer) {
	if (answer == 2)
		MarkAsKilled(shot.GetX(), shot.GetY());
	enemyField[shot.GetX()][shot.GetY()] = answer + 1;
}

void UpdateEnemyFieldByFrom(Cell from) {}

void UpdateEnemyShot(Cell shot) {
	int x = shot.GetX();
	int y = shot.GetY();
	if (field[x][y] == 0)
		field[x][y] = 3;
	if (field[x][y] == 1)
		field[x][y] = 2;
}

void UpdateOurFrom(Cell from) {}

std::vector<Ship> ships;

bool InRange(int a, int b, int c) {
	return a <= b && b <= c;
}

bool IsInShip(Cell p, Ship s) {
	return InRange(s.first.GetX(), p.GetX(), s.second.GetX())
		&& InRange(s.first.GetY(), p.GetY(), s.second.GetY());
}

int ShipNum(Cell p) {
	for (int i = 0; i < NUMSHIPS; ++i)
		if (IsInShip(p, ships[i]))
			return i;
	return -1;
}

bool IsFree(int x, int y) {
	if (!IsValid(Cell(x, y))) return true;
	return field[x][y] < 1 || field[x][y] > 2;
}

bool CanMoveCellRight(int x, int y) {
	Cell q(x + 1, y);
	if (!IsValid(q)) return false;
	if (field[x][y] == 2) return false;
	if (field[x + 1][y] == 3) return false;
	for (int j = -1; j < 2; ++j)
		if (!IsFree(x + 2, y + j))
			return false;
	return true;
}

bool CanMoveCellLeft(int x, int y) {
	Cell q(x - 1, y);
	if (!IsValid(q)) return false;
	if (field[x][y] == 2) return false;
	if (field[x - 1][y] == 3) return false;
	for (int j = -1; j < 2; ++j)
		if (!IsFree(x - 2, y + j))
			return false;
	return true;
}

bool CanMoveCellUp(int x, int y) {
	Cell q(x, y + 1);
	if (!IsValid(q)) return false;
	if (field[x][y] == 2) return false;
	if (field[x][y + 1] == 3) return false;
	for (int j = -1; j < 2; ++j)
		if (!IsFree(x + j, y + 2))
			return false;
	return true;
}

bool CanMoveCellDown(int x, int y) {
	Cell q(x, y - 1);
	if (!IsValid(q)) return false;
	if (field[x][y] == 2) return false;
	if (field[x][y - 1] == 3) return false;
	for (int j = -1; j < 2; ++j)
		if (!IsFree(x + j, y - 2))
			return false;
	return true;
}

bool IsDamaged(Ship s) {
	for (int x = s.first.GetX(); x <= s.second.GetX(); ++x)
		for (int y = s.first.GetY(); y <= s.second.GetY(); ++y)
			if (field[x][y] == 2)
				return true;
	return false;
}

void ApplyRightMovement(Ship& s) {
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int y = y1; y <= y2; ++y) {
		field[x1][y] = 0;
		field[x2 + 1][y] = 1;
	}
	s.first = Cell(x1 + 1, y1);
	s.second = Cell(x2 + 1, y2);
}

void ApplyLeftMovement(Ship& s) {
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int y = y1; y <= y2; ++y) {
		field[x2][y] = 0;
		field[x1 - 1][y] = 1;
	}
	s.first = Cell(x1 - 1, y1);
	s.second = Cell(x2 - 1, y2);
}

void ApplyUpMovement(Ship& s) {
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int x = x1; x <= x2; ++x) {
		field[x][y1] = 0;
		field[x][y2 + 1] = 1;
	}
	s.first = Cell(x1, y1 + 1);
	s.second = Cell(x2, y2 + 1);
}

void ApplyDownMovement(Ship& s) {
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int x = x1; x <= x2; ++x) {
		field[x][y2] = 0;
		field[x][y1 - 1] = 1;
	}
	s.first = Cell(x1, y1 - 1);
	s.second = Cell(x2, y2 - 1);
}

bool MoveShipRight(Ship &s) {
	if (IsDamaged(s)) return false;
	int x = s.second.GetX();
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	for (int y = y1; y <= y2; ++y)
		if (!CanMoveCellRight(x, y))
			return false;
	ApplyRightMovement(s);
	return true;
}

bool MoveShipLeft(Ship &s) {
	if (IsDamaged(s)) return false;
	int x = s.first.GetX();
	int y1 = s.first.GetY();
	int y2 = s.second.GetY();
	for (int y = y1; y <= y2; ++y)
		if (!CanMoveCellLeft(x, y))
			return false;
	ApplyLeftMovement(s);
	return true;
}

bool MoveShipUp(Ship &s) {
	if (IsDamaged(s)) return false;
	int y = s.second.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int x = x1; x <= x2; ++x)
		if (!CanMoveCellUp(x, y))
			return false;
	ApplyUpMovement(s);
	return true;
}

bool MoveShipDown(Ship &s) {
	if (IsDamaged(s)) return false;
	int y = s.first.GetY();
	int x1 = s.first.GetX();
	int x2 = s.second.GetX();
	for (int x = x1; x <= x2; ++x)
		if (!CanMoveCellDown(x, y))
			return false;
	ApplyDownMovement(s);
	return true;
}

void PlaceShip() {
	Cell begin, end;
	cin >> begin >> end;
	if (begin > end) std::swap(begin, end);
	ships.push_back(std::make_pair(begin, end));
	for (int i = begin.GetX(); i <= end.GetX(); ++i)
		for (int j = begin.GetY(); j <= end.GetY(); ++j)
				field[i][j] = 1;
}

void PlaceShips() {
	for (int shipsPlaced = 0; shipsPlaced < NUMSHIPS; ++shipsPlaced)
		PlaceShip();
}

void MoveShip(Cell shot) {
	int i = ShipNum(shot);
	if (i == -1) {
		cout << "0X";
		return;
	}
	int x = shot.GetX();
	int y = shot.GetY();
	if (IsFree(x - 1, y) && MoveShipRight(ships[i])) {
		cout << i << "E";
		return;
	}
	if (IsFree(x + 1, y) && MoveShipLeft(ships[i])) {
		cout << i << "W";
		return;
	}
	if (IsFree(x, y - 1) && MoveShipUp(ships[i])) {
		cout << i << "N";
		return;
	}
	if (IsFree(x, y + 1) && MoveShipDown(ships[i])) {
		cout << i << "S";
		return;
	}
	cout << "0X";
}

bool movedShip = false;
Cell from;

void Shoot() {
	if (!movedShip) {
		MoveShip(from);
		cout << ' ';
	} else {
		movedShip = false;
	}
	from = SelectFrom();
	UpdateOurFrom(from);
	Cell to = SelectVictim();
	cout << from << to << '\n';
	std::string answer;
	cin >> answer;
	if (answer.size() == 1) {
		UpdateEnemyField(to, (answer == "K") + 1);
		return;
	}
	enemyFrom = Cell(answer[0] - 'A', answer[1] - '0');
	enemyShot = Cell(answer[2] - 'A', answer[3] - '0');
	canShoot = false;
	UpdateEnemyField(to, 0);
}

bool used[FIELDSIZE][FIELDSIZE];

bool DFS(int x, int y) {
	if (!IsValid(x) || !IsValid(y) || used[x][y] || !field[x][y] || field[x][y] == 3) return true;
	used[x][y] = true;
	if (field[x][y] == 1) return false;
	return DFS(x - 1, y) && DFS(x + 1, y) && DFS(x, y - 1) && DFS(x, y + 1);
}

bool IsKilled(Cell shot) {
	for (int i = 0; i < FIELDSIZE; ++i)
		for (int j = 0; j < FIELDSIZE; ++j)
			used[i][j] = 0;
	return DFS(shot.GetX(), shot.GetY());
}

void GetShot() {
	movedShip = false;
	if (!canShoot)
		canShoot = true;
	else
		cin >> enemyFrom >> enemyShot;
	UpdateEnemyFieldByFrom(enemyFrom);
	if (field[enemyShot.GetX()][enemyShot.GetY()] == 1) {
		MoveShip(enemyShot);
		cout << ' ';
		movedShip = true;
	} else {
		movedShip = false;
	}
	if (field[enemyShot.GetX()][enemyShot.GetY()] == 1) {
		UpdateEnemyShot(enemyShot);
		cout << (IsKilled(enemyShot) ? 'K' : 'H') << '\n';
	} else {
		damagedThisTurn = false;
		UpdateEnemyShot(enemyShot);
	}
}

int main() {
	srand(time(0));
	PlaceShips();
	bool isSecond;
	cin >> isSecond;
	if (isSecond)
		while (damagedThisTurn)
			GetShot();
	for (int i = 0; i < MAXNUMTURNS; ++i) {
		canShoot = true;
		while (canShoot)
			Shoot();
		damagedThisTurn = true;
		while (damagedThisTurn)
			GetShot();
	}
	std::ofstream out("out");
	for (int i = 0; i < FIELDSIZE; ++i) {
		for (int j = 0; j < FIELDSIZE; ++j)
			out << enemyField[i][j];
		out << '\n';
	}
}
