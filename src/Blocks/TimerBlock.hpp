#pragma once
#include "Block.hpp"
#include "../UI/NumberField.hpp"

class TimerBlock : public Block
{
protected:
	Block* innerNextBlock;
	sf::Clock clock;
	sf::Time elapsedTime;
	sf::View* view;

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
	TimerBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, float height = 90, double durationSecs = 1);
	~TimerBlock();
	virtual void moveBy(sf::Vector2f);
	virtual bool blockInteract(Block*, bool disconeting = true);
	virtual TimerBlock* clone();
	virtual void render();
	virtual bool isInBoundingBox(sf::Vector2f point);
	virtual std::string name();
	double getDuration();
	void updateDuration();
	virtual void activate(Car& car);
	virtual void deactivate(Car& car);
};

