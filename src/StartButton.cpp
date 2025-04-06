#include "StartButton.hpp"

void StartButton::render() {
	sf::CircleShape triangle(8, 3);
	triangle.setFillColor(sf::Color::White);
	triangle.setPosition({ 16, 2 });
	triangle.setRotation(sf::degrees(90));

	sf::RectangleShape rectangle({ 13, 13 });
	rectangle.setFillColor(sf::Color::Red);
	rectangle.setPosition({ 3, 3 });

	sf::VertexArray outline(sf::PrimitiveType::LineStrip);
	std::vector<sf::Vector2f> vertexes{ {0, 0}, {20, 0}, {20, 20}, {0, 20}, {0, 0} };
	outline.resize(vertexes.size());
	for (int i = 0; i < vertexes.size(); i++) {
		outline[i].position = vertexes[i];
		outline[i].color = sf::Color::White;
	}

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

bool StartButton::click(sf::Vector2f pos)
{
	return state = !state;
}

StartButton::StartButton(sf::Vector2f _pos, sf::RenderWindow* window) : Entity(_pos, window), state(false) {

}

StartButton::~StartButton() {

}

bool StartButton::isMouseOver(sf::Vector2f mousePos)
{
	sf::FloatRect bounds( pos, { 20, 20 });
	return bounds.contains(mousePos);
}

