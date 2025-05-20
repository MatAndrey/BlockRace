#include "BlackSmokeSystem.hpp"

BlackSmokeSystem::BlackSmokeSystem(sf::RenderWindow& win) :
    emissionTimer(0),
    emissionRate(0.1f),
    window(win) {
}

void BlackSmokeSystem::update(float dt)
{
    emissionTimer += dt;

    if (emissionTimer >= emissionRate) {
        emissionTimer = 0;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> countDist(1, 3);
        int count = countDist(gen);

        sf::Vector2u winSize = window.getSize();

        for (int i = 0; i < count; ++i) {
            std::uniform_real_distribution<float> xDist(0, winSize.x * 1.0f);
            particles.emplace_back(xDist(gen), winSize.y * 1.0f);
        }
    }

    for (auto& particle : particles) {
        particle.update(dt);
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const BlackSmokeParticle& p) { return !p.isAlive(); }),
        particles.end());
}

void BlackSmokeSystem::draw()
{
    for (auto it = particles.rbegin(); it != particles.rend(); ++it) {
        window.draw(it->shape);
    }
}

void BlackSmokeSystem::activate()
{
    isActive = true;
}

void BlackSmokeSystem::disactivate()
{
    isActive = false;
    particles.clear();
}
