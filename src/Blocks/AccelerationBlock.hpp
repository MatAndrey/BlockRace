#pragma once
#include "Block.hpp"

class AccelerationBlock : public Block {
	float acceleration;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;
	
public:
	AccelerationBlock(sf::Vector2f _pos, sf::RenderWindow* window);
	~AccelerationBlock();
	virtual AccelerationBlock* clone();
	virtual void render();
	virtual Block* update(Car& car);
	virtual std::string name();
	virtual bool handleEvent(sf::Event& event, sf::Vector2f mousePos);
};