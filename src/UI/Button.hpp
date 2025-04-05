#pragma once
#include "../Entity.hpp"
#include <functional>
#include "../EventBus.hpp"

class Button : public Entity {
	sf::Font font;
	sf::Text text;
	sf::RectangleShape background;
	std::function<void()> callback;
public:
	Button(sf::Vector2f _pos, sf::RenderWindow* window, std::wstring textContent, std::function<void()> callback, sf::Vector2f size);
	~Button();
	virtual void render();
	void handleMousePressed(const sf::Event::MouseButtonPressed& event);
	bool isInBoundingBox(const sf::Vector2f mousePos);
};