#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class BlackSmokeParticle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    float startScale;
    float endScale;

    BlackSmokeParticle(float x, float y);

    void update(float dt);

    bool isAlive() const;
};