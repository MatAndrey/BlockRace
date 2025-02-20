#include <SFML/Graphics.hpp>
#include "Blocks/StartBlock.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "SFML works!");


    Car car;
    StartBlock start({ 10, 10 }, &car, &window);


    while (window.isOpen())
    {
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


        window.clear(sf::Color(100, 100, 100));
        start.draw();
        window.display();
    }
}