#include "Board.hpp"

Board::Board(int newSize, int newMines) {
	size = newSize;
	mines = newMines;
	cursor_x = cursor_y = 0;

	// Fille tiles
	for (int i = 0; i < (size * size); i++) {
		tiles.push_back(new Tile());
	}

	// Replace some tiles with mines
	int currentMines = 0;
	int x, y;
	Tile** pCur;
	while (currentMines < mines) {
		x = rand() % size;
		y = rand() % size;
		pCur = getTileAt(x, y);

		if (!(*pCur)->hasMine()) {
			*pCur = new Mine(*pCur);
			currentMines++;
		}
	}

}

void Board::print(bool withFog) {
	for (int i = 0; i < cursor_x; i++) cout << "  ";
	if (withFog) cout << "  v";		// Top cursor indicator
	cout << endl;

	for (int i = 0; i < size; i++) {

		// Left cursor indicator
		if (i == cursor_y && withFog) {
			cout << ">";
		} else cout << " ";

		printLine(withFog, i);

		// Right cursor indicator
		if (i == cursor_y && withFog) {
			cout << " <";
		}

		cout << endl;
	}

	// Places regular console cursor instead of a bottom cursor indicator
	if (!withFog) return;
	for (int i = 0; i < cursor_x; i++) cout << "  ";
	cout << "  ";

}

// Pulled this out of print() function for code neatness
void Board::printLine(bool withFog, int line) {
	Tile* pCur;

	for (int j = 0; j < size; j++) {
		pCur = tiles.at((line * size) + j);
		if (!withFog && pCur->isFlagged()) pCur->plantFlag();

		if (!withFog) {
			cout << " " << pCur->getSymbol();
		}
		else {
			if (pCur && !pCur->isRevealed()) {
				cout << " #";
			}
			else if (pCur && pCur->isRevealed()) {
				cout << " " << pCur->getSymbol();
			}
		}
	}
}

bool Board::revealTile() {
	return revealTile(*getTileAt(cursor_x, cursor_y));
}

void Board::moveCursor(int move_x, int move_y) {
	cursor_x = cursor_x + move_x;
	cursor_x = (int)fmin(cursor_x, size - 1);
	cursor_x = (int)fmax(cursor_x, 0);

	cursor_y = cursor_y + move_y;
	cursor_y = (int)fmin(cursor_y, size - 1);
	cursor_y = (int)fmax(cursor_y, 0);
}

void Board::plantFlag() {
	Tile* pCur = *getTileAt(cursor_x, cursor_y);
	pCur->plantFlag();
}

bool Board::revealTile(Tile* pCenter) {
	vector<Tile*> neighbors;

	int center_x = 0, center_y = 0;
	for (int i = 0; i < (size * size); i++) {
		if (tiles.at(i) == pCenter) {
			center_x = i % size;
			center_y = i / size;
			break;
		}
	}

	// Offset positions of neighbors to check
	int offset_x[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };
	int offset_y[8] = { -1, -1, -1,  0,  0,  1,  1,  1 };

	Tile** pCur;
	for (int i = 0; i < 8; i++) {
		
		pCur = getTileAt(center_x + offset_x[i], center_y + offset_y[i]);
		if (pCur) neighbors.push_back(*pCur);
	}

	int neighboringMines = (pCenter)->reveal(neighbors);

	if (neighboringMines == -1) {	// Tile is a mine
		return false;
	}

	if (neighboringMines == 0) {	// No neighboring mines, check surrounding tiles
		for (Tile* t : neighbors) revealTile(t);
	}

	return true;
}

bool Board::checkForWin() {
	for (Tile* x : tiles) {
		if ( !x->isRevealed() ) return false;
		if ( x->isFlagged() && !(x->hasMine()) ) return false;
	}

	return true;
}

Tile** Board::getTileAt(int x, int y) {
	if (x < 0 || y < 0) return nullptr;
	if (x >= size || y >= size) return nullptr;

	int index = (y * size) + x;

	if ( index >= 0 && index < (size * size) ) {
		return &(tiles.at(index));
	}

	return nullptr;
}
