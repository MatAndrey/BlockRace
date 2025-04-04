#pragma once
#include "Entity.hpp"
#include <cmath>

class Car : public Entity
{
	bool acceleration = false;
	bool deceleration = false;
	float speed;
	
	sf::Angle directionDelta;
	const float maxSpeed = 30;
	const float friction = 0.1;

	sf::Texture texture;
	sf::Sprite sprite;

	void updatePhysics(float timeDelta);
public:
	virtual void render();
	sf::Angle direction;
	void reset(sf::Vector2f defaultPos, sf::Angle defaultDir);
	void update(sf::Time elapsed);
	void accelerate(bool state);
	void decelerate(bool state);
	void setDirection(sf::Angle _dir);
	const sf::Transform& getTransform() const;
	sf::FloatRect getGlobalBounds() const;
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};