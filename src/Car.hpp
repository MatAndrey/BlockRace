#pragma once
#include "Entity.hpp"
#include <cmath>

class Car : public Entity
{
	float acceleration;
	float speed;
	sf::Angle direction;

	const float maxAcceleration = 10;
	const float maxSpeed = 10;
public:
	virtual void render();
	void reset();
	void update(sf::Time elapsed);
	void accelerate(float acceleration);
	void decelerate(float deceleration);
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};