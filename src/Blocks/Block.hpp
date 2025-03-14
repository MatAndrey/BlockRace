#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.hpp"
#include "../Car.hpp"

class Block : public Entity {
protected:
	sf::VertexArray outline;
	sf::Font font;
	sf::Text text;
public:
	sf::Vector2f size;
	Block* nextBlock;
	bool canBeChild;

	Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window);
	virtual ~Block();

	virtual void moveBy(sf::Vector2f);
	float getChainHeight();
	virtual bool isInBoundingBox(sf::Vector2f point);
	virtual bool blockInteract(Block*);
	virtual Block* clone() = 0;
	virtual void render() = 0;
	virtual void update(Car& car) = 0;
};