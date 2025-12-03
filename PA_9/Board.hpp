#pragma once

#include "Tile.hpp"
#include "Mine.hpp"
#include <vector>
#include <cmath>

using std::vector;

class Board {
public:
    Board(int newSize = 5, int newMines = 6);

    int getSize() { return size; }
    int getMines() { return mines; }
    int getCursorX() { return cursor_x; }
    int getCursorY() { return cursor_y; }

    void moveCursor(int move_x, int move_y);
    void plantFlag();
    bool revealTile();
    bool checkForWin();

    Tile* getTile(int x, int y);

private:
    int size;
    int mines;
    int cursor_x;
    int cursor_y;
    vector<Tile*> tiles;

    bool revealTile(Tile* pCenter);
    Tile** getTileAt(int x, int y);
};
