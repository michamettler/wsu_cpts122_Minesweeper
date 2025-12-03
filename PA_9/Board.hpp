#pragma once

#include "Tile.hpp"
#include "Mine.hpp"
#include <vector>
#include <iostream>
#include <cmath>

using std::vector;
using std::cout;
using std::endl;

class Board {
public:
	Board(int newSize = 5, int newMines = 6);
	
	int getSize() { return size; }
	int getMines() { return mines; }
	int getCursorX() { return cursor_x; }
	int getCursorY() { return cursor_y; }

	void print(bool withFog);
	void moveCursor(int move_x, int move_y);
	void plantFlag();
	bool revealTile();
	bool checkForWin();

private:
	int size;
	int mines;
	int cursor_x;
	int cursor_y;
	vector<Tile*> tiles;

	void printLine(bool withFog, int line);
	bool revealTile(Tile* pCenter);
	Tile** getTileAt(int x, int y);
};
