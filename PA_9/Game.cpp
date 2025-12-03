#include "Game.hpp"

Game::Game(int boardSize, int numMines)
	: gameState(GameState::PLAYING) {

	board = new Board(boardSize, numMines);

	window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Minesweeper - SFML");
	window.setFramerateLimit(60);

	// ===== AI CODE =====
	// Prompt: Help me for font handling with SFML
	// What it does: Tries to load a font - if it fails, we'll use shapes as fallback
	fontLoaded = false;
#ifdef _WIN32
	fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");
#elif __APPLE__
	fontLoaded = font.openFromFile("/System/Library/Fonts/Helvetica.ttc");
#else
	fontLoaded = font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#endif
	if (!fontLoaded) {
		fontLoaded = font.openFromFile("arial.ttf");
	}
	// ===== END AI CODE =====

	// Initialize color scheme (AI-generated colors)
	colorUnrevealed = sf::Color(180, 180, 180);
	colorRevealed = sf::Color(220, 220, 220);
	colorMine = sf::Color(220, 20, 20);
	colorFlag = sf::Color(255, 200, 0);
	colorCursor = sf::Color(100, 150, 255, 100);
	colorGrid = sf::Color(100, 100, 100);
	colorBackground = sf::Color(240, 240, 240);
	colorText = sf::Color(50, 50, 50);  // Dark gray for text
}

Game::~Game() {
	delete board;
}

void Game::run() {
	// Main game loop
	while (window.isOpen()) {
		handleEvents();
		update();
		render();
	}
}

void Game::handleEvents() {
	while (const std::optional<sf::Event> event = window.pollEvent()) {
		// Window close event
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}

		if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
			// Arrow key movement
			if (keyPress->code == sf::Keyboard::Key::Up) {
				board->moveCursor(0, -1);
			}
			else if (keyPress->code == sf::Keyboard::Key::Down) {
				board->moveCursor(0, 1);
			}
			else if (keyPress->code == sf::Keyboard::Key::Left) {
				board->moveCursor(-1, 0);
			}
			else if (keyPress->code == sf::Keyboard::Key::Right) {
				board->moveCursor(1, 0);
			}
			// Game actions
			else if (keyPress->code == sf::Keyboard::Key::Space) {
				// Reveal tile - if it's a mine, player loses
				if (!board->revealTile()) {
					gameState = GameState::LOST;
				}
			}
			else if (keyPress->code == sf::Keyboard::Key::F) {
				// Plant/remove flag
				board->plantFlag();
			}
			else if (keyPress->code == sf::Keyboard::Key::R) {
				// Restart game
				resetGame();
			}
		}
	}
}

void Game::update() {
	// Check for win condition
	if (gameState == GameState::PLAYING) {
		if (board->checkForWin()) {
			gameState = GameState::WON;
		}
	}
}

void Game::render() {
	window.clear(colorBackground);

	drawGrid();          // Draw the minesweeper board
	drawGameOverlay();   // Draw status bar at top
	drawLegend();        // Draw control instructions at bottom

	window.display();
}

