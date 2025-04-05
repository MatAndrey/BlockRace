#include "Game.hpp"
#include <iostream>

Game::Game() :
	car({ 10, 10 }, & window),
	level(".\\assets\\maps\\level1.json", &window, &car),
	popup(window)
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

	blockStore.push_back(new StartBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new TimerBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new AccelerationBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new DecelerationBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), &window, sf::degrees(5)));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), &window, sf::degrees(-5)));

	float y = 10;
	for (auto block : blockStore) {
		block->moveTo({ 10, y });
		y += block->size.y + 10;
	}

	loadFromFile();
	setupEventListeners();
}

Game::~Game()
{
	saveToFile();
	for (auto& block : blocks) {
		delete block;
	}
	for (auto& block : blockStore) {
		delete block;
	}
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
	EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &Game::onMouseButtonPressed);
	EventBus::get().subscribe<sf::Event::MouseButtonReleased>(this, &Game::onMouseButtonReleased);
	EventBus::get().subscribe<sf::Event::MouseMoved>(this, &Game::onMouseMoved);
	EventBus::get().subscribe<StartSimulationEvent>(this, &Game::onSimulationStart);
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

void Game::onMouseButtonPressed(const sf::Event::MouseButtonPressed& mouseButtonPressed) {
	if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
		sf::FloatRect blocksViewRect(blocksView.getCenter() - blocksView.getSize() / 2.f, blocksView.getSize());
		sf::Vector2f worldPos = window.mapPixelToCoords(mouseButtonPressed.position, blocksView);

		if (blocksViewRect.contains(sf::Vector2f(mouseButtonPressed.position))) {
			
			EventBus::get().publish(BlockPressedEvent{
					worldPos,
					sf::Mouse::Button::Left,
					isRunning
				});

		}

		startPos = worldPos;
		for (auto& block : blocks) {
			if (block->isInBoundingBox(worldPos) && !isRunning) {
				movingBlock = block;
			}
		}

		if (!movingBlock && blocksViewRect.contains(sf::Vector2f(mouseButtonPressed.position)) && worldPos.x > blockStoreWidth && !isRunning) {
			leftHold = true;
		}

		if (!isRunning) {
			for (auto block : blockStore) {
				if (block->isInBoundingBox(worldPos)) {
					startPos = worldPos;
					movingBlock = block->clone();
					blocks.push_back(movingBlock);
				}
			}
		}
	}
}

void Game::onMouseButtonReleased(const sf::Event::MouseButtonReleased& mouseButtonReleased) {
	if (mouseButtonReleased.button == sf::Mouse::Button::Left && !isRunning) {
		leftHold = false;
		for (auto iter = blocks.begin(); iter != blocks.end();) {
			Block* block = *iter;

			if (block->pos.x + block->size.x < blockStoreWidth) {
				iter = blocks.erase(iter);
				delete block;
				continue;
			}
			else {
				iter++;
			}
			if (block->blockInteract(movingBlock)) {
				break;
			}
		}

		movingBlock = nullptr;
	}
}

void Game::onMouseMoved(const sf::Event::MouseMoved& mouseMoved) {
	sf::Vector2f worldPos = window.mapPixelToCoords(mouseMoved.position, blocksView);
	bool isOver = false;	
	for (Block* block : blocks) {
		if (block->name() == "StartBlock") {
			StartBlock* sb = dynamic_cast<StartBlock*>(block);
			if (sb->isMouseOver(worldPos)) {
				isOver = true;
				break;
			}
		}
	}
	if (isOver) {
		const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
		window.setMouseCursor(cursor);
	}
	else {
		const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
		window.setMouseCursor(cursor);
	}

	if (!isRunning) {
		if (movingBlock != nullptr) {
			sf::FloatRect viewBounds({
				blocksView.getCenter().x - blocksView.getSize().x / 2,
				blocksView.getCenter().y - blocksView.getSize().y / 2 },
				{ blocksView.getSize().x, blocksView.getSize().y });

			
			if (viewBounds.contains(worldPos)) {
				movingBlock->moveBy(worldPos - startPos);
				startPos = worldPos;
			}
		}
		else if(leftHold) {
			for (Block* block : blocks) {
				block->pos += worldPos - startPos;
				if (block->pos.x < blockStoreWidth) {
					block->pos.x = blockStoreWidth;
				}
			}
			startPos = worldPos;
		}
	}
}

