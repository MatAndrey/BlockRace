#pragma once
#include "Block.hpp"

class RotationBlock : public Block {
	sf::Angle direction;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;

public:
	RotationBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::Angle _dir);
	~RotationBlock();
	float getAngle();
	virtual RotationBlock* clone();
	virtual void render();
	virtual Block* update(Car& car);
	virtual std::string name();
};