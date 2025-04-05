#include "Button.hpp"
#include <iostream>

Button::Button(sf::Vector2f pos, sf::RenderWindow* window, std::wstring textContent, std::function<void()> callback, sf::Vector2f size) :
	UIElement(pos, window), font(".\\assets\\fonts\\Share-Tech-CYR.otf"), text(font), callback(callback)
{
	EventBus::get().subscribe<sf::Event::MouseButtonPressed>(this, &Button::handleMousePressed);
	
	background.setSize(size);
	background.setPosition(pos);
	background.setFillColor(sf::Color(100, 100, 100));
	background.setOutlineColor(sf::Color::White);
	background.setOutlineThickness(1);

	font.setSmooth(false);
	text.setString(textContent);
	text.setCharacterSize(18);
	text.setFillColor(sf::Color::White);
	text.setPosition(pos + sf::Vector2f{ (size.x - text.getLocalBounds().size.x) / 2.0f,  (size.y - text.getLocalBounds().size.y) / 2.0f - 5 });
}

Button::~Button()
{
	EventBus::get().unsubscribe<sf::Event::MouseButtonPressed>(this);
}

void Button::render()
{
	text.setFont(font);
	window->draw(background);
	window->draw(text);
}

void Button::handleMousePressed(const sf::Event::MouseButtonPressed& event)
{
	sf::Vector2f worldPos = window->mapPixelToCoords(event.position);
	if (contains(worldPos)) {
		if(callback)
			callback();
	}
}

bool Button::contains(const sf::Vector2f& point) const
{
	return background.getGlobalBounds().contains(point);
}