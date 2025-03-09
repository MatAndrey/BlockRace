#pragma once
#include <SFML/Graphics.hpp>

class Entity {
protected:
	sf::RenderWindow* window;
public:
	sf::Vector2f pos;
	virtual void render() = 0;
	void moveBy(sf::Vector2f delta);
	void moveTo(sf::Vector2f newPos);

	Entity(sf::Vector2f _pos, sf::RenderWindow* window);
	~Entity();
};