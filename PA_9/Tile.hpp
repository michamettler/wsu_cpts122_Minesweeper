#pragma once

#include <vector>

using std::vector;

class Tile {
public:
	Tile() { revealed = false; flagged = false; symbol = '#'; }

	virtual bool hasMine() { return false; }
	virtual int reveal(vector<Tile*> neighbors);

	bool isRevealed() const { return revealed; }
	bool isFlagged() const { return flagged; }
	char getSymbol() const { return (flagged) ? 'F' : symbol; }
	void plantFlag() { flagged = !flagged; revealed = flagged; }

protected:
	bool revealed;
	bool flagged;
	char symbol;
};
