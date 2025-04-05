#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.hpp"

class StartButton : public UIElement {
	bool state;
	sf::CircleShape triangle;
	sf::RectangleShape rectangle;
	sf::VertexArray outline;
public:
	StartButton(sf::Vector2f _pos, sf::RenderWindow* window);
	~StartButton();
	virtual bool contains(const sf::Vector2f& point) const;
	virtual void render();
	bool getState();
	void setState(bool newState);
};