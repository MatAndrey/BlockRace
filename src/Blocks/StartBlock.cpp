#include "StartBlock.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

void StartBlock::draw() { 
    sf::RectangleShape shape1({ 150, 30 });
    shape1.setFillColor(sf::Color(44, 122, 65));
    shape1.setPosition(pos);
    window->draw(shape1);

    sf::RectangleShape shape2({ 25, 5 });
    shape2.setFillColor(sf::Color(44, 122, 65));
    shape2.setPosition(pos + sf::Vector2f(20, 30));
    window->draw(shape2);

    sf::Font font(".\\assets\\fonts\\Share-Tech-CYR.otf");
    sf::Text text(font);
    text.setString(L"Начало");
    text.setCharacterSize(20);
    text.setPosition(pos + sf::Vector2f{20, 0});
    
    window->draw(text);
}

StartBlock::StartBlock(sf::Vector2f _pos, Car* _car, sf::RenderWindow* _window): Block(_pos, {150, 30}, _window), car(_car) {}

StartBlock::~StartBlock() {}