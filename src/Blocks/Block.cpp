#include "Block.hpp"

Block::Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* _window) : pos(_pos), size(_size), child(nullptr), window(_window) {}

Block::~Block()
{
}
