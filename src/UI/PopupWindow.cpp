#include "PopupWindow.hpp"
#include <iostream>

PopupWindow::PopupWindow(sf::RenderWindow& parentWindow)
    : parentWindow(parentWindow), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), 
    titleText(font), messageText(font),
    winLossImage(winTexture) {
    background.setFillColor(sf::Color(0, 0, 0, 150));
    windowRect.setOutlineThickness(2);

    winLossImage.setScale({ 0.4, 0.4 });
    winTexture.setSmooth(true);
}

void PopupWindow::show(const std::wstring& title, const std::wstring& message,
    const std::vector<std::wstring>& options, Callback callback) {
    isImageVisible = false;
    currentCallback = callback;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);
    background.setFillColor(sf::Color(0, 0, 0, 150));

    windowRect.setSize({ popupWidth, popupHeight });
    windowRect.setPosition({ (parentSize.x - popupWidth) / 2, (parentSize.y - popupHeight) / 2 });
    windowRect.setFillColor(sf::Color(70, 70, 70));
    windowRect.setOutlineColor(sf::Color::White);

    setupText(titleText, title, font, 24, sf::Color::White);
    setupText(messageText, message, font, 18, sf::Color::White);

    titleText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 20 });
    messageText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 70 });

    float buttonWidth = 160.f;
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

void PopupWindow::showWinMessage(float raceTimeSecs, bool lastLevel) {
    std::wstringstream wss;
    wss << L"Трасса завершена. Время: " << raceTimeSecs;
    show(
        L"",
        wss.str(),
        lastLevel ?
        std::vector<std::wstring>{L"Продолжить"} :
        std::vector<std::wstring>{ L"Продолжить", L"Следующий уровень" },
        [this](int option) {
            isImageVisible = false;
            if (option == 1) {
                EventBus::get().publish<NextLevelEvent>({});
            }
        });
    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    winLossImage.setPosition({ parentSize.x / 2 - winLossImage.getLocalBounds().size.x / 2 * winLossImage.getScale().x, (parentSize.y - popupHeight) / 2 - 90 });

    messageText.setPosition(windowRect.getPosition() + sf::Vector2f{ 20, 180 });

    winLossImage.setTexture(winTexture);
    isImageVisible = true;
}

void PopupWindow::showLossMessage(std::function<void()> callback)
{
    show(
        L"",
        L"",
        std::vector<std::wstring>{L"Заново"},
        [this, callback](int option) {
            callback();
            isImageVisible = false;
        }
    );
    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    winLossImage.setPosition({ parentSize.x / 2 - winLossImage.getLocalBounds().size.x / 2 * winLossImage.getScale().x, (parentSize.y - popupHeight) / 2 - 60 });

    winLossImage.setTexture(lossTexture);
    isImageVisible = true;
}

void PopupWindow::showLevelSelection(const std::vector<std::wstring>& levelNames, Callback callback) {
    hide();
    currentCallback = callback;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);
    background.setFillColor(sf::Color(0, 0, 0, 150));

    const float buttonWidth = 120.f;
    const float buttonHeight = 50.f;
    const float margin = 20.f;

    size_t columns;
    if (levelNames.size() <= 3) {
        columns = levelNames.size();
    }
    else if (levelNames.size() <= 6) {
        columns = (levelNames.size() + 1) / 2;
    }
    else {
        columns = 5;
    }

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

float computeTextWidth(const std::wstring& str, const sf::Font& font, unsigned characterSize)
{
    float width = 0.f;
    for (wchar_t c : str)
    {
        const sf::Glyph& glyph = font.getGlyph(c, characterSize, false);
        width += glyph.advance;
    }
    return width;
}

float computeTextHeight(const sf::Font& font, unsigned characterSize)
{
    return font.getLineSpacing(characterSize);
}

