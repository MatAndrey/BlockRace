#include "Car.hpp"

Car::Car(sf::RenderWindow* _window): window(window)
{
}

Car::~Car()
{
}

void Car::render()
{
	sf::Texture texture(".\\assets\\images\\car.png");
	sf::Sprite sprite(texture);

	sprite.setPosition({ 50, 50 });
	sprite.setTextureRect({ {50, 50}, {50, 50} });

	window->draw(sprite);
}
