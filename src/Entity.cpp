#include "Entity.hpp"

Entity::Entity(sf::Vector2f _pos, sf::RenderWindow* window) : pos(_pos), window(window) {}

Entity::~Entity()
{
}

void Entity::moveBy(sf::Vector2f delta)
{
    pos += delta;
}

void Entity::moveTo(sf::Vector2f newPos) {
    pos = newPos;
}