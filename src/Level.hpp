#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Car.hpp"
#include "EventBus.hpp"

using json = nlohmann::json;

class Level {
    json jsonData;
    sf::RenderWindow* window;
    Car* car;
    std::vector<std::vector<sf::Vector2f>> roadBorders;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    sf::Vector2f carInitPos;
    sf::Angle carInitDir;
    bool checkCollision();
public:
    Level(const std::string& path, sf::RenderWindow* window, Car* car);

    void Level::render(sf::View& view);
    void update();
    void reset();
};
