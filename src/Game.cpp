#include "Game.hpp"
#include <iostream>

Game::Game() :
	car({ 10, 10 }, & window),
	level(&window, &car, &raceView),
	popup(window),
	blockManager(&window, &car, &blocksView),
	menu(&window, 30, &popup),
	font(".\\assets\\fonts\\Share-Tech-CYR.otf"),
	raceTimeText(font),
	mouseCoordinates(font)
{
	window.create(sf::VideoMode({ initWidth, initHeight }), "Block Race");
	window.setKeyRepeatEnabled(false);
	window.setMinimumSize(sf::Vector2u{ (unsigned) initWidth, (unsigned)initHeight });

	sf::WindowHandle hwnd = window.getNativeHandle();
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	sf::Vector2f windowSize(window.getSize());
	float workAreaHeight = windowSize.y - menu.height;

	appView.setViewport(sf::FloatRect({ 0, 0 }, { 1, 1 }));
	appView.setSize(windowSize);
	appView.setCenter(windowSize / 2.0f);

	raceView.setViewport(sf::FloatRect({ 0.25f, menu.height / windowSize.y }, { 0.75f, workAreaHeight / windowSize.y }));
	raceView.setSize({ 0.75f * windowSize.x, workAreaHeight });
	raceView.setCenter({ 0.75f * windowSize.x / 2, workAreaHeight / 2 });

	blocksView.setViewport(sf::FloatRect({ 0, menu.height / windowSize.y }, { 0.25f, workAreaHeight / windowSize.y }));
	blocksView.setSize({ 0.25f * windowSize.x, workAreaHeight });
	blocksView.setCenter({ 0.25f * windowSize.x / 2, workAreaHeight / 2 });

	mouseCoordinates.setPosition({raceView.getViewport().position.x * windowSize.x,
		raceView.getViewport().position.y * windowSize.y});

	setupEventListeners();

	EventBus::get().publish<SetLevelEvent>({ 1 });
	EventBus::get().publish<LoadFileEvent>(LoadFileEvent{ L"save.dat" });
}

void Game::loop()  
{  
   while (window.isOpen())  
   {  
       handleEvents();
	   update();
	   render();
   }  
}

void Game::setupEventListeners() {
	EventBus::get().subscribe<sf::Event::KeyPressed>(this, &Game::onKeyPressed);
	EventBus::get().subscribe<sf::Event::Resized>(this, &Game::onWindowResized);
	EventBus::get().subscribe<StopSimulationEvent>(this, &Game::onSimulationStop);
	EventBus::get().subscribe<StartSimulationEvent>(this, &Game::onSimulationStart);
	EventBus::get().subscribe<sf::Event::Closed>(this, &Game::onWindowClosed);
	EventBus::get().subscribe<CarAccidentEvent>(this, &Game::onCarAccident);
	EventBus::get().subscribe<SaveFileEvent>(this, &Game::onSaveFile);
	EventBus::get().subscribe<LoadFileEvent>(this, &Game::onLoadFile);
	EventBus::get().subscribe<ExitEvent>(this, &Game::onExit);
	EventBus::get().subscribe<sf::Event::MouseMoved>(this, &Game::onMouseMoved);
	EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &Game::onMousePressed);
	EventBus::get().subscribe<RaceFinishedEvent>(this, &Game::onRaceFinish);
	EventBus::get().subscribe<SetLevelEvent>(this, &Game::onLevelSet);
	EventBus::get().subscribe<NextLevelEvent>(this, &Game::onNextLevel);
}

void Game::handleEvents() {
	while (const std::optional event = window.pollEvent()) {
		if (popup.isVisible()) {
			popup.handleEvent(*event);
		}
		else {
			event->visit([](auto&& e) {
				EventBus::get().publish(std::forward<decltype(e)>(e));
			});
		}
	}
}

void Game::onKeyPressed(const sf::Event::KeyPressed& keyPressed) {
	if (keyPressed.scancode == sf::Keyboard::Scan::Escape) {
		EventBus::get().publish<ExitEvent>(ExitEvent{});
	}
}

void Game::onMouseMoved(const sf::Event::MouseMoved& event)
{
	for (auto& elem : UIElement::getAllElements()) {
		elem->updateHoverState(event.position);
	}
	UIElement::updateCursor(window);

	sf::FloatRect raceBorders{ {raceView.getViewport().position.x * window.getSize().x,
		raceView.getViewport().position.y * window.getSize().y}, raceView.getSize()};

	if (raceBorders.contains(sf::Vector2f(event.position))) {
		mousePos = window.mapPixelToCoords(event.position, raceView);
		isMouseOverRace = true;
	}
	else {
		isMouseOverRace = false;
	}
}

void Game::onMousePressed(const sf::Event::MouseButtonPressed& event)
{
	if (event.button == sf::Mouse::Button::Right) {
		sf::FloatRect raceBorders{ {raceView.getViewport().position.x * window.getSize().x,
			raceView.getViewport().position.y * window.getSize().y}, raceView.getSize() };
		if (raceBorders.contains(sf::Vector2f(event.position))) {
			targetPos = window.mapPixelToCoords(event.position, raceView);
			isTargetSaved = true;
		}
		if (isTargetSaved) {
			sf::FloatRect blocksBorders{ {blocksView.getViewport().position.x * window.getSize().x,
				blocksView.getViewport().position.y * window.getSize().y}, blocksView.getSize() };
			if (blocksBorders.contains(sf::Vector2f(event.position))) {
				isTargetSaved = false;
				EventBus::get().publish<SetTargetEvent>(SetTargetEvent{ targetPos, window.mapPixelToCoords(event.position, blocksView) });
			}
		}		
	}
}

