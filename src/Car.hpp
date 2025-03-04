#pragma once
#include "Entity.hpp"

class Car : public Entity
{
	float acceleration;
	float speed;
	sf::Angle direction;
public:
	virtual void render();
	void update(sf::Time elapsed);
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};