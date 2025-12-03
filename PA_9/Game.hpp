#pragma once

#include "Board.hpp"
#include <string>
#include <iostream>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ONE 49
#define TWO 50

using std::string;
using std::cin;
using std::cout;

class Game {
public:
	void play();

private:
	Board* gameBoard;

	int mainMenu();
	bool takeTurn();
	bool revealTile();
	bool plantFlag();
	void showLoss();
	void showWin();
};
