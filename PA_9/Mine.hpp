#pragma once
#include "Tile.hpp"
class Mine : public Tile {
public:
	Mine(Tile* oldTile) : Tile() { symbol = 'M'; oldTile = this; }
	bool hasMine() const override { return true; }
	int reveal(vector<Tile*> neighbors) override;
};