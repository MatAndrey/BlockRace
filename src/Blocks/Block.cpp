#include "Block.hpp"

Block::Block(sf::Vector2f _pos, sf::Vector2f _size, sf::RenderWindow* window) : Entity(_pos, window), size(_size), child(nullptr) {}

Block::~Block() {}
