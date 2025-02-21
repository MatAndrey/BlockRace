#pragma once

#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "../Car.hpp"

class StartBlock : public Block {
protected:
	Car* car;
public:
	StartBlock(sf::Vector2f _pos, Car* car, sf::RenderWindow* window);
	virtual ~StartBlock();
	virtual void render();
};