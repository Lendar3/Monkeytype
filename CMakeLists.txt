set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.5)
project(Task1)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         11.0.2
)
FetchContent_MakeAvailable(fmt)

add_executable(Monkeytype
        src/main.cpp
        src/GameController.cpp
        src/GameModel.cpp
        src/GameView.cpp
        src/Word.cpp
        src/Button.cpp
        src/Triangle.cpp
)
target_include_directories(Monkeytype PRIVATE include)
target_link_libraries(Monkeytype PRIVATE SFML::Graphics fmt)
