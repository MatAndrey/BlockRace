#include "BlackSmokeParticle.hpp"

BlackSmokeParticle::BlackSmokeParticle(float x, float y)
{
    // Случайный генератор
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Начальная позиция (внизу экрана с небольшим разбросом)
    std::uniform_real_distribution<float> posDist(-50.f, 50.f);
    shape.setPosition({ x + posDist(gen), y });

    // Размер частицы (крупные клубы)
    std::uniform_real_distribution<float> sizeDist(20.f, 50.f);
    float size = sizeDist(gen);
    shape.setRadius(size);
    shape.setOrigin({ size, size });

    // Цвет - черный с прозрачностью
    std::uniform_int_distribution<int> colorDist(20, 40);
    int dark = colorDist(gen);
    shape.setFillColor(sf::Color(dark, dark, dark, 200));

    // Движение вверх с небольшим отклонением
    std::uniform_real_distribution<float> velXDist(-10.f, 10.f);
    std::uniform_real_distribution<float> velYDist(-100.f, -60.f);
    velocity = sf::Vector2f(velXDist(gen), velYDist(gen));

    // Время жизни
    std::uniform_real_distribution<float> lifeDist(3.0f, 6.0f);
    maxLifetime = lifeDist(gen);
    lifetime = maxLifetime;

    // Масштабирование (начнет маленьким, увеличится, затем снова уменьшится)
    startScale = 0.3f;
    endScale = std::uniform_real_distribution<float>(0.8f, 1.5f)(gen);
}

void BlackSmokeParticle::update(float dt)
{
    // Движение
    shape.move(velocity * dt);

    // Уменьшение времени жизни
    lifetime -= dt;

    // Изменение прозрачности
    float alpha = (lifetime / maxLifetime) * 200;
    sf::Color color = shape.getFillColor();
    color.a = static_cast<uint8_t>(alpha);
    shape.setFillColor(color);

    // Анимация масштаба
    float progress = 1.0f - (lifetime / maxLifetime);
    if (progress < 0.5f) {
        // Растет от startScale до endScale
        float scale = startScale + (endScale - startScale) * (progress * 2);
        shape.setScale({ scale, scale });
    }
    else {
        // Затем уменьшается
        float scale = endScale - (endScale - 0.1f) * ((progress - 0.5f) * 2);
        shape.setScale({ scale, scale });
    }
}

bool BlackSmokeParticle::isAlive() const
{
    return lifetime > 0;
}


