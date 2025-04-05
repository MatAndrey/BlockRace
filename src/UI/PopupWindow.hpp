#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../EventBus.hpp"

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
    void hide();
    void render();
    void handleMousePressed(const sf::Event::MouseButtonPressed& event);

    bool isVisible() const;

private:
    sf::RenderWindow& parentWindow;
    sf::RectangleShape background;
    sf::RectangleShape windowRect;
    sf::Font font;
    sf::Text titleText;
    sf::Text messageText;
    std::vector<sf::RectangleShape> optionRects;
    std::vector<sf::Text> optionTexts;
    Callback currentCallback;

    bool visible = false;

    void setupText(sf::Text& text, const std::wstring& str, const sf::Font& font,
        unsigned size, const sf::Color& color);
};