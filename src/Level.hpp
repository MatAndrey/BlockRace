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
    void loadDataFromFile(const std::string& path);

    std::vector<sf::VertexArray> borderLines;
    bool isBordersVisible = false;
    bool raceMode = false;
    bool noBorders = false;
    void handleCheatRace(const CheatRaceEvent& event);
    void handleCheatShowBorders(const CheatShowBordersEvent& event);
    void handleCheatNoBorders(const CheatNoBordersEvent& event);
    void handleKeyPressed(const sf::Event::KeyPressed& event);
    void handleKeyReleased(const sf::Event::KeyReleased& event);
public:
    Level(const std::string& path, sf::RenderWindow* window, Car* car);

    void render(sf::View& view, float alpha);
    void update();
    void reset();
};
