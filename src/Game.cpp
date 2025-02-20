#include "Game.hpp"  

Game::Game()  
{  
   window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "Block Race");  

   Car car;
   startBlock = new StartBlock(sf::Vector2f(0, 0), &car, &window);
   blocks.push_back(startBlock);
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
       draw();        
   }  
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            window.setSize(resized->size);
        }
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
            {
                window.close();
            }
        }
    }
}

void Game::draw()
{
    window.clear(sf::Color(100, 100, 100));
    for (const auto& block : blocks) {
            block->draw();
    }
    window.display();
    
}

void Game::update()
{
	// TODO
}
