#pragma once
#include <list>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Blocks/StartBlock.hpp"
#include "Blocks/TimerBlock.hpp"
#include "Blocks/AccelerationBlock.hpp"
#include "Blocks/DecelerationBlock.hpp"
#include "Blocks/RotationBlock.hpp"
#include "Level.hpp"
#include "EventBus.hpp"
#include "UI/PopupWindow.hpp"

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
	bool leftHold = false;
	Block* nextBlockToUpdate = nullptr;
	bool isRunning = false;

	unsigned initWidth;
	unsigned initHeight;
	unsigned blockStoreWidth;
	sf::Clock clock;
	sf::Time elapsed;

	Level level;
	PopupWindow popup;

	void update();
	void render();
	void saveToFile();
	void loadFromFile();

	void setupEventListeners();

	void handleEvents();
	void onKeyPressed(const sf::Event::KeyPressed& event);
	void onWindowResized(const sf::Event::Resized& event);
	void onMouseButtonPressed(const sf::Event::MouseButtonPressed& event);
	void onMouseButtonReleased(const sf::Event::MouseButtonReleased& event);
	void onMouseMoved(const sf::Event::MouseMoved& event);
	void onSimulationStart(const StartSimulationEvent& event);
	void onSimulationStop(const StopSimulationEvent& event);
public:
	Game();
	~Game();
	void loop();
};