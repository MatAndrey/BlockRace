#include "Game.hpp"

Game::Game() :
	initHeight(1080),
	initWidth(1920),
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
	
	startBlock = new StartBlock(sf::Vector2f(10, 10), &window);
	blocks.push_back(startBlock);

	blocks.push_back(new TimerBlock(sf::Vector2f(10, 50), &window));
	blocks.push_back(new AccelerationBlock(sf::Vector2f(10, 100), &window));

	car.moveTo({ 0.75f * window.getSize().x / 2, 0.9f * window.getSize().y / 2 });
}

Game::~Game()
{
	for (const auto& block : blocks) {
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
					if (block->isInBoundingBox(worldPos)) {					
						startPos = worldPos;
						activeBlock = block;
					}
				}
			}
		}
		if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
			{
				for (Block* block : blocks) {
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
			if (leftHold && activeBlock != nullptr) {
				sf::FloatRect viewBounds({
					blocksView.getCenter().x - blocksView.getSize().x / 2,
					blocksView.getCenter().y - blocksView.getSize().y / 2 },
					{blocksView.getSize().x, blocksView.getSize().y});

				sf::Vector2f worldPos = window.mapPixelToCoords(mouseMoved->position, blocksView);
				if (viewBounds.contains(worldPos)) {
					activeBlock->moveBy(worldPos - startPos);
					startPos = worldPos;
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
	for (const auto& block : blocks) {
		block->render();
	}

	window.setView(appView);

    window.display();
    
}

void Game::update()
{
	sf::Time elapsed = clock.restart();
	car.update(elapsed);
}
