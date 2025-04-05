#pragma once
#include <list>
#include <fstream>
#include <Windows.h>
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
	Level level;
	PopupWindow popup;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	sf::Vector2f startPos = { 0, 0 };
	Block* movingBlock = nullptr;
	bool leftHold = false;

	const unsigned initWidth = 1920;
	const unsigned initHeight = 1080;
	const unsigned blockStoreWidth = 150;

	sf::Clock clock;
	sf::Time elapsed;
	Block* nextBlockToUpdate = nullptr;
	bool isRunning = false;

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
	void onWindowClosed(const sf::Event::Closed& event);
	void onSimulationStart(const StartSimulationEvent& event);
	void onSimulationStop(const StopSimulationEvent& event);
	void onCarAccident(const CarAccidentEvent& event);
public:
	Game();
	~Game();
	void loop();
};