#pragma once
#include "Block.hpp"
#include "../UI/NumberField.hpp"

class ConstSpeedBlock : public Block {
	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;

	float targetSpeed;
	NumberField field;
	sf::View* view;
public:
	ConstSpeedBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, float targetSpeed = 0);
	~ConstSpeedBlock();
	virtual ConstSpeedBlock* clone();
	virtual void render();
	virtual std::string name();
	virtual void activate(Car& car);
	virtual void deactivate(Car& car);
	float getTargetSpeed();
	void updateTargetSpeed();
};