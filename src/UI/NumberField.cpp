#include "NumberField.hpp"

NumberField::NumberField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size) :
	TextField(_pos, window, size)
{
}

void NumberField::addCharacter(char32_t c)
{
	TextField::addCharacter(c);
	if (inputString.empty()) return;
	if (!(c >= '0' && c <= '9' || c == '.')) {
		inputString.pop_back();
		text.setString(inputString);
	}
}
