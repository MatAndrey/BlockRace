#pragma once
#include "Block.hpp"

class DecelerationBlock : public Block {
	float deceleration;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;

public:
	DecelerationBlock(sf::Vector2f _pos, sf::RenderWindow* window);
	~DecelerationBlock();
	virtual DecelerationBlock* clone();
	virtual void render();
	virtual Block* update(Car& car);
	virtual std::string name();
};