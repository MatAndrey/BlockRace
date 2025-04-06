#pragma once
#include <string>
#include <sstream>
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
    std::vector<sf::CircleShape> checkpoints;
    int currCheckpoint = 0;
    float checkpointRadius = 5.0f;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    sf::Vector2f carInitPos;
    sf::Angle carInitDir;
    bool checkCollision();
    void updateCheckPoint();
public:
    Level(const std::string& path, sf::RenderWindow* window, Car* car);

    void render(sf::View& view, float alpha);
    void update();
    void reset();
};
