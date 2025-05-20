#pragma once
#include "Entity.hpp"
#include <cmath>
#include "Particles/SmokeSystem.hpp"

class Car : public Entity
{
	bool acceleration = false;
	bool deceleration = false;
	float speed;
	
	const float maxDirDeltaDeg = 15;
	sf::Angle directionDelta;
	const float maxSpeed = 100;
	float speedLimit = -1;

	sf::Texture texture;
	sf::Sprite sprite;

	SmokeSystem smoke;

public:
	sf::Angle direction;
	sf::Vector2f prevPos;

	virtual void render(sf::Vector2f interpolatedPos);
	virtual void render() {};
	void reset(sf::Vector2f defaultPos, sf::Angle defaultDir);
	void update(sf::Time dt);
	void accelerate(bool state);
	void decelerate(bool state);
	void setDirectionDelta(sf::Angle _dir);
	sf::Angle getDirectionDelta();
	void setSpeedLimit(float newLimit);
	const sf::Transform& getTransform() const;
	sf::FloatRect getGlobalBounds() const;
	Car(sf::Vector2f _pos, sf::RenderWindow* window);
	~Car();
};