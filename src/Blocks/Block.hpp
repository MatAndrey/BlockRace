#pragma once
#include <SFML/Graphics.hpp>

class Block {
protected:
	sf::Vector2f pos;
	sf::Vector2f size;
	Block* child;
	sf::RenderWindow* window;

public:
	Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window);
	virtual ~Block();

	virtual void render() = 0;
	bool isInBoundingBox(sf::Vector2f point);
	void move(sf::Vector2f delta);
};