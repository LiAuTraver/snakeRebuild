# successful cmake file

cmake_minimum_required(VERSION 3.27)
project(snakeRebuild)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D module_bug /std:c++latest")
# Enable module compilation
add_compile_definitions(CMAKE_CXX_COMPILE_FEATURES="cxx_std_23")
# add modules as library
add_library(std)
target_sources(std
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        module/std.ixx
)
set_property(TARGET std PROPERTY CXX_MODULES TRUE)
add_library(std.compat
        module/std.ixx module/std.compat.ixx)

add_library(ancillarycat.api.ansi
        src/ancillarycat.api.ansi.ixx)

add_library(ancillarycat.api.windows
        src/ancillarycat.api.windows.ixx)
add_library(ancillarycat.api.generator
        src/ancillarycat.api.generator.ixx)
add_library(ancillarycat.api
        module/ancillarycat.api.ixx)
target_link_libraries(ancillarycat.api
        PUBLIC
        ancillarycat.api.ansi
        ancillarycat.api.windows
        ancillarycat.api.generator
)
# order matters!
add_library(ancillarycat.console.console
        module/ancillarycat.console.console.ixx
        src/ancillarycat.console.console.cpp)
add_library(ancillarycat.console.point
        src/ancillarycat.console.point.ixx)
add_library(ancillarycat.console
        module/ancillarycat.console.ixx)
target_link_libraries(ancillarycat.console
        PUBLIC
        ancillarycat.console.point
        ancillarycat.console.console
)

add_library(ancillarycat.entities.entity
        module/ancillarycat.entities.entity.ixx
        src/ancillarycat.entities.entity.cpp)
add_library(ancillarycat.entities.snake
        module/ancillarycat.entities.snake.ixx)
add_library(ancillarycat.entities.food
        module/ancillarycat.entities.food.ixx)
add_library(ancillarycat.entities
        module/ancillarycat.entities.ixx)
target_link_libraries(ancillarycat.entities
        PUBLIC
        ancillarycat.entities.entity
        ancillarycat.entities.snake
        ancillarycat.entities.food
)

add_library(ancillarycat.utils
        module/ancillarycat.utils.ixx
        src/ancillarycat.utils.cpp)

add_library(ancillarycat.game.game
        src/ancillarycat.game.game.ixx)
add_library(ancillarycat.game.leaderboard
        src/ancillarycat.game.leaderboard.ixx)
add_library(ancillarycat.game.config
        src/ancillarycat.game.config.ixx)
add_library(ancillarycat.game
        module/ancillarycat.game.ixx)
target_link_libraries(ancillarycat.game
        PUBLIC
        ancillarycat.game.game
        ancillarycat.game.leaderboard
        ancillarycat.game.config
)

add_library(ancillarycat.handler
        src/ancillarycat.handler.cpp
        module/ancillarycat.handler.ixx)

add_library(ancillarycat.blocks.block
        module/ancillarycat.blocks.block.ixx)
add_library(ancillarycat.blocks.obstacle
        module/ancillarycat.blocks.obstacle.ixx)
add_library(ancillarycat.blocks
        module/ancillarycat.blocks.ixx)

target_link_libraries(ancillarycat.blocks
        PUBLIC
        ancillarycat.blocks.block
        ancillarycat.blocks.obstacle
)
add_executable(snakeRebuild
        src/realMain.cpp
        snakeRebuild.cpp)

# appear earlier, linked later
target_link_libraries(
        snakeRebuild
        PRIVATE
        ancillarycat.handler
        ancillarycat.game
        ancillarycat.utils
        ancillarycat.blocks
        ancillarycat.entities
        ancillarycat.console
        ancillarycat.api
        std.compat
        std
)

# another method

# not working!
#function(build_all_modules)
#    foreach (module ${ARGN})
#        if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/module/${module}.ixx)
#            if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${module}.cpp)
#                add_library(${module}
#                        src/${module}.cpp
#                        module/${module}.ixx
#                        )
#            else ()
#                add_library(${module} module/${module}.ixx)
#            endif ()
#        elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${module}.ixx)
#            if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${module}.cpp)
#                add_library(${module}
#                        src/${module}.cpp
#                        src/${module}.ixx
#                        )
#            else ()
#                add_library(${module} src/${module}.ixx)
#            endif ()
#        else ()
#            message(FATAL_ERROR "No source file found for module ${module}")
#        endif ()
#    endforeach ()
#endfunction()
#
#build_all_modules(
#        std
#        std.compat
#
#        ancillarycat.api.ansi
#        ancillarycat.api.windows
#        ancillarycat.api.generator
#        ancillarycat.api
#
#        ancillarycat.console
#
#        ancillarycat.entities.entity
#        ancillarycat.entities.snake
#        ancillarycat.entities.food
#        ancillarycat.entities
#
#        ancillarycat.utils
#
#        ancillarycat.game.game
#        ancillarycat.game.leaderboard
#        ancillarycat.game.config
#        ancillarycat.game
#
#        ancillarycat.handler
#
#        ancillarycat.blocks.obstacle
#        ancillarycat.blocks
#)
#function(build_all_modules)
#    foreach (interface_module ${ARGN})
#        # Find submodule partitions in module directory
#        file(GLOB_RECURSE submodule_partitions_module "${CMAKE_CURRENT_SOURCE_DIR}/module/${interface_module}.*.*.cpp")
#
#        # Find submodule partitions in src directory
#        file(GLOB_RECURSE submodule_partitions_src "${CMAKE_CURRENT_SOURCE_DIR}/src/${interface_module}.*.*.cpp")
#
#        # Combine results from both directories
#        set(submodule_partitions ${submodule_partitions_module} ${submodule_partitions_src})
#
#        message(STATUS "Submodule partitions for ${interface_module}: ${submodule_partitions}")
#
#        foreach(submodule_partition ${submodule_partitions})
#            message(STATUS "Processing submodule partition: ${submodule_partition}")
#            get_filename_component(submodule_name ${submodule_partition} NAME_WE)
#            # Extract interface module name
#            string(REGEX REPLACE "\\..*$" "" interface_module_name ${interface_module})
#            # Add submodule partition
#            add_library(${submodule_partition} ${submodule_partition} module/${interface_module_name}.ixx)
#            target_link_libraries(${interface_module} PUBLIC ${submodule_partition})
#        endforeach()
#
#        # Check if the interface module exists
#        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/module/${interface_module}.ixx")
#            add_library(${interface_module} module/${interface_module}.ixx)
#        elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${interface_module}.ixx")
#            add_library(${interface_module} src/${interface_module}.ixx)
#        else ()
#            message(FATAL_ERROR "No source file found for interface module ${interface_module}")
#        endif ()
#    endforeach()
#endfunction()
#
#build_all_modules(
#        ancillarycat.api
#        ancillarycat.console
#        ancillarycat.entities
#        ancillarycat.utils
#        ancillarycat.game
#        ancillarycat.handler
#        ancillarycat.blocks
#)
