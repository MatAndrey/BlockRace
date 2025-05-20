#include "SmokeParticle.hpp"

SmokeParticle::SmokeParticle(sf::Vector2f position)
{
    shape.setPosition(position);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> sizeDist(2.f, 6.f);
    float size = sizeDist(gen);
    shape.setRadius(size);
    shape.setOrigin({size, size});

    std::uniform_int_distribution<int> colorDist(50, 150);
    int gray = colorDist(gen);
    shape.setFillColor(sf::Color(gray, gray, gray, 200));

    std::uniform_real_distribution<float> velDist(-0.5f, 0.5f);
    velocity = sf::Vector2f(velDist(gen), velDist(gen));

    std::uniform_real_distribution<float> lifeDist(0.5f, 1.5f);
    maxLifetime = lifeDist(gen);
    lifetime = maxLifetime;
}

void SmokeParticle::update(float dt)
{
    shape.move(velocity);

    lifetime -= dt;

    float alpha = (lifetime / maxLifetime) * 200;
    sf::Color color = shape.getFillColor();
    color.a = static_cast<uint8_t>(alpha);
    shape.setFillColor(color);

    float scale = lifetime / maxLifetime;
    shape.setScale({scale, scale});
}

bool SmokeParticle::isAlive() const
{
    return lifetime > 0;
}