#include "NumberField.hpp"
#include <iostream>

NumberField::NumberField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size) :
	TextField(_pos, window, size)
{
}

void NumberField::addCharacter(char32_t c)
{
    if (c != '\b' && !(c >= '0' && c <= '9' || c == '.')) {
        return;
    }

    TextField::addCharacter(c);
}
