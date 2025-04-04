#include "TextField.hpp"
#include <iostream>

TextField::TextField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size)
    : Entity(_pos, window), size(size), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), text(font)
{
    background.setSize(size);
    background.setFillColor(sf::Color::White);
    background.setOutlineThickness(1);
    background.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setCharacterSize(size.y - 4);
    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f{ 2, 1 });

    cursor.setSize({ 1, size.y - 4 });
    cursor.setFillColor(sf::Color::Black);

    EventBus::get().subscribe<BlockPressedEvent>(this, &TextField::handlePress);
    EventBus::get().subscribe<sf::Event::TextEntered>(this, &TextField::handleTextEntered);
}

void TextField::render()
{
    sf::Transform transform;
    transform.translate(pos);
    
    window->draw(background, transform);
    window->draw(text, transform);

    if (clock.getElapsedTime() > sf::milliseconds(600)) {
        clock.restart();
        cursorState = !cursorState;
    }

    if (isActive && cursorState) {
        cursor.setPosition({ text.getLocalBounds().size.x + 4, 2 });
        window->draw(cursor, transform);
    }
}

std::string TextField::getText() const
{
    return inputString;
}

void TextField::setText(const std::string& newText)
{
    inputString = newText;
    text.setString(inputString);
    while (text.getLocalBounds().size.x > size.x - 5 || inputString.back() == '0' || inputString.back() == '.') {
        inputString.pop_back();
        text.setString(inputString);
    }
}

void TextField::disable()
{
    isActive = false;
    background.setOutlineColor(sf::Color::Black);
}

void TextField::enable()
{
    isActive = true;
    background.setOutlineColor(sf::Color::Blue);
}

void TextField::handlePress(const BlockPressedEvent& event)
{
    sf::FloatRect rect(pos, size);
    if (rect.contains(event.worldPos)) {
        enable();
    }
    else {
        disable();
    }
}

void TextField::handleTextEntered(const sf::Event::TextEntered& event)
{
    if(isActive)
        addCharacter(event.unicode);
}

void TextField::addCharacter(char32_t c)
{
    if (c == '\b' && !inputString.empty()) {
        inputString.pop_back();
    }
    else if (c >= 32 && c <= 126) {
        inputString += static_cast<char>(c);
    }
    text.setString(inputString);
    if (text.getLocalBounds().size.x > size.x - 1) {
        inputString.pop_back();
        text.setString(inputString);
    }
}
