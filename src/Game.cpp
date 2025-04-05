#include "Game.hpp"
#include <iostream>

Game::Game() :
	car({ 10, 10 }, &window),
	level(".\\assets\\maps\\level1.json", &window, &car),
	popup(window),
	blockManager(&window, &car, &blocksView)
{  
	window.create(sf::VideoMode({ initWidth, initHeight }), "Block Race");
	
	sf::WindowHandle hwnd = window.getNativeHandle();
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	appView.setViewport(sf::FloatRect({ 0, 0 }, { 1, 1 }));
	appView.setSize({ window.getSize().x * 1.0f, window.getSize().y * 1.0f });
	appView.setCenter({ window.getSize().x / 2.0f, window.getSize().y / 2.0f });

	raceView.setViewport(sf::FloatRect({ 0.25f, 0.05f }, { 0.75f, 0.95f }));
	raceView.setSize({ 0.75f * window.getSize().x, 0.95f * window.getSize().y });
	raceView.setCenter({ 0.75f * window.getSize().x / 2, 0.95f * window.getSize().y / 2 });

	blocksView.setViewport(sf::FloatRect({ 0, 0.05f }, { 0.25f, 0.95f }));
	blocksView.setSize({0.25f * window.getSize().x, 0.95f * window.getSize().y });
	blocksView.setCenter({0.25f * window.getSize().x / 2, 0.95f * window.getSize().y / 2 });

	setupEventListeners();
}

Game::~Game()
{
	// TODO
	blockManager.saveToFile();
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
	EventBus::get().subscribe<sf::Event::Closed>(this, &Game::onWindowClosed);
	EventBus::get().subscribe<CarAccidentEvent>(this, &Game::onCarAccident);
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
		window.close();
/*		popup.show(L"Подтверждение выхода", L"Сохранить изменения?",
			{ L"Сохранить", L"Не сохранять", L"Отмена" },
			[this](int option) {
				switch (option) {
				case 0:
					saveToFile();
					window.close();
					break;
				case 1:
					window.close();
					break;
				case 2:
					break;
				}
			})*/;
	}
}

void Game::onWindowResized(const sf::Event::Resized&) {
	appView.setSize({ window.getSize().x * 1.0f, window.getSize().y * 1.0f });
	appView.setCenter({ window.getSize().x / 2.0f, window.getSize().y / 2.0f });

	blocksView.setSize({ 0.25f * window.getSize().x, 0.95f * window.getSize().y });
	blocksView.setCenter({ 0.25f * window.getSize().x / 2, 0.95f * window.getSize().y / 2 });

	raceView.setSize({ 0.75f * window.getSize().x, 0.95f * window.getSize().y });
	raceView.setCenter({ 0.75f * window.getSize().x / 2, 0.95f * window.getSize().y / 2 });
}

void Game::onSimulationStop(const StopSimulationEvent& event)
{
	level.reset();
	blockManager.reset();
}

void Game::onCarAccident(const CarAccidentEvent& event)
{
	EventBus::get().publish<StopSimulationEvent>(StopSimulationEvent{});
	popup.show(L"Авария", L"Машинка выкатилась за пределы дороги",
		{ L"Заново" },
		[this](int option) {});
}

void Game::onWindowClosed(const sf::Event::Closed& event)
{
	window.close();
}

void Game::render()
{
    window.clear(sf::Color(100, 100, 100));

	window.setView(raceView);
	level.render(raceView);

	window.setView(blocksView);
	blockManager.render();

	window.setView(appView);
	popup.render();

    window.display();
}

void Game::update()
{
	sf::Time dt = clock.restart();
	blockManager.update(dt);
	car.update(dt);
	level.update();
}
