#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../EventBus.hpp"
#include "Button.hpp"
#include <sstream>

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
    void showWinMessage(float raceTimeSecs, bool lastLevel);
    void showLossMessage();
    void showLevelSelection(const std::vector<std::wstring>& levelNames, Callback callback);
    void showCredits(const std::vector<std::vector<std::wstring>>& credits);
    void hide();
    void render();
    void handleEvent(const sf::Event& event);

    bool isVisible() const;

    const float popupWidth = 550;
    const float popupHeight = 300;

private:
    sf::RenderWindow& parentWindow;
    sf::RectangleShape background;
    sf::RectangleShape windowRect;
    sf::Font font;
    sf::Text titleText;
    sf::Text messageText;
    std::vector<Button*> buttons;
    Callback currentCallback;

    sf::Texture winTexture{ ".\\assets\\images\\win.png" };
    sf::Texture lossTexture{ ".\\assets\\images\\loss.png" };
    sf::Sprite winLossImage;
    bool isImageVisible = false;

    std::vector<sf::Text> creditsTexts;
    sf::Clock creditsClock;
    bool isCreditsMode = false;
    float creditsScrollSpeed = 100.0f; // (пикселей/сек)
    float creditsOffset = 0;

    bool visible = false;

    void setupText(sf::Text& text, const std::wstring& str, const sf::Font& font,
        unsigned size, const sf::Color& color);
};