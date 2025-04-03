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

	std::cout << carInitDir.asDegrees() << std::endl;

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

void Level::carReset()
{
	car->reset(carInitPos, carInitDir);
}
