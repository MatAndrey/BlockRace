#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include <vector>

class StartBlock : public Block {
	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
public:
	StartBlock(sf::Vector2f _pos, sf::RenderWindow* window);
	virtual ~StartBlock();
	virtual StartBlock* clone();
	virtual void render();
	virtual void update(Car& car);
	virtual std::string name();
};