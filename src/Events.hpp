#pragma once
#include "SFML/Graphics.hpp"

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

struct SaveFileEvent {
    std::wstring fileName;
};

struct LoadFileEvent {
    std::wstring fileName;
};

struct ExitEvent {};

struct RaceFinishedEvent {};

struct SetTargetEvent {
    sf::Vector2f targetPos;
    sf::Vector2f eventPos;
};

struct SetLevelEvent {
    int levelNumber;
};

struct NextLevelEvent {};

struct ClearBlocksEvent {};

struct CheatRaceEvent {};

struct CheatShowBordersEvent {};

struct CheatNoBordersEvent {};

struct CheatUnlockLevelsEvent {};