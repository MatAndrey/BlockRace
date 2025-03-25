#include "TimerBlock.hpp"
#include <iostream>

void TimerBlock::render()
{
    sf::Transform transform;
    transform.translate(pos);

    if (getInnerHeight() + 90 != size.y ) {
        resize();
    }

    window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(shape3, transform);
    window->draw(shape4, transform);
    window->draw(shape5, transform);
    window->draw(shape6, transform);
    window->draw(shape7, transform);
    window->draw(shape8, transform);
    window->draw(outline, transform);

    window->draw(text, transform);

    field.pos = pos + sf::Vector2f{ 5, 5 };
    field.render();
}

void TimerBlock::resize()
{
    float offset = getInnerHeight() + 60;
    size.y = offset + 30;
    shape1.setSize({ 20, size.y });
    shape5.setPosition({ 20, offset });
    shape6.setPosition({ 40, offset + 5 });
    shape7.setPosition({ 65, offset });
    shape8.setPosition({ 20, offset + 30 });

    std::vector<sf::Vector2f> vertexes{
        {0, 0}, {20, 0}, {20, 5}, {45, 5}, {45, 0}, {120, 0}, {120, 30}, {65, 30}, {65, 35},
        {40, 35}, {40, 30}, {20, 30}, {20, offset}, {40, offset}, {40, offset + 5,},
        {65, offset + 5}, {65, offset}, {120, offset}, {120, offset + 30}, {45, offset + 30},
        {45, offset + 35}, {20, offset + 35}, {20, offset + 30}, {0, offset + 30}, {0, 0}
    };
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
    }

    if (nextBlock) {
        nextBlock->moveBy(pos + sf::Vector2f{ 0, offset + 30 } - nextBlock->pos);
    }
}

float TimerBlock::getInnerHeight()
{
    if (innerNextBlock) {
        return innerNextBlock->getChainHeight();
    }
    return 0.0;
}

TimerBlock::TimerBlock(sf::Vector2f _pos, sf::RenderWindow* window, float height, int durationMs) :
    Block(_pos, { 120, height }, window), innerNextBlock(nullptr), timerDuration(sf::milliseconds(durationMs)), elapsedTime(sf::seconds(0)),
    shape1({ 20, 100 }), shape2({ 25, 25 }), shape3({ 75, 30 }), shape4({ 25, 5 }), shape5({20, 30}),
    shape6({ 25, 25 }), shape7({ 55, 30 }), shape8({ 25, 5 }), isWorking(false), field(pos + sf::Vector2f{5, 5}, window, {40, 20})
{
    float offset = size.y - 30;

    shape1.setFillColor(sf::Color(44, 122, 65));
    shape1.setSize({ 20, size.y });

    shape2.setFillColor(sf::Color(44, 122, 65));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(44, 122, 65));
    shape3.setPosition(sf::Vector2f(45, 0));

    shape4.setFillColor(sf::Color(44, 122, 65));
    shape4.setPosition(sf::Vector2f(40, 30));

    shape5.setFillColor(sf::Color(44, 122, 65));
    shape5.setPosition({ 20, offset });

    shape6.setFillColor(sf::Color(44, 122, 65));
    shape6.setPosition({ 40, offset + 5 });

    shape7.setFillColor(sf::Color(44, 122, 65));
    shape7.setPosition({ 65, offset });

    shape8.setFillColor(sf::Color(44, 122, 65));
    shape8.setPosition({ 20, offset + 30 });

    text.setString(L"1 секунду");
    text.setCharacterSize(14);
    text.setPosition(sf::Vector2f{ 5, 8 });

    std::vector<sf::Vector2f> vertexes{ 
        {0, 0}, {20, 0}, {20, 5}, {45, 5}, {45, 0}, {120, 0}, {120, 30}, {65, 30}, {65, 35},
        {40, 35}, {40, 30}, {20, 30}, {20, offset}, {40, offset}, {40, offset + 5,},
        {65, offset + 5}, {65, offset}, {120, offset}, {120, offset + 30}, {45, offset + 30},
        {45, offset + 35}, {20, offset + 35}, {20, offset + 30}, {0, offset + 30}, {0, 0}
    };
    outline.resize(vertexes.size());
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
        outline[i].color = sf::Color::White;
    }
}

TimerBlock::~TimerBlock()
{
}

void TimerBlock::moveBy(sf::Vector2f delta)
{
    Block::moveBy(delta);
    if (innerNextBlock) {
        innerNextBlock->moveBy(delta);
    }
}

bool TimerBlock::blockInteract(Block* other)
{
    if (innerNextBlock == other && innerNextBlock != nullptr) {
        innerNextBlock->prevBlock = nullptr;
        innerNextBlock = nullptr;
    }

    if (other != nullptr && nextBlock != other && other->canBeChild) {
        if ((sf::Vector2f(pos.x + 20 - other->pos.x, pos.y + 30 - other->pos.y)).length() < interactionRadius) {
            if (!other->prevBlock) {
                innerNextBlock = other;
                other->prevBlock = this;
                other->moveBy(sf::Vector2f(pos.x + 20, pos.y + 30) - other->pos);
                return true;
            }            
        }
    }
    if (Block::blockInteract(other)) return true;
    return false;
}

TimerBlock* TimerBlock::clone()
{
    return new TimerBlock(pos, window, size.y, getDuration());
}

Block* TimerBlock::update(Car& car)
{
    if (isWorking) {
        elapsedTime += clock.restart();
        if (elapsedTime < timerDuration) {
            Block* blockToUpdate = innerNextBlock;
            while (blockToUpdate) {
                Block* newBlockToUpdate = blockToUpdate->update(car);
                if (newBlockToUpdate == blockToUpdate) {
                    break;
                }
                else {
                    blockToUpdate = newBlockToUpdate;
                }
            }
            return this;
        }
        else {
            isWorking = false;
            return nextBlock;
        }
    }
    else {
        clock.restart();
        elapsedTime = sf::seconds(0);
        isWorking = true;
        return this;
    }
}

bool TimerBlock::isInBoundingBox(sf::Vector2f point) {
    sf::FloatRect rect1(shape1.getPosition() + pos, shape1.getSize());
    sf::FloatRect rect2(shape2.getPosition() + pos, shape2.getSize());
    sf::FloatRect rect3(shape3.getPosition() + pos, shape3.getSize());
    sf::FloatRect rect4(shape5.getPosition() + pos, shape5.getSize());
    sf::FloatRect rect5(shape6.getPosition() + pos, shape6.getSize());
    sf::FloatRect rect6(shape7.getPosition() + pos, shape7.getSize());
    return rect1.contains(point) || rect2.contains(point) || rect3.contains(point)
        || rect4.contains(point) || rect5.contains(point) || rect6.contains(point);
}

std::string TimerBlock::name()
{
    return "TimerBlock";
}

int TimerBlock::getDuration()
{
    return timerDuration.asMilliseconds();
}

void TimerBlock::reset()
{
    isWorking = false;
}
