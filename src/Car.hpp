#pragma once
#include "Entity.hpp"
#include <cmath>

class Car : public Entity
{
	float acceleration;
	float speed;
	sf::Angle direction;
	sf::Angle directionDelta;
	const float maxAcceleration = 30;
	const float maxSpeed = 20;

	sf::Texture texture;
	sf::Sprite sprite;
public:
	virtual void render();
	void reset();
	void update(sf::Time elapsed);
	void accelerate(float acceleration);
	void decelerate(float deceleration);
	void setDirection(sf::Angle _dir);
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};