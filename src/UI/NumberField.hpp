#pragma once
#include "TextField.hpp"

class NumberField : public TextField {
public:
	NumberField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size);
	virtual void addCharacter(char32_t c);
};