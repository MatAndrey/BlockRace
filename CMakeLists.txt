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

# Добавляем исполняемый файл
add_executable(BlockRace src/main.cpp "src/Blocks/Block.cpp" "src/Blocks/StartBlock.cpp" "src/Car.cpp" "src/Game.cpp" "src/Entity.cpp" "src/Blocks/TimerBlock.cpp" "src/Blocks/AccelerationBlock.cpp" "src/Blocks/DecelerationBlock.cpp")

# Добавление assets в билд
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Подключаем SFML к проекту
target_link_libraries(BlockRace PRIVATE sfml-graphics sfml-window sfml-system)
