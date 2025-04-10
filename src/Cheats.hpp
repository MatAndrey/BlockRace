#pragma once
#include "EventBus.hpp"
#include "SFML/Graphics.hpp"

class Cheats {
	std::string code;

	void handleTextInput(const sf::Event::KeyPressed& event);
public:
	Cheats();
};