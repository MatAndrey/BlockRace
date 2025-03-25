#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Entity.hpp"
#include "../Car.hpp"

class Block : public Entity {
protected:
	sf::VertexArray outline;
	sf::Font font;
	sf::Text text;
	static const short interactionRadius = 10;
	bool isMoving = false;
public:
	sf::Vector2f size;
	Block* nextBlock;
	Block* prevBlock;
	bool canBeChild;

	Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window);
	virtual ~Block();

	virtual void moveBy(sf::Vector2f);
	float getChainHeight();
	virtual bool isInBoundingBox(sf::Vector2f point);
	virtual bool blockInteract(Block*);
	virtual Block* clone() = 0;
	virtual void render() = 0;
	virtual Block* update(Car& car) = 0;
	virtual std::string name() = 0;
};