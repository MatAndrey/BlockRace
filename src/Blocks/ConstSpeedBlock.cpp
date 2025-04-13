#include "ConstSpeedBlock.hpp"
#include <iostream>

ConstSpeedBlock::ConstSpeedBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, float targetSpeed) :
    Block(_pos, { 120, 30 }, window), shape1({ 20, 30 }), shape2({ 25, 30 }), shape3({ 75, 30 }),
    targetSpeed(targetSpeed), field(pos, window, { 28, 16 }), view(view)
{
    field.setText(std::to_string(targetSpeed));
    field.setView(view);

    shape1.setFillColor(sf::Color(122, 44, 44));

    shape2.setFillColor(sf::Color(122, 44, 44));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(122, 44, 44));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"Со скоростью");
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

ConstSpeedBlock::~ConstSpeedBlock()
{
}

ConstSpeedBlock* ConstSpeedBlock::clone()
{
    return new ConstSpeedBlock(pos, window, view, targetSpeed);
}

void ConstSpeedBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(outline, transform);

    window->draw(text, transform);

    field.pos = pos + sf::Vector2f{ 87, 7 };
    field.render();
}

std::string ConstSpeedBlock::name()
{
    return "ConstSpeedBlock";
}

void ConstSpeedBlock::activate(Car& car)
{
    if (!isRunning) {
        updateTargetSpeed();
        Block::activate(car);
        car.setSpeedLimit(targetSpeed);
        car.accelerate(true);
        car.decelerate(false);
    }
}

void ConstSpeedBlock::deactivate(Car& car)
{
    if (isRunning) {
        Block::deactivate(car);
        car.setSpeedLimit(-1);
        car.accelerate(false);
    }
}

float ConstSpeedBlock::getTargetSpeed()
{
    updateTargetSpeed();
    return targetSpeed;
}

void ConstSpeedBlock::updateTargetSpeed()
{
    try {
        targetSpeed = std::stof(field.getText());
    }
    catch (...) {
        targetSpeed = 0;
        field.setText("0");
    }
}
