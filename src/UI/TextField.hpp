#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Entity.hpp"

class TextField : public Entity {
    sf::RectangleShape background;
    sf::Text text;
    sf::Vector2f size;
    sf::Font font;
    std::string inputString;
    bool isActive;
public:
    TextField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size);
    void handleEvent(const sf::Event* event);
    virtual void render();

    std::string getText() const;

    void setText(const std::string& newText);  
};