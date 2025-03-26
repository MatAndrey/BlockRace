#include "NumberField.hpp"

NumberField::NumberField(sf::Vector2f _pos, sf::RenderWindow* window, sf::Vector2f size) :
	TextField(_pos, window, size)
{
}

void NumberField::onTextInput(const sf::Event::TextEntered* event)
{
	TextField::onTextInput(event);
	if (inputString.empty()) return;
	char c = inputString.back();
	if (!(c >= '0' && c <= '9' || c == '.')) {
		inputString.pop_back();
		text.setString(inputString);
	}	
}
