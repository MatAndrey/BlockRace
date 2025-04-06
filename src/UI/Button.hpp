#pragma once
#include "UIElement.hpp"
#include <functional>
#include "../EventBus.hpp"

class Button : public UIElement {
	sf::Font font;
	sf::Text text;
	sf::RectangleShape background;
	std::function<void()> callback;
	bool isHovered = false;
public:
	Button(sf::Vector2f pos, sf::RenderWindow* window, std::wstring textContent, std::function<void()> callback, sf::Vector2f size);
	~Button();
	virtual void render();
	void handleMousePressed(const sf::Event::MouseButtonPressed& event);
	virtual bool contains(const sf::Vector2f& point) const;
};