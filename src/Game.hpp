#pragma once
#include <vector>
#include "Blocks/Block.hpp"
#include "Blocks/StartBlock.hpp"

class Game {
	std::vector<Block*> blocks;
	StartBlock* startBlock;
	sf::RenderWindow window;

public:
	Game();
	~Game();
	void loop();
	void handleEvents();
	void update();
	void draw();
};