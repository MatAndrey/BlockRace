#include "Block.hpp"
#include <iostream>

Block::Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window) :
    Entity(_pos, window), size(_size), nextBlock(nullptr), outline(sf::PrimitiveType::LineStrip),
    font(".\\assets\\fonts\\Share-Tech-CYR.otf"), text(font), canBeChild(true), prevBlock(nullptr),
    timeToWork(sf::milliseconds(500))
{}

Block::~Block() {}

bool Block::isInBoundingBox(sf::Vector2f point) {
    sf::FloatRect rect(pos, size);
    return rect.contains(point);
}

bool Block::blockInteract(Block* other, bool disconneting)
{
    if (other == this) return false;
    if (other == nullptr) return false;

    if (nextBlock == other && disconneting && (pos.x - other->pos.x != 0 && pos.y + size.y - other->pos.y != 0)) {
        if (other->prevBlock) {
            other->prevBlock->nextBlock = nullptr;
            other->prevBlock = nullptr;
        }
        nextBlock = nullptr;
        return true;
    }
    if (nextBlock != other) {
        if (nextBlock == nullptr && other->canBeChild && (sf::Vector2f(pos.x - other->pos.x, pos.y + size.y - other->pos.y)).length() < interactionRadius) {
            if (other->prevBlock) {
                other->prevBlock->nextBlock = nullptr;
            }
            nextBlock = other;
            other->prevBlock = this;
            other->moveBy(sf::Vector2f(pos.x, pos.y + size.y) - other->pos);
            return true;
        }
        if (other->nextBlock == nullptr && canBeChild && (sf::Vector2f(pos.x - other->pos.x, other->pos.y + other->size.y - pos.y)).length() < interactionRadius) {
            other->nextBlock = this;
            prevBlock = other;
            moveBy(sf::Vector2f(other->pos.x, other->pos.y + other->size.y) - pos);
            return true;
        }
    }
    return false;
}

void Block::update(Car& car, sf::Time dt)
{
    elapsedTime += dt;
    if (elapsedTime < timeToWork) {
        activate(car);     
    }
    else {
        deactivate(car);
    }
}

Block* Block::getNext()
{
    return nextBlock;
}

void Block::activate(Car& car)
{
    if (!isRunning) {
        isRunning = true;
        elapsedTime = sf::seconds(0);
        for (int i = 0; i < outline.getVertexCount(); i++) {
            outline[i].color = sf::Color::Blue;
        }
    }    
}

void Block::deactivate(Car& car)
{
    isRunning = false;
    elapsedTime = sf::seconds(0);
    for (int i = 0; i < outline.getVertexCount(); i++) {
        outline[i].color = sf::Color::White;
    }
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
