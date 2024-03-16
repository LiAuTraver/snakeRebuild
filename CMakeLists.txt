cmake_minimum_required(VERSION 3.27)
project(snakeRebuild)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else (clang++)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -fmodules")
endif ()

# Enable module compilation
add_compile_definitions(CMAKE_CXX_COMPILE_FEATURES="cxx_std_23")

# Add your module as a library
add_library(std
        std.ixx)
add_library(std.compat
        std.compat.ixx
)
add_library(ancillarycat.console
        ancillarycat.console.cpp
        ancillarycat.console.ixx
)
add_library(ancillarycat.ansi
        ancillarycat.ansi.ixx
)
add_library(ancillarycat.handler
        ancillarycat.handler.ixx
)
add_library(ancillarycat.windows.api
        ancillarycat.windows.api.ixx
)
add_library(ancillarycat.entities
        ancillarycat.entities.ixx
        ancillarycat.entity.ixx
        ancillarycat.entity.cpp
        ancillarycat.snake.ixx
)
add_library(ancillarycat.config
        ancillarycat.config.ixx
)
add_library(ancillarycat.leaderboard
        ancillarycat.leaderboard.ixx
)
add_library(ancillarycat.generator
        ancillarycat.generator.ixx
)
add_library(ancillarycat.panel
        ancillarycat.panel.ixx
)
add_library(ancillarycat.game
        ancillarycat.game.ixx
)

# Specify the sources for your library
set_property(TARGET ancillarycat.game PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.console PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.ansi PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.handler PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.windows.api PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.entities PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.config PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.leaderboard PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.generator PROPERTY CXX_MODULES TRUE)
set_property(TARGET ancillarycat.panel PROPERTY CXX_MODULES TRUE)
set_property(TARGET std.compat PROPERTY CXX_MODULES TRUE)
set_property(TARGET std PROPERTY CXX_MODULES TRUE)

# Add your executable
add_executable(snakeRebuild snakeRebuild.cpp)

# Link your library to your executable
target_link_libraries(snakeRebuild
        std
        std.compat
        ancillarycat.console
        ancillarycat.ansi
        ancillarycat.handler
        ancillarycat.windows.api
        ancillarycat.entities
        ancillarycat.config
        ancillarycat.leaderboard
        ancillarycat.generator
        ancillarycat.panel
        ancillarycat.game
)
