#include "AccelerationBlock.hpp"

AccelerationBlock::AccelerationBlock(sf::Vector2f _pos, sf::RenderWindow* window):
	Block(_pos, {150, 30}, window), acceleration(1), shape1({20, 30}), shape2({25, 30}), shape3({105, 30})
{
    shape1.setFillColor(sf::Color(44, 122, 65));

    shape2.setFillColor(sf::Color(44, 122, 65));
    shape2.setPosition(sf::Vector2f(20, 5));

    shape3.setFillColor(sf::Color(44, 122, 65));
    shape3.setPosition(sf::Vector2f(45, 0));

    text.setString(L"���������");
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
}

AccelerationBlock::~AccelerationBlock()
{
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

void AccelerationBlock::update(Car& car)
{
    car.accelerate(1);
	if (nextBlock) {
		nextBlock->update(car);
	}
}
