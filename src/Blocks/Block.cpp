#include "Block.hpp"
#include <iostream>

Block::Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window) :
    Entity(_pos, window), size(_size), nextBlock(nullptr), outline(sf::PrimitiveType::LineStrip),
    font(".\\assets\\fonts\\Share-Tech-CYR.otf"), text(font), canBeChild(true)
{}

Block::~Block() {}

bool Block::isInBoundingBox(sf::Vector2f point) {
    sf::FloatRect rect(pos, size);
    return rect.contains(point);
}

bool Block::blockInteract(Block* other)
{
    if (other == this) return false;
    if (other == nullptr) return false;
    
    if (nextBlock == other) {
        nextBlock = nullptr;
    }
    if (nextBlock != other) {
        if (nextBlock == nullptr && other->canBeChild && (sf::Vector2f(pos.x - other->pos.x, pos.y + size.y - other->pos.y)).length() < interactionRadius) {
            nextBlock = other;
            other->moveBy(sf::Vector2f(pos.x, pos.y + size.y) - other->pos);
            return true;
        }
        if (other->nextBlock == nullptr && canBeChild && (sf::Vector2f(pos.x - other->pos.x, other->pos.y + other->size.y - pos.y)).length() < interactionRadius) {
            other->nextBlock = this;
            moveBy(sf::Vector2f(other->pos.x, other->pos.y + other->size.y) - pos);
            return true;
        }
    }
    return false;
}

void Block::moveBy(sf::Vector2f delta)
{
    Entity::moveBy(delta);
    if (nextBlock) {
        nextBlock->moveBy(delta);
    }
}

float Block::getChainHeight()
{
    if (nextBlock) {
        return nextBlock->getChainHeight() + size.y;
    }
    else {
        return size.y;
    }
}
