#include "Block.hpp"

Block::Block(sf::Vector2f _pos, sf::RenderWindow* _window) : pos(_pos), child(nullptr), window(_window) {}

Block::~Block()
{
}
