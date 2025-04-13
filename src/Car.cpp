#include "Car.hpp"
#include <iostream>

void Car::render(sf::Vector2f interpolatedPos)
{
    sf::Transform transform;
    transform.translate(interpolatedPos);
    transform.rotate(direction);
    window->draw(sprite, transform);
}

void Car::reset(sf::Vector2f defaultPos, sf::Angle defaultDir)
{
    pos = defaultPos;
    direction = defaultDir;
    acceleration = 0;
    speed = 0;
    directionDelta = sf::degrees(0);
}

void Car::update(sf::Time deltaTime) {
    prevPos = pos;
    double dt = deltaTime.asSeconds() * 10;
    if (acceleration) {
        speed += 5 * dt;
    }
    if (std::abs(speed) > 0) {
        double delta = (deceleration ? 15 : 2) * dt;
        if (speed > 0) {
            if (speed - delta > 0) {
                speed -= delta;
            }
            else {
                speed = 0;
            }
        }
        else {
            if (speed + delta < 0) {
                speed += delta;
            }
            else {
                speed = 0;
            }
        }
    }
    if (abs(speed) > speedLimit && speedLimit > -1) {
        speed = abs(speed) / speed * speedLimit;
    }
    if (abs(speed) > maxSpeed) {
        speed = abs(speed) / speed * maxSpeed;
    } 

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

void Car::setDirectionDelta(sf::Angle _dir)
{
    float degrees = _dir.asDegrees();
    if (degrees > maxDirDeltaDeg) {
        directionDelta = sf::degrees(maxDirDeltaDeg);
    }
    else if (degrees < -maxDirDeltaDeg) {
        directionDelta = sf::degrees(-maxDirDeltaDeg);
    }
    else {
        directionDelta = _dir;
    }
}

sf::Angle Car::getDirectionDelta()
{
    return directionDelta;
}

void Car::setSpeedLimit(float newLimit)
{
    speedLimit = newLimit;
}

const sf::Transform& Car::getTransform() const
{
    sf::Transform t;
    t.translate(pos);
    t.rotate(direction);
    return t;
}

sf::FloatRect Car::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}


Car::Car(sf::Vector2f _pos, sf::RenderWindow* window) :
    Entity(_pos, window),
    speed(0), acceleration(0), direction(sf::degrees(-90)), directionDelta(sf::degrees(0)),
    texture(".\\assets\\images\\car.png"), sprite(texture)
{
    sprite.setScale({ 0.1, 0.1 });
    sprite.setOrigin(sprite.getLocalBounds().size / 2.0f);
}

Car::~Car()
{
}
