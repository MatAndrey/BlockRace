#pragma once

class StartBlock;

struct BlockPressedEvent {
    sf::Vector2f worldPos;
    sf::Mouse::Button button;
    bool isRunning;
};

struct BlockReleaseEvent {
    sf::Vector2f worldPos;
    sf::Mouse::Button button;
};

struct StartSimulationEvent {
    StartBlock* startBlock;
};

struct StopSimulationEvent {};

struct DisableBlocksEvent {};

struct CarAccidentEvent {};