void PopupWindow::showCredits(const std::vector<std::vector<std::wstring>>& credits) {
    hide();
    isCreditsMode = true;
    visible = true;
    currentCallback = nullptr;

    sf::Vector2f parentSize = static_cast<sf::Vector2f>(parentWindow.getSize());
    background.setSize(parentSize);
    background.setFillColor(sf::Color::Black);

    creditsTexts.clear();

    float currentY = parentSize.y;

    for (const auto& entry : credits) {
        creditsTexts.emplace_back(font);
        sf::Text& roleText = creditsTexts.back();
        roleText.setCharacterSize(32);
        roleText.setString(sf::String(entry[0]));
        roleText.setFillColor(sf::Color::White);
        roleText.setStyle(sf::Text::Bold);

        float roleWidth = computeTextWidth(entry[0], font, 32);
        roleText.setPosition(sf::Vector2f((parentSize.x - roleWidth) / 2, currentY));

        currentY += computeTextHeight(font, 32);

        creditsTexts.emplace_back(font);
        sf::Text& firstNameText = creditsTexts.back();
        firstNameText.setCharacterSize(28);
        firstNameText.setString(sf::String(entry[1]));
        firstNameText.setFillColor(sf::Color(200, 200, 200));

        float firstNameWidth = computeTextWidth(entry[1], font, 28);
        firstNameText.setPosition(sf::Vector2f((parentSize.x - firstNameWidth) / 2, currentY));

        if (entry.size() > 2) {
            creditsTexts.emplace_back(font);
            sf::Text& secondNameText = creditsTexts.back();
            secondNameText.setCharacterSize(28);
            secondNameText.setString(sf::String(entry[2]));
            secondNameText.setFillColor(sf::Color(200, 200, 200));

            float secondNameWidth = computeTextWidth(entry[2], font, 28);
            secondNameText.setPosition(sf::Vector2f((parentSize.x - secondNameWidth) / 2, currentY + computeTextHeight(font, 28)));

            currentY += computeTextHeight(font, 28);
        }

        currentY += 70;
    }

    creditsClock.restart();
}

void PopupWindow::hide() {
    setupText(titleText, L"", font, 24, sf::Color::White);
    setupText(messageText, L"", font, 18, sf::Color::White);
    windowRect.setFillColor({ 0, 0, 0, 0 });
    windowRect.setOutlineColor({ 0, 0, 0, 0 });
    visible = false;
    isCreditsMode = false;
    creditsOffset = 0;
    creditsScrollSpeed = 100;
    for (Button* button : buttons) {
        delete button;
    }
    buttons.clear();
}

void PopupWindow::render() {
    if (!visible) return;

    if (isCreditsMode)
    {
        sf::Time dt = creditsClock.restart();
        float dy = creditsScrollSpeed * dt.asSeconds();
        creditsOffset += dy;

        float textHeight = computeTextHeight(font, 32);

        if (!creditsTexts.empty() &&
            creditsTexts.back().getPosition().y + textHeight < 0)
        {
            hide();
            return;
        }

        float windowHeight = (static_cast<sf::Vector2f>(parentWindow.getSize())).y;
        float creditsHeight = creditsTexts.back().getPosition().y + textHeight;

        parentWindow.draw(background);
        sf::Transform trans1;
        trans1.translate({ 0, -creditsOffset });
        sf::Transform trans2;
        trans2.translate({ 0, creditsTexts.back().getPosition().y + textHeight * 2 - windowHeight - creditsOffset });
        for (auto& text : creditsTexts)
        {
            parentWindow.draw(text, trans1);
            parentWindow.draw(text, trans2);
        }
        if (creditsOffset > creditsHeight) {
            creditsOffset -= creditsHeight - windowHeight + textHeight;
        }
    }
    else {
        parentWindow.draw(background);
        parentWindow.draw(windowRect);
        parentWindow.draw(titleText);
        parentWindow.draw(messageText);
        if (isImageVisible) {
            parentWindow.draw(winLossImage);
        }        

        for (auto& button : buttons) {
            button->render();
        }
    }
}

void PopupWindow::handleEvent(const sf::Event& event) {
    if (!visible) return;

    if (event.is <sf::Event::Closed>()) {
        hide();
        EventBus::get().publish<ExitEvent>({});
    }

    if (const auto& keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            hide();
        }
        if (isCreditsMode) {
            if (keyPressed->code == sf::Keyboard::Key::Equal) {
                creditsScrollSpeed += 100;
            }
            if (keyPressed->code == sf::Keyboard::Key::Hyphen) {
                creditsScrollSpeed -= 100;
            }
        }        
    }

    if (isCreditsMode) {
        if (const auto& mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            hide();
			isCreditsMode = false;
        }
        return;
    }

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