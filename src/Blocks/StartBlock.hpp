#pragma once

#include "Block.hpp"
#include "../Car.hpp"

class StartBlock : protected Block {
protected:
	Car* car;
public:
	StartBlock(sf::Vector2f _pos, Car* car, sf::RenderWindow* _window);
	virtual void draw();
};