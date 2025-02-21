#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Block;
class StartBlock;

class Game {
	std::vector<Block*> blocks;
	StartBlock* startBlock;
	sf::View appView;
	sf::View blocksView;
	sf::View raceView;

	bool leftHold = false;
	sf::Vector2f startPos = { 0, 0 };
	Block* activeBlock = nullptr;

public:
	unsigned initWidth;
	unsigned initHeight;
	sf::RenderWindow window;

	Game();
	~Game();
	void loop();
	void handleEvents();
	void update();
	void render();
};