void Game::drawTile(int x, int y, const Tile* tile) {
	if (!tile) return;

	// Calculate tile position on screen
	float posX = static_cast<float>(PADDING + x * TILE_SIZE);
	float posY = static_cast<float>(PADDING + STATUS_HEIGHT + y * TILE_SIZE);

	// Draw tile background (with 2px spacing for grid lines)
	sf::RectangleShape tileRect(sf::Vector2f(static_cast<float>(TILE_SIZE - 2), static_cast<float>(TILE_SIZE - 2)));
	tileRect.setPosition(sf::Vector2f(posX + 1.f, posY + 1.f));

	// Handle flagged tiles
	if (tile->isFlagged()) {
		// Draw unrevealed background
		tileRect.setFillColor(colorUnrevealed);
		window.draw(tileRect);

		// Draw flag symbol (pole + triangle)
		float flagPoleWidth = 4.f;
		float flagPoleHeight = TILE_SIZE * 0.6f;
		sf::RectangleShape flagPole(sf::Vector2f(flagPoleWidth, flagPoleHeight));
		flagPole.setFillColor(sf::Color::Black);
		flagPole.setPosition(sf::Vector2f(posX + TILE_SIZE / 2.f - flagPoleWidth / 2.f, posY + TILE_SIZE * 0.2f));
		window.draw(flagPole);

		sf::ConvexShape flag;
		flag.setPointCount(3);
		flag.setPoint(0, sf::Vector2f(posX + TILE_SIZE / 2.f, posY + TILE_SIZE * 0.2f));
		flag.setPoint(1, sf::Vector2f(posX + TILE_SIZE * 0.8f, posY + TILE_SIZE * 0.35f));
		flag.setPoint(2, sf::Vector2f(posX + TILE_SIZE / 2.f, posY + TILE_SIZE * 0.5f));
		flag.setFillColor(colorFlag);
		window.draw(flag);
	}
	// Handle unrevealed, unflagged tiles
	else if (!tile->isRevealed()) {
		tileRect.setFillColor(colorUnrevealed);
		window.draw(tileRect);
	}
	// Handle revealed tiles
	else {
		if (tile->hasMine()) {
			// Draw mine tile
			tileRect.setFillColor(colorMine);
			window.draw(tileRect);

			// Draw classic mine symbol (circle with spikes)
			float mineRadius = TILE_SIZE / 5.f;
			float mineCenterX = posX + TILE_SIZE / 2.f;
			float mineCenterY = posY + TILE_SIZE / 2.f;

			// Center circle
			sf::CircleShape mineCenter(mineRadius);
			mineCenter.setFillColor(sf::Color::Black);
			mineCenter.setPosition(sf::Vector2f(mineCenterX - mineRadius, mineCenterY - mineRadius));
			window.draw(mineCenter);

			// 8 spikes radiating outward
			for (int i = 0; i < 8; i++) {
				sf::RectangleShape spike(sf::Vector2f(mineRadius * 1.5f, 3.f));
				spike.setFillColor(sf::Color::Black);
				spike.setOrigin(sf::Vector2f(0, 1.5f));
				spike.setPosition(sf::Vector2f(mineCenterX, mineCenterY));
				spike.setRotation(sf::degrees(i * 45.f));
				window.draw(spike);
			}
		}
		else {
			// Draw empty revealed tile
			tileRect.setFillColor(colorRevealed);
			window.draw(tileRect);

			// Draw number if there are neighboring mines
			char symbol = tile->getSymbol();
			if (symbol >= '1' && symbol <= '8') {
				int number = symbol - '0';
				sf::Color numColor = getNumberColor(number);

				if (fontLoaded) {
					// Use text rendering for numbers
					sf::Text text(font);
					text.setString(std::string(1, symbol));
					text.setCharacterSize(48);
					text.setFillColor(numColor);

					// Center the text in the tile
					sf::FloatRect textBounds = text.getLocalBounds();
					text.setPosition(sf::Vector2f(
						posX + (TILE_SIZE - textBounds.size.x) / 2.f - textBounds.position.x,
						posY + (TILE_SIZE - textBounds.size.y) / 2.f - textBounds.position.y
					));
					window.draw(text);
				}
				else {
					// Fallback: use dots if font didn't load
					float circleRadius = 3.f;
					float centerX = posX + TILE_SIZE / 2.f;
					float centerY = posY + TILE_SIZE / 2.f;

					for (int i = 0; i < number; i++) {
						sf::CircleShape dot(circleRadius);
						dot.setFillColor(numColor);

						// Arrange dots in a 3x3 grid pattern
						int row = i / 3;
						int col = i % 3;
						float offsetX = (col - 1) * 10.f;
						float offsetY = (row - 1) * 10.f;

						dot.setPosition(sf::Vector2f(centerX + offsetX - circleRadius, centerY + offsetY - circleRadius));
						window.draw(dot);
					}
				}
			}
		}
	}

	// Highlight cursor position with semi-transparent overlay
	if (x == board->getCursorX() && y == board->getCursorY()) {
		sf::RectangleShape cursor(sf::Vector2f(static_cast<float>(TILE_SIZE - 1), static_cast<float>(TILE_SIZE - 1)));
		cursor.setPosition(sf::Vector2f(posX + 0.5f, posY + 0.5f));
		cursor.setFillColor(sf::Color(255, 255, 100, 80));  // Semi-transparent yellow
		window.draw(cursor);
	}
}

void Game::drawGrid() {
	int size = board->getSize();

	// Draw all tiles
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			Tile* tile = board->getTile(x, y);
			drawTile(x, y, tile);
		}
	}

	// Draw grid lines (vertical and horizontal)
	for (int i = 0; i <= size; i++) {
		// Vertical lines
		sf::RectangleShape vLine(sf::Vector2f(1.f, static_cast<float>(size * TILE_SIZE)));
		vLine.setPosition(sf::Vector2f(static_cast<float>(PADDING + i * TILE_SIZE), static_cast<float>(PADDING + STATUS_HEIGHT)));
		vLine.setFillColor(colorGrid);
		window.draw(vLine);

		// Horizontal lines
		sf::RectangleShape hLine(sf::Vector2f(static_cast<float>(size * TILE_SIZE), 1.f));
		hLine.setPosition(sf::Vector2f(static_cast<float>(PADDING), static_cast<float>(PADDING + STATUS_HEIGHT + i * TILE_SIZE)));
		hLine.setFillColor(colorGrid);
		window.draw(hLine);
	}
}

