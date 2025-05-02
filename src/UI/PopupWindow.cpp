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
        Button* button = new Button(
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

void PopupWindow::showLevelSelection(const std::vector<std::wstring>& levelNames, Callback callback) {
    currentCallback = callback;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);
    background.setFillColor(sf::Color(0, 0, 0, 150));

    float buttonWidth = 120.f, buttonHeight = 50.f, margin = 20.f;
    size_t columns = 5;
    size_t rows = (levelNames.size() + columns - 1) / columns;

    float totalWidth = columns * buttonWidth + (columns - 1) * margin;
    float totalHeight = rows * buttonHeight + (rows - 1) * margin;
    float startX = (parentSize.x - totalWidth) / 2;
    float startY = (parentSize.y - totalHeight) / 2;

    buttons.clear();
    for (size_t i = 0; i < levelNames.size(); ++i) {
        size_t row = i / columns, col = i % columns;

        Button* button = new Button(
            { startX + col * (buttonWidth + margin), startY + row * (buttonHeight + margin) },
            &parentWindow, levelNames[i],
            [this, i]() {
                if (currentCallback) {
                    currentCallback(i);
                }
                hide();
            },
            { buttonWidth, buttonHeight });
        buttons.push_back(button);
    }

    visible = true;
}

void PopupWindow::hide() {
    visible = false;
    for (Button* button : buttons) {
        delete button;
    }
    buttons.clear();
}

void PopupWindow::render() {
    if (!visible) return;

    parentWindow.draw(background);
    parentWindow.draw(windowRect);
    parentWindow.draw(titleText);
    parentWindow.draw(messageText);

    for (auto& button : buttons) {
        button->render();
    }
}

void PopupWindow::handleEvent(const sf::Event& event) {
    if (!visible) return;

    if (const auto& mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos = parentWindow.mapPixelToCoords(
            { mousePressed->position.x, mousePressed->position.y });

        if (mousePressed->button == sf::Mouse::Button::Left) {
            bool clickedOnButton = false;

            for (int i = 0; i < buttons.size(); i++) {
                if (buttons[i]->contains(mousePos) && currentCallback) {
                    hide();
                    currentCallback(i);
                    clickedOnButton = true;
                    break;
                }
            }

            if (!clickedOnButton) {
                hide();
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