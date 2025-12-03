#pragma once
#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

// Window and layout constants
#define TILE_SIZE 80
#define PADDING 50
#define STATUS_HEIGHT 60
#define LEGEND_WIDTH 250        // Width for side legend
#define BOARD_SIZE 16
#define NUM_MINES 40
#define WINDOW_WIDTH (BOARD_SIZE * TILE_SIZE + 2 * PADDING + LEGEND_WIDTH)  // Added legend width
#define WINDOW_HEIGHT (BOARD_SIZE * TILE_SIZE + 2 * PADDING + STATUS_HEIGHT)  // Removed legend height

class Game {
public:
    Game(int boardSize = BOARD_SIZE, int numMines = NUM_MINES);
    ~Game();  // Destructor

    void run();  // Main game loop

private:
    // Event handling and game logic
    void handleEvents();
    void update();
    void render();
    void resetGame();

    // Rendering functions
    void drawTile(int x, int y, const Tile* tile);
    void drawGrid();
    void drawGameOverlay();
    void drawLegend();  // Draw control instructions

    // Game data
    Board* board;
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;

    // Game state enum
    enum class GameState {
        PLAYING,
        WON,
        LOST
    };
    GameState gameState;

    // Color scheme
    sf::Color colorUnrevealed;
    sf::Color colorRevealed;
    sf::Color colorMine;
    sf::Color colorFlag;
    sf::Color colorCursor;
    sf::Color colorGrid;
    sf::Color colorBackground;
    sf::Color colorText;  // For legend text

    // Helper function for number colors
    sf::Color getNumberColor(int number);
};