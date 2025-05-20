#include "FireworkParticle.hpp"

FireworkParticle::FireworkParticle(sf::Vector2f position, sf::Color baseColor) 
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> sizeDist(2.f, 5.f);
    float size = sizeDist(gen);
    shape.setRadius(size);
    shape.setOrigin({size, size});
    shape.setPosition(position);

    std::uniform_real_distribution<float> lifeDist(1.0f, 2.5f);
    maxLifetime = lifeDist(gen);
    lifetime = maxLifetime;

    std::uniform_real_distribution<float> angleDist(0, 2 * 3.14159265f);
    std::uniform_real_distribution<float> speedDist(50.f, 200.f);
    float angle = angleDist(gen);
    float speed = speedDist(gen);
    velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

    std::uniform_int_distribution<int> colorVar(-30, 30);
    color = sf::Color(
        std::clamp(baseColor.r + colorVar(gen), 0, 255),
        std::clamp(baseColor.g + colorVar(gen), 0, 255),
        std::clamp(baseColor.b + colorVar(gen), 0, 255),
        255
    );
    shape.setFillColor(color);

    
    gravity = 98.1f;
}

void FireworkParticle::update(float dt) {
    velocity.y += gravity * dt;
    shape.move(velocity * dt);

    lifetime -= dt;

    float alpha = (lifetime / maxLifetime) * 255;
    sf::Color currentColor = color;
    currentColor.a = static_cast<uint8_t>(alpha);
    shape.setFillColor(currentColor);
}

bool FireworkParticle::isAlive() const {
    return lifetime > 0;
}