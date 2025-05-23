#include "DecelerationBlock.hpp"

DecelerationBlock::DecelerationBlock(sf::Vector2f _pos, sf::RenderWindow* window) :
    Block(_pos, { 120, 30 }, window), deceleration(1), shape1({ 20, 30 }), shape2({ 25, 30 }), shape3({ 75, 30 })
{
    shape1.setFillColor(sf::Color(44, 71, 122));

    shape2.setFillColor(sf::Color(44, 71, 122));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(44, 71, 122));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"Торможение");
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

DecelerationBlock::~DecelerationBlock()
{
}

DecelerationBlock* DecelerationBlock::clone()
{
    return new DecelerationBlock(pos, window);
}

void DecelerationBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(outline, transform);

    window->draw(text, transform);
}

void DecelerationBlock::activate(Car& car)
{
    if (!isRunning) {
        Block::activate(car);
        car.decelerate(true);
    }    
}

void DecelerationBlock::deactivate(Car& car)
{
    if (isRunning) {
        Block::deactivate(car);
        car.decelerate(false);
    }    
}

std::string DecelerationBlock::name()
{
    return "DecelerationBlock";
}
