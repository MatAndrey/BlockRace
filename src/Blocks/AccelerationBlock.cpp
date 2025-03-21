#include "AccelerationBlock.hpp"
#include <iostream>

AccelerationBlock::AccelerationBlock(sf::Vector2f _pos, sf::RenderWindow* window):
	Block(_pos, {120, 30}, window), acceleration(1), shape1({20, 30}), shape2({25, 30}), shape3({75, 30})
{
    shape1.setFillColor(sf::Color(44, 71, 122));

    shape2.setFillColor(sf::Color(44, 71, 122));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(44, 71, 122));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"Ускорение");
    text.setCharacterSize(14);
    text.setPosition(sf::Vector2f{ 5, 8 });

    std::vector<sf::Vector2f> vertexes{
        {0, 0}, {20, 0}, {20, 5}, {45, 5}, {45, 0}, {120, 0}, {120, 30},
        {45, 30}, {45, 35}, {20, 35}, {20, 30}, {0, 30}, {0, 0}
    };
    outline.resize(vertexes.size());
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
        outline[i].color = sf::Color::White;
    }
}

AccelerationBlock::~AccelerationBlock()
{
}

AccelerationBlock* AccelerationBlock::clone()
{
    return new AccelerationBlock(pos, window);
}

void AccelerationBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(outline, transform);

    window->draw(text, transform);
}

Block* AccelerationBlock::update(Car& car)
{
    car.accelerate(acceleration);
	if (nextBlock) {
        return nextBlock;
    }
    else {
        return nullptr;
    }
}

std::string AccelerationBlock::name()
{
    return "AccelerationBlock";
}
