#include "Game.hpp"

void Game::play() {
    while (true) {
        // Handle exit option from main menu
        if (mainMenu() == 2) return;

        // Play the game
        srand(time(NULL));
        gameBoard = new Board(9, 10);
        while (takeTurn());
    }
}

int Game::mainMenu() {
	string options = "[1] - New Game\n[2] - Exit\n> ";
	int choice = 0;

	while (choice < 1 || choice > 2) {
		system("cls");
		cout << options;
		cin >> choice;
	}
	
	return choice;
}

bool Game::takeTurn() {
	system("cls");
	gameBoard->print(true);

    // Moving cursor and interacting with tiles
    int dir = 0;
    while (dir == 0) {
        switch (dir = _getch()) {
        case KEY_UP:
            gameBoard->moveCursor(0, -1);   // Move up
            break;
        case KEY_DOWN:
            gameBoard->moveCursor(0, 1);    // Move down
            break;
        case KEY_LEFT:
            gameBoard->moveCursor(-1, 0);   // Move left
            break;
        case KEY_RIGHT:
            gameBoard->moveCursor(1, 0);    // Move right
            break;
        case ONE:
            return revealTile();
            break;
        case TWO:
            return plantFlag();
            break;
        default:
            dir = 0;
        }
    }
	
	return true;
}

bool Game::revealTile() {
    if (!gameBoard->revealTile()) {
        showLoss();
        return false;
    }

    if (gameBoard->checkForWin()) {
        showWin();
        return false;
    }
    return true;
}

bool Game::plantFlag() {
    gameBoard->plantFlag();
    return true;
}

void Game::showLoss() {
    system("cls");
    gameBoard->print(false);
    cout << endl;
    cout << "You lost..." << endl;
    cout << "Press any key ";
    _getch();
}

void Game::showWin() {
    system("cls");
    gameBoard->print(true);
    cout << endl;
    cout << "Congratulations, you WON!" << endl;
    cout << "Press any key ";
    _getch();
}
