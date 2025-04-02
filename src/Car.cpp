#include "Car.hpp"
#include <iostream>

void Car::render()
{
    sprite.setRotation(direction);
    sprite.setPosition(sf::Vector2f(pos));
    window->draw(sprite);
}

void Car::reset()
{
    acceleration = 0;
    speed = 0;
    pos = {1000, 300};
    direction = sf::degrees(180);
    directionDelta = sf::degrees(0);
}

void Car::update(sf::Time elapsed) {
    const float fixed_dt = 1.0f / 60.0f;
    float dt = elapsed.asSeconds() * 10;
    while (dt > fixed_dt) {
        updatePhysics(fixed_dt);
        dt -= fixed_dt;
    }
    if (dt > 0) {
        updatePhysics(dt);
    }
}

void Car::updatePhysics(float dt) {
    if (acceleration) {
        speed += 0.1;
    }
    if (abs(speed) > maxSpeed) {
        speed = abs(speed) / speed * maxSpeed;
    }
    speed *= std::exp(-friction * dt * (deceleration ? 8 : 1));
 

    if (std::abs(speed) < 0.05f) {
        speed = 0.0f;
    }

    if (abs(speed) > 2) {
        direction += directionDelta * dt;
    }    
    float angle = direction.asRadians();

    double dx = std::cos(angle) * speed * dt;
    double dy = std::sin(angle) * speed * dt;

    pos.x += dx;
    pos.y += dy;
}

void Car::accelerate(bool state)
{
    acceleration = state;
}

void Car::decelerate(bool state)
{
    deceleration = state;
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
