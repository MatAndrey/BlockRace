#pragma once
#include "Entity.hpp"
#include <cmath>

class Car : public Entity
{
	bool acceleration = false;
	bool deceleration = false;
	float speed;
	
	sf::Angle directionDelta;
	const float maxSpeed = 100;

	sf::Texture texture;
	sf::Sprite sprite;

public:
	virtual void render(sf::Vector2f interpolatedPos);
	virtual void render() {};
	sf::Angle direction;
	sf::Vector2f prevPos;
	void reset(sf::Vector2f defaultPos, sf::Angle defaultDir);
	void update(sf::Time dt);
	void accelerate(bool state);
	void decelerate(bool state);
	void setDirection(sf::Angle _dir);
	const sf::Transform& getTransform() const;
	sf::FloatRect getGlobalBounds() const;
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};