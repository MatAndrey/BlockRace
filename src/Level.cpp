#include "Level.hpp"
#include <iostream>

Level::Level(const std::string& path, sf::RenderWindow* window, Car* car) :
	car(car), window(window), mapTexture(), mapSprite(mapTexture)
{
	std::ifstream file(path);

	jsonData = json::parse(file);

	if (mapTexture.loadFromFile(jsonData["mapTexturePath"].get<std::string>())) {
		mapSprite.setTexture(mapTexture);
		mapSprite.setTextureRect({ {0 , 0 }, sf::Vector2i(mapTexture.getSize()) });
	}
	
	carInitPos = sf::Vector2f(jsonData["carInitX"], jsonData["carInitY"]);
	carInitDir = sf::degrees(jsonData["carInitDir"]);


	for (const auto& border : jsonData["mapBorders"]) {
		std::vector<sf::Vector2f> vertexes;
		for (const auto& borderVertex : border) {
			vertexes.push_back({ borderVertex[0], borderVertex[1] });
		}
		roadBorders.push_back(vertexes);
	}
	
	carReset();
}

bool Level::checkCollision() {
    if (roadBorders.empty()) return false;

    // Получаем глобальные границы машины
    const sf::FloatRect carBounds = car->getGlobalBounds();
    const sf::Transform transform = car->getTransform();

    // Размеры машины
    float width = carBounds.size.x;
    float height = carBounds.size.y;

    // Углы машины в локальных координатах (относительно центра)
    std::vector<sf::Vector2f> localCorners = {
        {-width / 2, -height / 2}, // Левый верхний
        {width / 2, -height / 2},  // Правый верхний
        {width / 2, height / 2},   // Правый нижний
        {-width / 2, height / 2}   // Левый нижний
    };

    // Переводим их в глобальные координаты
    std::vector<sf::Vector2f> carCorners;
    for (const auto& corner : localCorners) {
        carCorners.push_back(transform.transformPoint({ carBounds.position.x + carBounds.size.x / 2 + corner.x,
            carBounds.position.y + carBounds.size.y / 2 + corner.y }));
    }

    // Функция проверки пересечения двух отрезков
    auto linesIntersect = [](const sf::Vector2f& p1, const sf::Vector2f& p2,
        const sf::Vector2f& p3, const sf::Vector2f& p4) {
            auto cross = [](const sf::Vector2f& a, const sf::Vector2f& b) {
                return a.x * b.y - a.y * b.x;
                };

            sf::Vector2f r = p2 - p1;
            sf::Vector2f s = p4 - p3;
            float rxs = cross(r, s);
            float t = cross(p3 - p1, s) / rxs;
            float u = cross(p3 - p1, r) / rxs;

            return (rxs != 0 && t >= 0 && t <= 1 && u >= 0 && u <= 1);
        };

    // Проверяем каждую сторону машины с каждым сегментом границ дороги
    for (const auto& border : roadBorders) {
        if (border.size() < 2) continue;

        for (size_t i = 0; i < border.size() - 1; ++i) {
            const sf::Vector2f& wallStart = border[i];
            const sf::Vector2f& wallEnd = border[i + 1];

            // Проверяем каждую сторону машины
            for (size_t j = 0; j < carCorners.size(); ++j) {
                size_t nextJ = (j + 1) % carCorners.size();
                const sf::Vector2f& carStart = carCorners[j];
                const sf::Vector2f& carEnd = carCorners[nextJ];

                if (linesIntersect(carStart, carEnd, wallStart, wallEnd)) {
                    return true; // Столкновение обнаружено
                }
            }
        }
    }

    return false; // Столкновений нет
}

void Level::render(sf::View& view)
{
	window->draw(mapSprite);
    
    for (const auto& border : roadBorders) {
        sf::VertexArray lines(sf::PrimitiveType::LineStrip);
        for (const auto& point : border) {
            sf::Vertex v;
            v.position = point;
            v.color = sf::Color::Red;
            lines.append(v);
        }
        window->draw(lines);
    }
    
    car->render();
    view.setCenter(car->pos);
}

void Level::update()
{
    if (checkCollision()) {
        EventBus::get().publish<CarAccidentEvent>(CarAccidentEvent{});
    }
}

void Level::carReset()
{
	car->reset(carInitPos, carInitDir);
}

