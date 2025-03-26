#pragma once
#include <list>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Blocks/StartBlock.hpp"
#include "Blocks/TimerBlock.hpp"
#include "Blocks/AccelerationBlock.hpp"
#include "Blocks/DecelerationBlock.hpp"
#include "Blocks/RotationBlock.hpp"

class Game {
	std::list<Block*> blocks;
	std::list<Block*> blockStore;

	Car car;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	sf::Vector2f startPos = { 0, 0 };
	Block* movingBlock = nullptr;
	Block* nextBlockToUpdate = nullptr;
	StartBlock* activeStartBlock = nullptr;
	bool isRunning = false;
	TextField* activeTextField = nullptr;

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