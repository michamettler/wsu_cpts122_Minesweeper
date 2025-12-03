#include "Tile.hpp"

int Tile::reveal(vector<Tile*> neighbors) {
    if (revealed) return true;
    revealed = true;

    int adjacentMines = 0;
    for (Tile* x : neighbors) {
        if (x->hasMine()) adjacentMines++;
    }

    if (adjacentMines == 0) symbol = ' ';
    else symbol = adjacentMines + 48;    // int -> char

    return adjacentMines;
}
