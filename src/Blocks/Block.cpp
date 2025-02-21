#include "Block.hpp"

Block::Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window) : pos(_pos), size(_size), child(nullptr), window(window) {}

Block::~Block() {}

bool Block::isInBoundingBox(sf::Vector2f point) {
    sf::FloatRect rect (pos, size);
    return rect.contains(point);
}

void Block::move(sf::Vector2f delta)
{
    pos += delta;
}
