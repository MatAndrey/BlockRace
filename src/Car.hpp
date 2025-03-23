#pragma once
#include "Entity.hpp"

class Car : public Entity
{
	float acceleration;
	float speed;
	sf::Angle direction;
	sf::Angle directionDelta;
public:
	virtual void render();
	void update(sf::Time elapsed);
	void accelerate(float acceleration);
	void setDirection(sf::Angle);
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};