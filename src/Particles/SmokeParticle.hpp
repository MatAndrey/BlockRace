#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class SmokeParticle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;

    SmokeParticle(sf::Vector2f position);

    void update(float dt);

    bool isAlive() const;
};