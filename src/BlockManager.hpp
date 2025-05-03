#pragma once
#include "Blocks/StartBlock.hpp"
#include "Blocks/TimerBlock.hpp"
#include "Blocks/AccelerationBlock.hpp"
#include "Blocks/DecelerationBlock.hpp"
#include "Blocks/RotationBlock.hpp"
#include "Blocks/MoveToBlock.hpp"
#include "Blocks/ConstSpeedBlock.hpp"
#include <list>
#include <fstream>

class BlockManager {
	std::list<Block*> blocks;
	std::list<Block*> blockStore;
	Car* car;

	sf::Vector2f startPos = { 0, 0 };
	Block* movingBlock = nullptr;
	bool leftHold = false;

	Block* nextBlockToUpdate = nullptr;
	bool isRunning = false;
	bool isSavedNow = true;

	const float blockStoreWidth = 170;

	sf::RenderWindow* window;
	sf::View* blocksView;
	sf::RectangleShape blocksBackground;
	sf::RectangleShape storeBackground;
public:
	BlockManager(sf::RenderWindow* window, Car* car, sf::View* blocksView);
	void update(sf::Time deltaTime);
	void render();
	void saveToFile(std::wstring fileName);
	void loadFromFile(std::wstring fileName);
	void reset();
	bool isSaved() const;
	void start(StartBlock* sb);
	
	void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event);
	void onMouseButtonReleased(const sf::Event::MouseButtonReleased& event);
	void onMouseMoved(const sf::Event::MouseMoved& event);
	void onClearBlocks(const ClearBlocksEvent& event);
};