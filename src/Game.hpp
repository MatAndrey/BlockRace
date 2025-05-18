#pragma once
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Level.hpp"
#include "EventBus.hpp"
#include "UI/PopupWindow.hpp"
#include "BlockManager.hpp"
#include "UI/Menu.hpp"
#include "UI/UIElement.hpp"
#include "Cheats.hpp"

class Game {
	Car car;
	Level level;
	PopupWindow popup;
	BlockManager blockManager;
	Menu menu;

	sf::View appView;
	sf::View blocksView;
	sf::View raceView;
	sf::RenderWindow window;

	sf::Clock clock;
	sf::Time timeAccumulator = sf::seconds(0);
	sf::Time deltaTime = sf::seconds(1.0 / 60.0);

	sf::Time raceTime;
	bool isRaceOn = false;
	sf::Font font;
	sf::Text raceTimeText;
	sf::Text mouseCoordinates;
	int currentLevel = 1;

	const unsigned initWidth = 800;
	const unsigned initHeight = 600;

	sf::Vector2f targetPos;
	sf::Vector2f mousePos;
	bool isMouseOverRace = false;
	bool isTargetSaved = false;

	void update();
	void render();
	void renderTime();

	void setupEventListeners();
	void handleEvents();
	void onKeyPressed(const sf::Event::KeyPressed& event);
	void onMouseMoved(const sf::Event::MouseMoved& event);
	void onMousePressed(const sf::Event::MouseButtonPressed& event);
	void onWindowResized(const sf::Event::Resized& event);
	void onWindowClosed(const sf::Event::Closed& event);
	void onSimulationStop(const StopSimulationEvent& event);
	void onSimulationStart(const StartSimulationEvent& event);
	void onRaceFinish(const RaceFinishedEvent& event);
	void onCarAccident(const CarAccidentEvent& event);
	void onSaveFile(const SaveFileEvent& event);
	void onLoadFile(const LoadFileEvent& event);
	void onExit(const ExitEvent& event);
	void onLevelSet(const SetLevelEvent& event);
	void onNextLevel(const NextLevelEvent& event);

	Cheats cheats;
public:
	Game();
	void loop();
};