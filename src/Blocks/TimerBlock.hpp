#pragma once
#include "Block.hpp"
#include "../UI/NumberField.hpp"

class TimerBlock : public Block
{
protected:
	Block* innerNextBlock;
	sf::Clock clock;
	sf::Time elapsedTime;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;
	sf::RectangleShape shape4;
	sf::RectangleShape shape5;
	sf::RectangleShape shape6;
	sf::RectangleShape shape7;
	sf::RectangleShape shape8;
	NumberField field;

	void resize();
	float getInnerHeight();
public:
	TimerBlock(sf::Vector2f _pos, sf::RenderWindow* window, float height = 90, double durationMcs = 1);
	~TimerBlock();
	virtual void moveBy(sf::Vector2f);
	virtual bool blockInteract(Block*);
	virtual TimerBlock* clone();
	virtual void render();
	virtual sf::Time update(Car& car, sf::Time elapsed);
	virtual bool isInBoundingBox(sf::Vector2f point);
	virtual std::string name();
	double getDuration();
	TextField* onClick(sf::Vector2f mousePos);
	void updateDuration();
	virtual void activate(Car& car);
	virtual void deactivate(Car& car);
};

