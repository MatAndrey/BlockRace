#pragma once
#include <SFML/Graphics.hpp>

class Car
{
public:
	Car(sf::RenderWindow* _window);
	~Car();
	void render();

protected:
	sf::RenderWindow* window;
};