cmake_minimum_required(VERSION 3.27)
project(snakeRebuild)

set(CMAKE_CXX_STANDARD 23)

# set to C++23
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")



include_directories(.)

add_executable(snakeRebuild
        ancillarycat.ansi.ixx
        ancillarycat.config.ixx
        ancillarycat.console.ixx
        ancillarycat.cursor.ixx
        ancillarycat.handler.ixx
        ancillarycat.leaderboard.ixx
        ancillarycat.panel.ixx
        ancillarycat.print.ixx
        ancillarycat.snake.ixx
        ancillarycat.windows.api.ixx
        config.hpp
        core.hpp
        limits.hpp
        realMain.cpp
        snakeRebuild.cpp
        std.compat.ixx
        std.ixx)
