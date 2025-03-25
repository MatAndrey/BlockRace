#include "TextField.hpp"

TextField::TextField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size)
    : Entity(_pos, window), size(size), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), isActive(false), text(font) {
    background.setSize(size);
    background.setFillColor(sf::Color::White);
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition({ 5, 5 });
}

void TextField::handleEvent(const sf::Event* event) {
    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (background.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
            isActive = true;
            background.setOutlineColor(sf::Color::Blue);
        }
        else {
            isActive = false;
            background.setOutlineColor(sf::Color::Black);
        }
    }

    if (isActive) {
        if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (textEntered->unicode == '\b' && !inputString.empty()) {
                inputString.pop_back();
            }
            else if (textEntered->unicode >= 32 && textEntered->unicode <= 126) {
                inputString += static_cast<char>(textEntered->unicode);
            }
            text.setString(inputString);
        }        
    }
}

void TextField::render()
{
    sf::Transform transform;
    transform.translate(pos);

    window->draw(background, transform);
    window->draw(text, transform);
}

std::string TextField::getText() const
{
    return inputString;
}

void TextField::setText(const std::string& newText)
{
    inputString = newText;
    text.setString(inputString);
}
