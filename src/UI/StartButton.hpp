#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.hpp"

class StartButton : public Entity {
	bool state;
	sf::CircleShape triangle;
	sf::RectangleShape rectangle;
	sf::VertexArray outline;
public:
	StartButton(sf::Vector2f _pos, sf::RenderWindow* window);
	~StartButton();
	bool isMouseOver(sf::Vector2f pos);
	virtual void render();
	bool click(sf::Vector2f pos);
};