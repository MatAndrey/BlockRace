#include "RotationBlock.hpp"

RotationBlock::RotationBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, sf::Angle _dir) :
    Block(_pos, { 120, 30 }, window), direction(_dir), shape1({ 20, 30 }), shape2({ 25, 30 }), shape3({ 75, 30 }),
    view(view), field(pos, window, { 27, 16 })
{
    field.setView(view);
    field.setText(std::to_string(direction.asDegrees()));

    shape1.setFillColor(sf::Color(44, 71, 122));

    shape2.setFillColor(sf::Color(44, 71, 122));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(44, 71, 122));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"Повернуть на");
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

RotationBlock::~RotationBlock()
{
}

float RotationBlock::getAngle()
{
    updateDirection();
    return direction.asDegrees();
}

RotationBlock* RotationBlock::clone()
{
    return new RotationBlock(pos, window, view, direction);
}

void RotationBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(outline, transform);

    window->draw(text, transform);

    field.pos = pos + sf::Vector2f{ 88, 7 };
    field.render();
}

std::string RotationBlock::name()
{
    return "RotationBlock";
}

void RotationBlock::activate(Car& car)
{
    if (!isRunning) {
        updateDirection();
        Block::activate(car);
        car.setDirectionDelta(direction);
    }    
}

void RotationBlock::deactivate(Car& car)
{
    if (isRunning) {
        Block::deactivate(car);
        car.setDirectionDelta(sf::degrees(0));
    }    
}

void RotationBlock::updateDirection()
{
    try {
        direction = sf::degrees(std::stof(field.getText()));
    }
    catch (...) {
        direction = sf::degrees(0);
        field.setText("0");
    }
}
