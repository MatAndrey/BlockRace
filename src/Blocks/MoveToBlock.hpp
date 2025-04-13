#pragma once
#include "Block.hpp"
#include <cmath>
#include "../UI/NumberField.hpp"

class MoveToBlock : public Block {
	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;
	sf::View* view;

	sf::Vector2f targetPos;
	NumberField fieldX;
	NumberField fieldY;
public:
	MoveToBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, sf::Vector2f targetPos = {0, 0});
	~MoveToBlock();
	virtual MoveToBlock* clone();
	virtual void render();
	virtual std::string name();
	virtual void activate(Car& car);
	virtual void deactivate(Car& car);
	void updateTarget();
	sf::Vector2f getTargetPos();
};