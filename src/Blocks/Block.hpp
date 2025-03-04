#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.hpp"

class Block : public Entity {
protected:
	Block* child;
	sf::Vector2f size;
public:
	Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window);
	virtual ~Block();

	virtual void render() = 0;
};