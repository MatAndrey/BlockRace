#pragma once
#include "Block.hpp"
#include "../UI/NumberField.hpp"

class RotationBlock : public Block {
	sf::Angle direction;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;
	
	sf::View* view;
	NumberField field;
public:
	RotationBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, sf::Angle _dir);
	~RotationBlock();
	float getAngle();
	virtual RotationBlock* clone();
	virtual void render();
	virtual std::string name();
	virtual void activate(Car& car);
	virtual void deactivate(Car& car);
	void updateDirection();
};