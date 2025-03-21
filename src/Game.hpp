#pragma once
#include <list>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Blocks/StartBlock.hpp"
#include "Blocks/TimerBlock.hpp"
#include "Blocks/AccelerationBlock.hpp"
#include "Blocks/DecelerationBlock.hpp"

class Game {
	std::list<Block*> blocks;
	std::list<Block*> blockStore;

	Car car;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	bool leftHold = false;
	sf::Vector2f startPos = { 0, 0 };
	Block* activeBlock = nullptr;
	Block* nextBlockToUpdate;

	unsigned initWidth;
	unsigned initHeight;
	unsigned blockStoreWidth;
	sf::Clock clock;

	void handleEvents();
	void update();
	void render();
	void reset();

	void saveToFile();
	void loadFromFile();
public:
	Game();
	~Game();
	void loop();
};