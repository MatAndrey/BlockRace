#include "StartBlock.hpp"

void StartBlock::render() { 
    sf::Transform transform;
    transform.translate(pos);

	window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(outline, transform);
    window->draw(text, transform);
}

StartBlock::StartBlock(sf::Vector2f _pos, sf::RenderWindow* window):
    Block(_pos, {150, 30}, window),
    shape1({ 150, 30 }), shape2({ 25, 5 })
{
    shape1.setFillColor(sf::Color(44, 122, 65));

    shape2.setFillColor(sf::Color(44, 122, 65));
    shape2.setPosition(sf::Vector2f(20, 30));

    text.setString(L"Начало");
    text.setCharacterSize(20);
    text.setPosition({ 10, 0 });


    std::vector<sf::Vector2f> vertexes{ {0, 0}, {150, 0}, {150, 30}, {45, 30}, {45, 35}, {20, 35}, {20, 30}, {0, 30}, {0, 0} };
    outline.resize(vertexes.size());
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
        outline[i].color = sf::Color::White;
    }
}

StartBlock::~StartBlock() {

}

void StartBlock::update(Car& car) {
    if (nextBlock != nullptr) {
        nextBlock->update(car);
    }
}