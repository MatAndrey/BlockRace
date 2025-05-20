#pragma once
#include "SmokeParticle.hpp"

class SmokeSystem {
private:
    std::vector<SmokeParticle> particles;
    sf::Vector2f carPosition;
    float carSpeed;
    float emissionRate;
    float timeSinceLastEmission;

public:
    SmokeSystem();

    void setCarParameters(const sf::Vector2f& position, float speed);

    void update(float dt);

    void draw(sf::RenderWindow& window);
};