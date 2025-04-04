#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Entity.hpp"
#include "../EventBus.hpp"

class TextField : public Entity {
    sf::RectangleShape background;
    sf::Vector2f size;
    sf::Font font;
    sf::RectangleShape cursor;
    bool isActive = false;
    bool cursorState = false;
    sf::Clock clock;
protected:
    sf::Text text;
    std::string inputString;
public:
    TextField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size);
    virtual void render();
    std::string getText() const;
    void setText(const std::string& newText);
    void disable();
    void enable();
    void handlePress(const BlockPressedEvent& event);
    void handleTextEntered(const sf::Event::TextEntered& event);
    virtual void addCharacter(char32_t c);
};