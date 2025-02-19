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
add_executable(BlockRace src/main.cpp)

# Подключаем SFML к проекту
target_link_libraries(BlockRace PRIVATE sfml-graphics sfml-window sfml-system)
