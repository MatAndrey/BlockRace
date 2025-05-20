#pragma once
#include "FireworkParticle.hpp"

class FireworkSystem {
private:
    std::vector<FireworkParticle> particles;
    std::vector<sf::Vector2f> explosionPositions;
    float timeSinceLastExplosion;
    float explosionDelay;
    int explosionsRemaining;
    sf::RenderWindow& window;

public:
    FireworkSystem(sf::RenderWindow& win);

    void triggerFireworks(int count);

    void update(float dt);

    void createExplosion(const sf::Vector2f& position);

    void draw(sf::RenderWindow& window);

    bool isActive() const;
};