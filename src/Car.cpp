#include "Car.hpp"
#include <iostream>

void Car::render()
{
    sprite.setRotation(direction);
    sprite.setPosition(pos);
    window->draw(sprite);
}

void Car::reset()
{
    acceleration = 0;
    speed = 0;
    direction = sf::degrees(-90);
    directionDelta = sf::degrees(0);
}

void Car::update(sf::Time elapsed) {
    float dt = elapsed.asSeconds() * 10;
    const float max_dt = 0.16f;
    if (dt > max_dt) {
        dt = max_dt;
    }
    float friction = 5.0f;

    speed += acceleration * dt;
    if (acceleration > 0) {
        acceleration -= dt;
        if (acceleration < 0)
            acceleration = 0;
    }
    else if (acceleration < 0) {
        acceleration += dt;
        if (acceleration > 0)
            acceleration = 0;
    }
    if (acceleration == 0.0f) {
        if (speed > 0.0f) {
            speed = std::max(0.0f, speed - friction * dt);
        }
        else if (speed < 0.0f) {
            speed = std::min(0.0f, speed + friction * dt);
        }
    }

    if (std::abs(speed) < 0.1f) {
        speed = 0.0f;
    }
    if (std::abs(speed) > maxSpeed) {
        speed = (speed > 0) ? maxSpeed : -maxSpeed;
    }
    else if (acceleration > 0) {
        acceleration -= dt;
    }

    direction += directionDelta * speed / 50;
    float angle = direction.asRadians();
    sf::Vector2f velocity = sf::Vector2f(std::cos(angle), std::sin(angle)) * speed * dt;
    pos += velocity;
}

void Car::accelerate(float _acceleration)
{
    acceleration += _acceleration;
    if (acceleration > maxAcceleration) {
        acceleration = maxAcceleration;
    }
    if (acceleration < -maxAcceleration) {
        acceleration = -maxAcceleration;
    }
}

void Car::decelerate(float _deceleration)
{
    acceleration -= _deceleration;
    if (acceleration < 0) {
        acceleration = 0;
    }
}

void Car::setDirection(sf::Angle _dir)
{
    directionDelta = _dir;
}

Car::Car(sf::Vector2f _pos, sf::RenderWindow* window) : 
    Entity(_pos, window),
    speed(0), acceleration(0), direction(sf::degrees(-90)), directionDelta(sf::degrees(0)),
    texture(".\\assets\\images\\car.png"), sprite(texture)
{
    sprite.setScale({ 0.1, 0.1 });
}

Car::~Car()
{
}
