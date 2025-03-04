#include "Entity.hpp"

Entity::Entity(sf::Vector2f _pos, sf::RenderWindow* window) : pos(_pos), window(window) {}

Entity::~Entity()
{
}

bool Entity::isInBoundingBox(sf::Vector2f point) {
    sf::FloatRect rect(pos, size);
    return rect.contains(point);
}

void Entity::moveBy(sf::Vector2f delta)
{
    pos += delta;
}

void Entity::moveTo(sf::Vector2f newPos) {
    pos = newPos;
}