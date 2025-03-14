#pragma once
#include "Block.hpp"

class TimerBlock : public Block
{
protected:
	Block* innerNextBlock;
	sf::Time timerDuration;

	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::RectangleShape shape3;
	sf::RectangleShape shape4;
	sf::RectangleShape shape5;
	sf::RectangleShape shape6;
	sf::RectangleShape shape7;
	sf::RectangleShape shape8;

	void resize();
	float getInnerHeight();
public:
	TimerBlock(sf::Vector2f _pos, sf::RenderWindow* window);
	~TimerBlock();
	virtual void moveBy(sf::Vector2f);
	virtual bool blockInteract(Block*);
	virtual TimerBlock* clone();
	virtual void render();
	virtual void update(Car& car);
	virtual bool isInBoundingBox(sf::Vector2f point);
};

