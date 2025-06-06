#include "StartButton.hpp"

void StartButton::render() {
	sf::Transform transform;
	transform.translate(pos);
	
	if (state) {
		window->draw(rectangle, transform);
	}
	else {
		window->draw(triangle, transform);
	}
	window->draw(outline, transform);
}

bool StartButton::getState()
{
	return state;
}

void StartButton::setState(bool newState)
{
	state = newState;
}

StartButton::StartButton(sf::Vector2f _pos, sf::RenderWindow* window) : UIElement(_pos, window), state(false),
triangle(8, 3), rectangle({ 13, 13 }), outline(sf::PrimitiveType::LineStrip) 
{
	triangle.setFillColor(sf::Color::White);
	triangle.setPosition({ 16, 2 });
	triangle.setRotation(sf::degrees(90));

	rectangle.setFillColor(sf::Color::Red);
	rectangle.setPosition({ 3, 3 });

	std::vector<sf::Vector2f> vertexes{ {0, 0}, {20, 0}, {20, 20}, {0, 20}, {0, 0} };
	outline.resize(vertexes.size());
	for (int i = 0; i < vertexes.size(); i++) {
		outline[i].position = vertexes[i];
		outline[i].color = sf::Color::White;
	}
}

StartButton::~StartButton() {

}

bool StartButton::contains(const sf::Vector2f& point) const
{
	sf::FloatRect bounds(pos, { 20, 20 });
	return bounds.contains(point);
}

