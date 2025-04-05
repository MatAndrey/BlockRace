#include "PopupWindow.hpp"
#include <iostream>

PopupWindow::PopupWindow(sf::RenderWindow& parentWindow)
    : parentWindow(parentWindow), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), 
    titleText(font), messageText(font) {
    background.setFillColor(sf::Color(0, 0, 0, 150));
    windowRect.setFillColor(sf::Color(70, 70, 70));
    windowRect.setOutlineColor(sf::Color::White);
    windowRect.setOutlineThickness(2);

    EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &PopupWindow::handleMousePressed);
}

void PopupWindow::show(const std::wstring& title, const std::wstring& message,
    const std::vector<std::wstring>& options, Callback callback) {
    currentCallback = callback;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);

    windowRect.setSize({ parentSize.x * 0.5f, parentSize.y * 0.4f });
    windowRect.setPosition({ parentSize.x * 0.25f, parentSize.y * 0.3f });

    setupText(titleText, title, font, 24, sf::Color::White);
    setupText(messageText, message, font, 18, sf::Color::White);

    titleText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 20 });
    messageText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 70 });

    optionRects.clear();
    optionTexts.clear();

    float buttonWidth = 150.f;
    float buttonHeight = 40.f;
    float startX = windowRect.getPosition().x +
        (windowRect.getSize().x - (buttonWidth + 10) * options.size() + 10) / 2;

    for (size_t i = 0; i < options.size(); ++i) {
        sf::RectangleShape button({ buttonWidth, buttonHeight });
        button.setPosition({ startX + i * (buttonWidth + 10),
            windowRect.getPosition().y + windowRect.getSize().y - 60 });
        button.setFillColor(sf::Color(100, 100, 100));
        button.setOutlineColor(sf::Color::White);
        button.setOutlineThickness(1);

        sf::Text text(font);
        setupText(text, options[i], font, 18, sf::Color::White);
        text.setPosition(button.getPosition() +
            sf::Vector2f{ (buttonWidth - text.getLocalBounds().size.x) / 2,
                        (buttonHeight - text.getLocalBounds().size.y) / 2 - 5 });

        optionRects.push_back(button);
        optionTexts.push_back(text);
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

    for (size_t i = 0; i < optionRects.size(); ++i) {
        parentWindow.draw(optionRects[i]);
        parentWindow.draw(optionTexts[i]);
    }
}

void PopupWindow::handleMousePressed(const sf::Event::MouseButtonPressed& event)
{
    if (event.button == sf::Mouse::Button::Left && visible) {
        sf::Vector2f mousePos = parentWindow.mapPixelToCoords(
            { event.position.x, event.position.y });

        for (size_t i = 0; i < optionRects.size(); ++i) {
            if (optionRects[i].getGlobalBounds().contains(mousePos)) {
                if (currentCallback) {
                    currentCallback(static_cast<int>(i));
                }
                hide();
                break;
            }
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