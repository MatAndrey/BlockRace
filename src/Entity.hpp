#pragma once
#include <SFML/Graphics.hpp>

class Entity {
protected:
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::RenderWindow* window;
public:
	virtual void render() = 0;
	bool isInBoundingBox(sf::Vector2f point);
	void moveBy(sf::Vector2f delta);
	void moveTo(sf::Vector2f newPos);

	Entity(sf::Vector2f _pos, sf::RenderWindow* window);
	~Entity();
};