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
    std::vector<std::vector<sf::Vector2f>> roadBorders;
    std::vector<sf::CircleShape> checkpoints;
    int currCheckpoint = 0;
    float checkpointRadius = 5.0f;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    sf::Vector2f carInitPos;
    sf::Angle carInitDir;

    sf::Vector2f startPos;
    bool isMoving = false;
    sf::Vector2f cameraPos;
    sf::View* view;
    sf::RenderWindow* window;
    Car* car;

    void onMousePressed(const sf::Event::MouseButtonPressed& event);
    void onMouseReleased(const sf::Event::MouseButtonReleased& event);
    void onMouseMoved(const sf::Event::MouseMoved& event);
    bool checkCollision();
    void updateCheckPoint();
    void loadDataFromFile(const std::wstring& path);
    sf::Vector2f getClampedCameraPos(sf::Vector2f cameraPos);

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
    Level(sf::RenderWindow* window, Car* car, sf::View* view);

    void render(float alpha, bool isRunning);
    void update();
    void reset();
    void setupCameraPos();
    void setLevel(int levelNumber);
};
