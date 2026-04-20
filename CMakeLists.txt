cmake_minimum_required(VERSION 3.22)

project(SFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_executable(main
    src/main.cpp
    src/Game.cpp
    src/Capsule.cpp
    src/Enemy.cpp
    src/Turret.cpp
    src/Spawner.cpp
    src/Projectiles.cpp
)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE SFML::Graphics)

# Keep runtime assets next to the built executable.
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:main>/assets"
)