#include "SmokeSystem.hpp"

SmokeSystem::SmokeSystem() : emissionRate(0.05f), timeSinceLastEmission(0), carSpeed(0) {
}

void SmokeSystem::setCarParameters(const sf::Vector2f& position, float speed)
{
    carPosition = position;
    carSpeed = speed;
}

void SmokeSystem::update(float dt)
{
    timeSinceLastEmission += dt;

    if (carSpeed > 0.f && timeSinceLastEmission >= emissionRate) {
        timeSinceLastEmission = 0;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> countDist(2, 3);
        int count = countDist(gen);

        for (int i = 0; i < count; ++i) {
            sf::Vector2f offset(-10.f, 0);
            particles.emplace_back(sf::Vector2f(carPosition.x + offset.x, carPosition.y + offset.y));
        }
    }

    for (auto& particle : particles) {
        particle.update(dt);
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const SmokeParticle& p) { return !p.isAlive(); }),
        particles.end());
}

void SmokeSystem::draw(sf::RenderWindow& window)
{
    for (auto& particle : particles) {
        window.draw(particle.shape);
    }
}
