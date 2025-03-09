#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Blocks/StartBlock.hpp"
#include "Blocks/TimerBlock.hpp"
#include "Blocks/AccelerationBlock.hpp"

class Block;
class StartBlock;

class Game {
	std::vector<Block*> blocks;
	StartBlock* startBlock;

	Car car;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	bool leftHold = false;
	sf::Vector2f startPos = { 0, 0 };
	Block* activeBlock = nullptr;

	unsigned initWidth;
	unsigned initHeight;
	sf::Clock clock;

	void handleEvents();
	void update();
	void render();
public:
	Game();
	~Game();
	void loop();
};