#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "BlackSmokeParticle.hpp"

class BlackSmokeSystem {
private:
    std::vector<BlackSmokeParticle> particles;
    float emissionTimer;
    float emissionRate;
    sf::RenderWindow& window;

public:
    bool isActive = false;
    BlackSmokeSystem(sf::RenderWindow& win);

    void update(float dt);
    void draw();
    void activate();
    void disactivate();
};