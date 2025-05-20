#pragma once
#include <SFML/Graphics.hpp>
#include <random>


class FireworkParticle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float gravity;

    FireworkParticle(sf::Vector2f position, sf::Color baseColor);

    void update(float dt);

    bool isAlive() const;
};