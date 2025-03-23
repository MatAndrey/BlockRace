#include "Car.hpp"

void Car::render()
{
    sf::Texture texture(".\\assets\\images\\car.png");
    sf::Sprite sprite(texture);
    sprite.setScale({ 0.1, 0.1 });

    sprite.setRotation(direction);
    sprite.setPosition(pos);

    window->draw(sprite);
}

void Car::update(sf::Time elapsed) {
    float dt = elapsed.asSeconds() * 10;
    speed += acceleration * dt;
    if (abs(speed) <= 0.1) {
        speed = 0;
    }
    else if (speed > 0) {
        speed -= dt;
    }
    else {
        speed += dt;
    }

    if (abs(acceleration) <= 0.1) {
        acceleration = 0;
    }
    else if (acceleration > 0) {
        acceleration -= dt;
    }
    else {
        acceleration += dt;
    }

    direction += directionDelta;

    pos += speed * dt * sf::Vector2f{cos(direction.asRadians()), sin(direction.asRadians())};
}

void Car::accelerate(float _acceleration)
{
    acceleration = _acceleration;
}

void Car::setDirection(sf::Angle _dir)
{
    directionDelta = _dir;
}

Car::Car(sf::Vector2f _pos, sf::RenderWindow* window) : 
    Entity(_pos, window),
    speed(0), acceleration(0), direction(sf::degrees(-90)), directionDelta(sf::degrees(0))
{}

Car::~Car()
{
}
