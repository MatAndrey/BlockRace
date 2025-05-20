cmake_minimum_required(VERSION 3.16)
project(BlockRace)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)

# Добавляем SFML через FetchContent (автоматическая загрузка)
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0  # Используем последнюю стабильную версию
)
FetchContent_MakeAvailable(SFML)
# Добавление JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Добавляем исполняемый файл
add_executable(BlockRace 
    src/main.cpp 
    "src/Blocks/Block.cpp" 
    "src/Blocks/StartBlock.cpp" 
    "src/Car.cpp" 
    "src/Game.cpp" 
    "src/Entity.cpp" 
    "src/Blocks/TimerBlock.cpp" 
    "src/Blocks/AccelerationBlock.cpp" 
    "src/Blocks/DecelerationBlock.cpp" 
    "src/UI/StartButton.cpp" 
    "src/Blocks/RotationBlock.cpp"
    "src/UI/TextField.cpp"
    "src/UI/NumberField.cpp" 
    "src/Blocks/RotationBlock.cpp" 
    "src/Level.cpp" 
    "src/UI/PopupWindow.cpp" 
    "src/UI/Button.cpp" 
    "src/BlockManager.cpp" 
    "src/UI/Menu.cpp" 
    "src/FileDialog.cpp" 
    "src/UI/UIElement.cpp"
    "src/Cheats.cpp" 
    "src/Blocks/MoveToBlock.cpp" 
    "src/Blocks/ConstSpeedBlock.cpp" 
    "src/Particles/SmokeParticle.cpp" 
    "src/Particles/SmokeSystem.cpp" 
    "src/Particles/FireworkParticle.cpp" 
    "src/Particles/FireworkSystem.cpp" "src/Particles/BlackSmokeParticle.cpp" "src/Particles/BlackSmokeSystem.cpp")

# Добавление assets в билд
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Подключаем SFML к проекту
target_link_libraries(BlockRace PRIVATE sfml-graphics sfml-window sfml-system nlohmann_json::nlohmann_json)
if(WIN32)
    target_link_libraries(BlockRace PRIVATE comdlg32.lib)
endif()
