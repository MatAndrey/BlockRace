#include "MoveToBlock.hpp"
#include <iostream>

MoveToBlock::MoveToBlock(sf::Vector2f _pos, sf::RenderWindow* window, sf::View* view, sf::Vector2f targetPos) :
    Block(_pos, { 120, 30 }, window), view(view),
    shape1({ 20, 30 }), shape2({ 25, 30 }), shape3({ 105, 30 }), 
    targetPos(targetPos),
    fieldX(pos, window, { 30, 16 }), fieldY(pos, window, { 30, 16 })
{
    timeToWork = sf::seconds(600);
    fieldX.setText(std::to_string(targetPos.x));
    fieldX.setView(view);

    fieldY.setText(std::to_string(targetPos.y));
    fieldY.setView(view);

    shape1.setFillColor(sf::Color(122, 44, 44));

    shape2.setFillColor(sf::Color(122, 44, 44));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(122, 44, 44));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"≈хать к X:         Y:");
    text.setCharacterSize(14);
    text.setPosition(sf::Vector2f{ 5, 8 });

    std::vector<sf::Vector2f> vertexes{
        {0, 0}, {20, 0}, {20, 5}, {45, 5}, {45, 0}, {150, 0}, {150, 30},
        {45, 30}, {45, 35}, {20, 35}, {20, 30}, {0, 30}, {0, 0}
    };
    outline.resize(vertexes.size());
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
        outline[i].color = sf::Color::White;
    }

    EventBus::get().subscribe<SetTargetEvent>(this, &MoveToBlock::onSetTarget);
}

MoveToBlock::~MoveToBlock()
{
    EventBus::get().unsubscribe<SetTargetEvent>(this);
}

MoveToBlock* MoveToBlock::clone()
{
    return new MoveToBlock(pos, window, view, targetPos);
}

void MoveToBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(outline, transform);

    window->draw(text, transform);

    fieldX.pos = pos + sf::Vector2f{ 65, 7 };
    fieldX.render();

    fieldY.pos = pos + sf::Vector2f{ 112, 7 };
    fieldY.render();
}

std::string MoveToBlock::name()
{
    return "MoveToBlock";
}

void MoveToBlock::activate(Car& car)
{
    if (!isRunning) {
        updateTarget();
        Block::activate(car);
        car.setSpeedLimit(20);
    }

    float distanceToTarget = (targetPos - car.pos).length();

    float angle = std::atan2(car.pos.y - targetPos.y, car.pos.x - targetPos.x);
    sf::Angle targetAngle = sf::radians(angle + 3.14159265358979323846f);
    car.setDirectionDelta((targetAngle - car.direction).wrapSigned());

    if (distanceToTarget > 80) {
        car.accelerate(true);
        car.decelerate(false);
    }
    else {
        car.accelerate(false);
    }

    if (distanceToTarget < 1) {
        car.decelerate(true);
        deactivate(car);
    }
}

void MoveToBlock::deactivate(Car& car)
{
    if (isRunning) {
        Block::deactivate(car);
        car.setSpeedLimit(-1);
        car.setDirectionDelta(sf::degrees(0));
        car.accelerate(false);
    }
}

void MoveToBlock::updateTarget()
{
    try {
        targetPos.x = std::stof(fieldX.getText());
    }
    catch (...) {
        targetPos.x = 0;
        fieldX.setText("0");
    }
    try {
        targetPos.y = std::stof(fieldY.getText());
    }
    catch (...) {
        targetPos.y = 0;
        fieldY.setText("0");
    }
}

sf::Vector2f MoveToBlock::getTargetPos()
{
    updateTarget();
    return targetPos;
}

void MoveToBlock::onSetTarget(const SetTargetEvent& event)
{
    if (isInBoundingBox(event.eventPos)) {
        targetPos = event.targetPos;
        fieldX.setText(std::to_string(targetPos.x));
        fieldY.setText(std::to_string(targetPos.y));
    }    
}
