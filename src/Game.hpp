#pragma once
#include <fstream>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Level.hpp"
#include "EventBus.hpp"
#include "UI/PopupWindow.hpp"
#include "BlockManager.hpp"

class Game {
	Car car;
	Level level;
	PopupWindow popup;
	BlockManager blockManager;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	sf::Clock clock;

	const unsigned initWidth = 1920;
	const unsigned initHeight = 1080;

	void update();
	void render();

	void setupEventListeners();
	void handleEvents();
	void onKeyPressed(const sf::Event::KeyPressed& event);
	void onWindowResized(const sf::Event::Resized& event);
	void onWindowClosed(const sf::Event::Closed& event);
	void onSimulationStop(const StopSimulationEvent& event);
	void onCarAccident(const CarAccidentEvent& event);
public:
	Game();
	~Game();
	void loop();
};