void Game::drawGameOverlay() {
	// Draw status bar at top with game state colors
	sf::RectangleShape statusBar(sf::Vector2f(
		static_cast<float>(board->getSize() * TILE_SIZE),
		static_cast<float>(STATUS_HEIGHT - 10)
	));
	statusBar.setPosition(sf::Vector2f(static_cast<float>(PADDING), static_cast<float>(PADDING + 5)));
	statusBar.setOutlineColor(colorGrid);
	statusBar.setOutlineThickness(2.f);

	// Color status bar based on game state
	std::string statusText = "";
	if (gameState == GameState::PLAYING) {
		statusBar.setFillColor(sf::Color(200, 200, 200));  // Gray - playing
		statusText = "Playing... Find all the mines!";
	}
	else if (gameState == GameState::WON) {
		statusBar.setFillColor(sf::Color(100, 255, 100));  // Green - won
		statusText = "You Won! Press R to play again";
	}
	else if (gameState == GameState::LOST) {
		statusBar.setFillColor(sf::Color(255, 100, 100));  // Red - lost
		statusText = "Game Over! Press R to try again";
	}

	window.draw(statusBar);

	// Draw status text if font is loaded
	if (fontLoaded && !statusText.empty()) {
		sf::Text text(font);
		text.setString(statusText);
		text.setCharacterSize(22);
		text.setStyle(sf::Text::Bold);
		text.setFillColor(colorText);

		// Center the text in the status bar
		sf::FloatRect textBounds = text.getLocalBounds();
		float textX = static_cast<float>(PADDING) + (board->getSize() * TILE_SIZE - textBounds.size.x) / 2.f;
		float textY = static_cast<float>(PADDING + 5) + (STATUS_HEIGHT - 10 - textBounds.size.y) / 2.f - textBounds.position.y;
		text.setPosition(sf::Vector2f(textX, textY));

		window.draw(text);
	}
}

void Game::drawLegend() {
	// Calculate legend position (right side of board)
	int boardPixelWidth = board->getSize() * TILE_SIZE;
	float legendX = static_cast<float>(PADDING + boardPixelWidth + 20);
	float legendY = static_cast<float>(PADDING + STATUS_HEIGHT);

	// Draw legend background
	sf::RectangleShape legendBg(sf::Vector2f(
		static_cast<float>(LEGEND_WIDTH - 30),
		static_cast<float>(board->getSize() * TILE_SIZE)
	));
	legendBg.setPosition(sf::Vector2f(legendX, legendY));
	legendBg.setFillColor(sf::Color(200, 200, 200));
	legendBg.setOutlineColor(colorGrid);
	legendBg.setOutlineThickness(2.f);
	window.draw(legendBg);

	// Only draw text if font loaded successfully
	if (fontLoaded) {
		// Title
		sf::Text title(font);
		title.setString("Controls");
		title.setCharacterSize(28);  // Increased from 24
		title.setStyle(sf::Text::Bold);
		title.setFillColor(colorText);
		title.setPosition(sf::Vector2f(legendX + 20, legendY + 20));
		window.draw(title);

		// Create control instruction strings
		std::string controls[] = {
			"Arrow Keys:",
			"  Move cursor",
			"",
			"SPACE:",
			"  Reveal tile",
			"",
			"F:",
			"  Flag/Unflag tile",
			"",
			"R:",
			"  Restart game"
		};

		float textX = legendX + 20;
		float textY = legendY + 75.f;  // Slightly adjusted
		float lineSpacing = 28.f;      // Increased from 24

		// Draw each control instruction
		for (int i = 0; i < 11; i++) {
			if (controls[i].empty()) continue;  // Skip empty lines

			sf::Text text(font);
			text.setString(controls[i]);

			// Bold for control keys, regular for descriptions
			if (controls[i].find(":") != std::string::npos) {
				text.setCharacterSize(24);  // Increased from 20
				text.setStyle(sf::Text::Bold);
			}
			else {
				text.setCharacterSize(22);  // Increased from 18
			}

			text.setFillColor(colorText);
			text.setPosition(sf::Vector2f(textX, textY + i * lineSpacing));
			window.draw(text);
		}
	}
}

void Game::resetGame() {
	// Reset board with same size and mine count
	int size = board->getSize();
	int mines = board->getMines();
	delete board;
	board = new Board(size, mines);
	gameState = GameState::PLAYING;
}

sf::Color Game::getNumberColor(int number) {
	// Classic minesweeper number colors (AI-generated)
	switch (number) {
	case 1: return sf::Color(0, 0, 255);      // Blue
	case 2: return sf::Color(0, 150, 0);      // Green
	case 3: return sf::Color(255, 0, 0);      // Red
	case 4: return sf::Color(0, 0, 139);      // Dark Blue
	case 5: return sf::Color(139, 0, 0);      // Dark Red
	case 6: return sf::Color(0, 139, 139);    // Cyan
	case 7: return sf::Color(0, 0, 0);        // Black
	case 8: return sf::Color(128, 128, 128);  // Gray
	default: return sf::Color::Black;
	}
}