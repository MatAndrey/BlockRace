#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../EventBus.hpp"
#include "Button.hpp"

class PopupWindow {
public:
    using Callback = std::function<void(int optionIndex)>;

    PopupWindow(sf::RenderWindow& parentWindow);

    void show(
        const std::wstring& title,
        const std::wstring& message,
        const std::vector<std::wstring>& options,
        Callback callback = nullptr
    );
    void showLevelSelection(const std::vector<std::wstring>& levelNames, Callback callback);
    void hide();
    void render();
    void handleEvent(const sf::Event& event);

    bool isVisible() const;

private:
    sf::RenderWindow& parentWindow;
    sf::RectangleShape background;
    sf::RectangleShape windowRect;
    sf::Font font;
    sf::Text titleText;
    sf::Text messageText;
    std::vector<Button*> buttons;
    Callback currentCallback;

    bool visible = false;

    void setupText(sf::Text& text, const std::wstring& str, const sf::Font& font,
        unsigned size, const sf::Color& color);
};