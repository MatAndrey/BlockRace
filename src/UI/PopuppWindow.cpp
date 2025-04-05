#include "PopupWindow.hpp"
#include <iostream>

PopupWindow::PopupWindow(sf::RenderWindow& parentWindow)
    : parentWindow(parentWindow), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), 
    titleText(font), messageText(font) {
    background.setFillColor(sf::Color(0, 0, 0, 150));
    windowRect.setFillColor(sf::Color(70, 70, 70));
    windowRect.setOutlineColor(sf::Color::White);
    windowRect.setOutlineThickness(2);
}

void PopupWindow::show(const std::wstring& title, const std::wstring& message,
    const std::vector<std::wstring>& options, Callback callback) {
    currentCallback = callback;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);

    windowRect.setSize({ parentSize.x * 0.4f, parentSize.y * 0.3f });
    windowRect.setPosition({ parentSize.x * 0.3f, parentSize.y * 0.35f });

    setupText(titleText, title, font, 24, sf::Color::White);
    setupText(messageText, message, font, 18, sf::Color::White);

    titleText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 20 });
    messageText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 70 });

    float buttonWidth = 150.f;
    float buttonHeight = 40.f;
    float startX = windowRect.getPosition().x +
        (windowRect.getSize().x - (buttonWidth + 10) * options.size() + 10) / 2;

    for (size_t i = 0; i < options.size(); ++i) {
        Button button(
            sf::Vector2f{ startX + i * (buttonWidth + 10),
            windowRect.getPosition().y + windowRect.getSize().y - 60 },
            &parentWindow,
            options[i],
            nullptr,
            sf::Vector2f{ buttonWidth, buttonHeight });
        
        buttons.push_back(button);
    }

    visible = true;
}

void PopupWindow::hide() {
    visible = false;
}

void PopupWindow::render() {
    if (!visible) return;

    parentWindow.draw(background);
    parentWindow.draw(windowRect);
    parentWindow.draw(titleText);
    parentWindow.draw(messageText);

    for (auto& button : buttons) {
        button.render();
    }
}

void PopupWindow::handleEvent(const sf::Event& event) {
    if (!visible) return;

    if (const auto& mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = parentWindow.mapPixelToCoords(
            { mousePressed->position.x, mousePressed->position.y });
        if (mousePressed->button == sf::Mouse::Button::Left) {
            for (int i = 0; i < buttons.size(); i++) {
                if (buttons[i].isInBoundingBox(mousePos) && currentCallback) {
                    currentCallback(i);
                }
            }
        }
    }
    if (const auto& mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        bool isOver = false;
        sf::Vector2f mousePos = parentWindow.mapPixelToCoords(
            { mouseMoved->position.x, mouseMoved->position.y });
        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].isInBoundingBox(mousePos)) {
                isOver = true;
                break;
            }
        }
        if (isOver) {
            const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
            parentWindow.setMouseCursor(cursor);
        }
        else {
            const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
            parentWindow.setMouseCursor(cursor);
        }
    }
}

bool PopupWindow::isVisible() const {
    return visible;
}

void PopupWindow::setupText(sf::Text& text, const std::wstring& str,
    const sf::Font& font, unsigned size, const sf::Color& color) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
}