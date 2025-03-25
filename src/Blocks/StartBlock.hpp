#pragma once
#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include <vector>
#include "../UI/StartButton.hpp"

class StartBlock : public Block {
	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	StartButton button;
public:
	StartBlock(sf::Vector2f _pos, sf::RenderWindow* window);
	bool isMouseOver(sf::Vector2f pos);
	virtual ~StartBlock();
	virtual StartBlock* clone();
	virtual void render();
	virtual Block* update(Car& car);
	virtual std::string name();
	bool click(sf::Vector2f pos);
};