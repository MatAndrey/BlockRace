#include "Car.hpp"
#include <iostream>

void Car::render()
{
    sf::Texture texture(".\\assets\\images\\car.png");
    sf::Sprite sprite(texture);
    sprite.setScale({ 0.1, 0.1 });

    sprite.setRotation(direction);
    sprite.setPosition(pos);

    window->draw(sprite);
}

void Car::reset()
{
    acceleration = 0;
    speed = 0;
    direction = sf::degrees(-90);
}

void Car::update(sf::Time elapsed) {
    float dt = elapsed.asSeconds() * 10;
    float friction = 1.0f;

    speed += acceleration * dt;
    if (acceleration > 0) {
        acceleration -= dt;
    }
    else if (acceleration < 0) {
        acceleration += dt;
    }
    if (abs(acceleration) < 0.1f) {
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
    if (speed > maxSpeed) {
        speed = maxSpeed;
    }
    if (speed < -maxSpeed) {
        speed = -maxSpeed;
    }

    float angle = direction.asRadians();
    sf::Vector2f velocity = sf::Vector2f(std::cos(angle), std::sin(angle)) * speed * dt;
    pos += velocity;

    if (std::abs(acceleration) < 0.1f) {
        acceleration = 0.0f;
    }
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

Car::Car(sf::Vector2f _pos, sf::RenderWindow* window) : 
    Entity(_pos, window),
    speed(0), acceleration(0), direction(sf::degrees(-90))
{}

Car::~Car()
{
}