void Game::onSimulationStart(const StartSimulationEvent& event)
{
	if (!isRunning) {
		isRunning = true;
		nextBlockToUpdate = event.startBlock->getNext();
		elapsed = sf::seconds(0);
	}
}

void Game::onSimulationStop(const StopSimulationEvent& event)
{
	if (isRunning) {
		level.carReset();
		EventBus::get().publish<DisableBlocksEvent>(DisableBlocksEvent{});
		nextBlockToUpdate = nullptr;
		isRunning = false;
		for (const auto& block : blocks) {
			block->deactivate(car);
		}
		
	}
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
	// TODO
	sf::RectangleShape blocksBackground;
	blocksBackground.setFillColor(sf::Color(200, 200, 200));
	blocksBackground.setSize({ 1920, 1080 });
	window.draw(blocksBackground);
	sf::RectangleShape storeBackground;
	storeBackground.setFillColor(sf::Color(150, 150, 150));
	storeBackground.setSize({ (float)blockStoreWidth, 1080 });
	window.draw(storeBackground);

	for (const auto& block : blockStore) {
		block->render();
	}
	for (const auto& block : blocks) {
		block->render();
	}

	window.setView(appView);
	popup.render();

    window.display();
    
}

void Game::saveToFile()
{
	std::ofstream file("save.dat");
	for (const auto& block : blocks) {
		if (block->name() == "RotationBlock") {
			RotationBlock* rb = dynamic_cast<RotationBlock*>(block);
			file << rb->name() << "\t" << rb->pos.x << "\t" << rb->pos.y << "\t" << rb->getAngle() << "\n";
		} else if (block->name() == "TimerBlock") {
			TimerBlock* tb = dynamic_cast<TimerBlock*>(block);
			file << tb->name() << "\t" << tb->pos.x << "\t" << tb->pos.y << "\t" << tb->size.y << "\t" << tb->getDuration() << "\n";
		}
		else {
			file << block->name() << "\t" << block->pos.x << "\t" << block->pos.y << "\n";
		}
	}
	file.close();
}

void Game::loadFromFile()
{
	std::ifstream file("save.dat");
	if (!file) {
		return;
	}
	while(!file.eof()) {
		std::string type;
		sf::Vector2f pos;
		file >> type >> pos.x >> pos.y;
		Block* block = nullptr;
		if (type == "StartBlock") {
			block = new StartBlock(pos, &window);
		}
		else if (type == "AccelerationBlock") {
			block = new AccelerationBlock(pos, &window);
		}
		else if (type == "DecelerationBlock") {
			block = new DecelerationBlock(pos, &window);
		}
		else if (type == "TimerBlock") {
			double duration;
			float height;
			file >> height >> duration;

			block = new TimerBlock(pos, &window, height, duration);
		}
		else if (type == "RotationBlock") {
			float angle;
			file >> angle;
			block = new RotationBlock(pos, &window, sf::degrees(angle));
		}
		if (block) {
			blocks.push_back(block);
		}
	}
	file.close();
	for (Block* a : blocks) {
		for (Block* b : blocks) {
			a->blockInteract(b, false);
		}
	}
}

void Game::update()
{
	sf::Time dt = clock.restart();
	elapsed += dt;

	if (nextBlockToUpdate) {
		elapsed -= nextBlockToUpdate->update(car, elapsed);
		if (!nextBlockToUpdate->isRunning) {			
			nextBlockToUpdate = nextBlockToUpdate->getNext();
		}
	}

	car.update(dt);
	level.update();
}