void Game::onWindowResized(const sf::Event::Resized&) {
	sf::Vector2f windowSize(window.getSize());
	float workAreaHeight = windowSize.y - menu.height;

	appView.setViewport(sf::FloatRect({ 0, 0 }, { 1, 1 }));
	appView.setSize(windowSize);
	appView.setCenter(windowSize / 2.0f);

	float blocksViewWidth = std::max(minBlocksViewWidth, windowSize.x * 0.25f);

	raceView.setViewport(sf::FloatRect({ blocksViewWidth / windowSize.x, menu.height / windowSize.y }, { 1 - blocksViewWidth / windowSize.x, workAreaHeight / windowSize.y }));
	raceView.setSize({ windowSize.x - blocksViewWidth , workAreaHeight });
	raceView.setCenter({ (windowSize.x - blocksViewWidth) / 2, workAreaHeight / 2 });

	blocksView.setViewport(sf::FloatRect({ 0, menu.height / windowSize.y }, { blocksViewWidth / windowSize.x, workAreaHeight / windowSize.y }));
	blocksView.setSize({ blocksViewWidth, workAreaHeight });
	blocksView.setCenter({ blocksViewWidth / 2, workAreaHeight / 2 });

	mouseCoordinates.setPosition({ raceView.getViewport().position.x * windowSize.x,
		raceView.getViewport().position.y * windowSize.y });
}

void Game::onSimulationStop(const StopSimulationEvent& event)
{
	blockManager.reset();
	level.reset();
	isRaceOn = false;
}

void Game::onSimulationStart(const StartSimulationEvent& event)
{
	timeAccumulator = sf::seconds(0);
	raceTime = sf::seconds(0);
	isRaceOn = true;
	blockManager.start(event.startBlock);
}

void Game::onRaceFinish(const RaceFinishedEvent& event)
{
	isRaceOn = false;
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
	float raceTimeSecs = raceTime.asSeconds();
	if (timeRecords.find(currentLevel) == timeRecords.end() || raceTimeSecs < timeRecords[currentLevel]) {
		timeRecords[currentLevel] = raceTimeSecs;
	}
	menu.setAvailableLevelsCount(timeRecords.size() + 1);
	popup.showWinMessage(raceTimeSecs, currentLevel == 10);
}

void Game::onCarAccident(const CarAccidentEvent& event)
{
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
	popup.showLossMessage();
}

void Game::onSaveFile(const SaveFileEvent& event)
{
	blockManager.saveToFile(event.fileName);
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
}

void Game::onLoadFile(const LoadFileEvent& event)
{
	blockManager.loadFromFile(event.fileName);
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
}

void Game::onExit(const ExitEvent& event)
{
	if (blockManager.isSaved()) {
		window.close();
		return;
	}
	popup.show(L"Подтверждение выхода", L"Сохранить изменения?",
		{ L"Сохранить", L"Не сохранять", L"Отмена" },
		[this](int option) {
			switch (option) {
			case 0:
				EventBus::get().publish<SaveFileEvent>(SaveFileEvent{ L"save.dat" });
				window.close();
				break;
			case 1:
				window.close();
				break;
			case 2:
				break;
			}
		});
}
void Game::onLevelSet(const SetLevelEvent& event)
{
	currentLevel = event.levelNumber;
	level.setLevel(currentLevel);
}

void Game::onNextLevel(const NextLevelEvent& event)
{
	if (currentLevel < 10) {
		onLevelSet({ ++currentLevel });
	}
}


void Game::onWindowClosed(const sf::Event::Closed& event)
{
	EventBus::get().publish<ExitEvent>(ExitEvent{});
}

void Game::render()
{
    window.clear(sf::Color(100, 100, 100));

	window.setView(raceView);
	level.render(timeAccumulator / deltaTime, isRaceOn);

	window.setView(blocksView);
	blockManager.render();

	window.setView(appView);
	menu.render();
	renderTime();

	std::wstringstream wss;
	if (isMouseOverRace) {
		wss << "X: " << mousePos.x << " Y: " << mousePos.y;
	}	
	if (isTargetSaved) {
		wss << L"\nСохранено X: " << targetPos.x << " Y: " << targetPos.y;
	}
	mouseCoordinates.setString(wss.str());
	window.draw(mouseCoordinates);

	popup.render();

    window.display();
}

void Game::renderTime()
{
	std::wstringstream wss;
	wss << L"Время: " << std::setprecision(2) << std::fixed << raceTime.asSeconds();

	auto currentLevelTimeRecord = timeRecords.find(currentLevel);
	if (currentLevelTimeRecord != timeRecords.end()) {
		float currentRecord = currentLevelTimeRecord->second;
		wss << L"\nРекорд: " << currentRecord;
	}
	raceTimeText.setString(wss.str());
	raceTimeText.setPosition({ window.getSize().x - 200.0f, menu.height });
	window.draw(raceTimeText);
}

void Game::update()
{
	sf::Time dt = clock.restart();
	timeAccumulator += dt;
	if (isRaceOn) {
		raceTime += dt;
	}

	while (timeAccumulator >= deltaTime) {
		blockManager.update(deltaTime);
		car.update(deltaTime);
		level.update();
		timeAccumulator -= deltaTime;
	}	
}
