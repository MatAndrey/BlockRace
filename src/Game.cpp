#include "Game.hpp"
#include <iostream>

Game::Game() :
	initHeight(1080),
	initWidth(1920),
	blockStoreWidth(150),
	car({ 10, 10 }, & window),
	nextBlockToUpdate(nullptr)
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
				leftHold = true;
				sf::Vector2f worldPos = window.mapPixelToCoords(mouseButtonPressed->position, blocksView);
				for (auto block : blocks) {
					if (block->name() == "StartBlock") {
						StartBlock* sb = dynamic_cast<StartBlock*> (block);
						if (sb->isMouseOver(worldPos)) {
							bool chainState = sb->click(worldPos);
							if (chainState) {
								nextBlockToUpdate = sb;
							}
							else {
								reset();
							}
							break;
						}
					}
					if (block->isInBoundingBox(worldPos) && !nextBlockToUpdate) {
						startPos = worldPos;
						activeBlock = block;
					}
				}
				if (!nextBlockToUpdate) {
					for (auto block : blockStore) {
						if (block->isInBoundingBox(worldPos)) {
							startPos = worldPos;
							activeBlock = block->clone();
							blocks.push_back(activeBlock);
						}
					}
				}				
			}
		}
		if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left && !nextBlockToUpdate)
			{
				for (auto iter = blocks.begin(); iter != blocks.end();) {
					Block* block = *iter;

					if (block->pos.x + block->size.x < blockStoreWidth) {
						if (block->name() != "StartBlock") {
							iter = blocks.erase(iter);
							delete block;
							continue;
						}
						else {
							block->nextBlock = nullptr;
							iter++;
						}						
					}
					else {
						iter++;
					}
					if (block->blockInteract(activeBlock)) {
						break;
					}
				}

				leftHold = false;
				activeBlock = nullptr;

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
				if (leftHold && activeBlock != nullptr) {
					sf::FloatRect viewBounds({
						blocksView.getCenter().x - blocksView.getSize().x / 2,
						blocksView.getCenter().y - blocksView.getSize().y / 2 },
						{ blocksView.getSize().x, blocksView.getSize().y });

					sf::Vector2f worldPos = window.mapPixelToCoords(mouseMoved->position, blocksView);
					if (viewBounds.contains(worldPos)) {
						activeBlock->moveBy(worldPos - startPos);
						startPos = worldPos;
					}
				}
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
}

void Game::saveToFile()
{
	std::ofstream file("save.dat");
	for (const auto& block : blocks) {
		file << block->name() << "\t" << block->pos.x << "\t" << block->pos.y << "\n";
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
			block = new TimerBlock(pos, &window);
		}
		if (block) {
			for (auto b : blocks) {
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
