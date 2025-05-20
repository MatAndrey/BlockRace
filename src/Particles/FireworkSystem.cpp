#include "FireworkSystem.hpp"

FireworkSystem::FireworkSystem(sf::RenderWindow& win)
    : timeSinceLastExplosion(0), explosionDelay(0.3f), explosionsRemaining(0), window(win) {
}

void FireworkSystem::triggerFireworks(int count)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    sf::Vector2u ws = window.getSize();
    std::uniform_real_distribution<float> distX(0, ws.x);
    std::uniform_real_distribution<float> distY(0, ws.y);

    explosionPositions.clear();
    for (int i = 0; i < count; ++i) {
        explosionPositions.emplace_back(
            distX(gen),
            distY(gen)
        );
    }

    explosionsRemaining = count;
    timeSinceLastExplosion = explosionDelay;
}

void FireworkSystem::update(float dt)
{
    timeSinceLastExplosion += dt;

    if (explosionsRemaining > 0 && timeSinceLastExplosion >= explosionDelay) {
        timeSinceLastExplosion = 0;
        explosionsRemaining--;

        createExplosion(explosionPositions[explosionsRemaining]);
    }

    for (auto& particle : particles) {
        particle.update(dt);
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const FireworkParticle& p) { return !p.isAlive(); }),
        particles.end());
}

void FireworkSystem::createExplosion(const sf::Vector2f& position)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> colorDist(0, 5);

    sf::Color baseColor;
    switch (colorDist(gen)) {
    case 0: baseColor = sf::Color::Red; break;
    case 1: baseColor = sf::Color::Blue; break;
    case 2: baseColor = sf::Color::Green; break;
    case 3: baseColor = sf::Color(255, 165, 0); break;
    case 4: baseColor = sf::Color::Magenta; break; 
    case 5: baseColor = sf::Color::Cyan; break;
    }

    std::uniform_int_distribution<int> countDist(50, 100);
    int particleCount = countDist(gen);

    for (int i = 0; i < particleCount; ++i) {
        particles.emplace_back(position, baseColor);
    }
}

void FireworkSystem::draw(sf::RenderWindow& window)
{
    for (auto& particle : particles) {
        window.draw(particle.shape);
    }
}

bool FireworkSystem::isActive() const
{
    return explosionsRemaining > 0 || !particles.empty();
}