#include "Game.hpp"
#include <iostream>

Game::Game() :
	initHeight(1080),
	initWidth(1920),
	blockStoreWidth(150),
	car({ 10, 10 }, & window)
{  
	window.create(sf::VideoMode({ initWidth, initHeight }), "Block Race");

	appView.setViewport(sf::FloatRect({ 0, 0 }, { 1, 1 }));

	raceView.setViewport(sf::FloatRect({ 0.25f, 0.05f }, { 0.75f, 0.9f }));
	raceView.setSize({ 0.75f * window.getSize().x, 0.9f * window.getSize().y });
	raceView.setCenter({ 0.75f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });

	blocksView.setViewport(sf::FloatRect({ 0, 0.05f }, { 0.25f, 0.9f }));
	blocksView.setSize({0.25f * window.getSize().x, 0.9f * window.getSize().y });
	blocksView.setCenter({0.25f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });

	blockStore.push_back(new StartBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new TimerBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new AccelerationBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new DecelerationBlock(sf::Vector2f(0, 0), &window));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), &window, sf::degrees(1.5)));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), &window, sf::degrees(-1.5)));
	blockStore.push_back(new RotationBlock(sf::Vector2f(0, 0), &window, sf::degrees(0)));

	float y = 10;
	for (auto block : blockStore) {
		block->moveTo({ 10, y });
		y += block->size.y + 10;
	}

	reset();
	loadFromFile();
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

void Game::handleEvents() {
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
			{
				window.close();
			}
		}
		if (event->is<sf::Event::Resized>()) {
			blocksView.setSize({ 0.25f * window.getSize().x, 0.9f * window.getSize().y });
			blocksView.setCenter({ 0.25f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });

			raceView.setSize({ 0.75f * window.getSize().x, 0.9f * window.getSize().y });
			raceView.setCenter({ 0.75f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });
		}
		
		if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseButtonPressed->position, blocksView);
				bool textFieldActivated = false;
				for (auto &block : blocks) {
					if (block->name() == "StartBlock") {
						StartBlock* sb = dynamic_cast<StartBlock*> (block);
						if (sb->isMouseOver(worldPos) && (sb == activeStartBlock || !isRunning)) {
							bool chainState = sb->click(worldPos);
							if (chainState) {
								nextBlockToUpdate = sb;
								activeStartBlock = sb;
								isRunning = true;
							}
							else {
								reset();
							}
							break;
						}
					}
					else if (block->name() == "TimerBlock") {
						TimerBlock* tb = dynamic_cast<TimerBlock*> (block);
						TextField* tf = tb->onClick(worldPos);
						if (tf != nullptr) {
							if (activeTextField) {
								activeTextField->disable();
							}
							activeTextField = tf;
							textFieldActivated = true;
							break;
						}
					}
					if (block->isInBoundingBox(worldPos) && !isRunning) {
						startPos = worldPos;
						movingBlock = block;
					}
				}
				if (!textFieldActivated) {
					activeTextField = nullptr;
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
		if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left && !isRunning)
			{
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
		if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			if (!nextBlockToUpdate) {
				for (Block* block : blocks) {
					if (block->name() == "StartBlock") {
						StartBlock* sb = dynamic_cast<StartBlock*> (block);
						if (sb->isMouseOver(window.mapPixelToCoords(mouseMoved->position, blocksView))) {
							const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
							window.setMouseCursor(cursor);
						}
						else {
							const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
							window.setMouseCursor(cursor);
						}
						break;
					}
				}
				if (movingBlock != nullptr) {
					sf::FloatRect viewBounds({
						blocksView.getCenter().x - blocksView.getSize().x / 2,
						blocksView.getCenter().y - blocksView.getSize().y / 2 },
						{ blocksView.getSize().x, blocksView.getSize().y });

					sf::Vector2f worldPos = window.mapPixelToCoords(mouseMoved->position, blocksView);
					if (viewBounds.contains(worldPos)) {
						movingBlock->moveBy(worldPos - startPos);
						startPos = worldPos;
					}
				}
			}
		}
		if (const auto* textEntred = event->getIf<sf::Event::TextEntered>()) {
			if (activeTextField) {
				activeTextField->onTextInput(textEntred);
			}			
		}
	}
}

void Game::render()
{
    window.clear(sf::Color(100, 100, 100));

	window.setView(raceView);
	sf::RectangleShape raceBackground;
	raceBackground.setFillColor(sf::Color::White);
	raceBackground.setSize({ 1920, 1080 });
	window.draw(raceBackground);
	car.render();
	
	window.setView(blocksView);

	sf::RectangleShape blocksBackground;
	blocksBackground.setFillColor(sf::Color(200, 200, 200));
	blocksBackground.setSize({ 1920, 1080 });
	window.draw(blocksBackground);
	sf::RectangleShape storeBackground;
	storeBackground.setFillColor(sf::Color(150, 150, 150));
	storeBackground.setSize({ (float)blockStoreWidth, 1080 });
	window.draw(storeBackground);

	for (const auto& block : blocks) {
		block->render();
	}
	for (const auto& block : blockStore) {
		block->render();
	}

	window.setView(appView);

    window.display();
    
}

void Game::reset()
{
	car.reset();
	car.moveTo({ 0.75f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });
	nextBlockToUpdate = nullptr;
	activeStartBlock = nullptr;
	isRunning = false;
	for (const auto& block : blocks) {
		if (block->name() == "TimerBlock") {
			TimerBlock* tb = dynamic_cast<TimerBlock*>(block);
			tb->reset();
		}
	}
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
			for (auto& b : blocks) {
				b->blockInteract(block);
			}
			blocks.push_back(block);
		}
		
	}
	file.close();

}

void Game::update()
{
	sf::Time elapsed = clock.restart();
	if (nextBlockToUpdate) {
		nextBlockToUpdate = nextBlockToUpdate->update(car);
	}

	car.update(elapsed);
}
