#include "Level.hpp"
#include <iostream>

bool doSegmentsIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2) {
    auto crossProduct = [](sf::Vector2f a, sf::Vector2f b) {
        return a.x * b.y - a.y * b.x;
        };

    auto subtractPoints = [](sf::Vector2f a, sf::Vector2f b) {
        return sf::Vector2f(a.x - b.x, a.y - b.y);
        };

    sf::Vector2f r = subtractPoints(p2, p1);
    sf::Vector2f s = subtractPoints(q2, q1);
    float rxs = crossProduct(r, s);
    float qpxr = crossProduct(subtractPoints(q1, p1), r);

    if (rxs == 0 && qpxr == 0) {
        // Линии коллинеарны
        return false;
    }

    if (rxs == 0 && qpxr != 0) {
        // Линии не пересекаются
        return false;
    }

    float t = crossProduct(subtractPoints(q1, p1), s) / rxs;
    float u = crossProduct(subtractPoints(q1, p1), r) / rxs;

    return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}

bool isCarIntersectingLine(const Car* car, const std::vector<sf::Vector2f>& linePoints) {
    const sf::FloatRect& rect = car->getBounds();
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> rectangleEdges = {
        {sf::Vector2f(rect.position.x, rect.position.y), sf::Vector2f(rect.position.x, rect.position.y + rect.size.y)},
        {sf::Vector2f(rect.position.x, rect.position.y + rect.size.y), sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y)},
        {sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y), sf::Vector2f(rect.position.x + rect.size.x, rect.position.y)},
        {sf::Vector2f(rect.position.x + rect.size.x, rect.position.y), sf::Vector2f(rect.position.x, rect.position.y)},
    };

    for (size_t i = 0; i < linePoints.size() - 1; ++i) {
        const sf::Vector2f& lineStart = linePoints[i];
        const sf::Vector2f& lineEnd = linePoints[i + 1];

        for (const auto& edge : rectangleEdges) {
            if (doSegmentsIntersect(edge.first, edge.second, lineStart, lineEnd)) {
                return true;
            }
        }
    }
    return false;
}

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

void Level::render(sf::View& view)
{
	view.setCenter(car->pos);
	window->draw(mapSprite);
}

void Level::update(std::function<void()> resetFunction)
{
    for (const auto& border : roadBorders) {
        if (isCarIntersectingLine(car, border)) {
            resetFunction();
        }
    }
}

void Level::carReset()
{
	car->reset(carInitPos, carInitDir);
}

