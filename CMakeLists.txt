cmake_minimum_required(VERSION 4.2)
project(Tetris)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(FetchContent)

find_package(SDL QUIET)
if (NOT SDL_FOUND)
    FetchContent_Declare(
            SDL
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.4.4
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL/src"
            BINARY_DIR "${CMAKE_BINARY_DIR}/lib/SDL/build"
    )
    FetchContent_MakeAvailable(SDL)
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/lib/SDL-tmp")
endif ()

find_package(SDL_image QUIET)
if (NOT SDL_image_FOUND)
    FetchContent_Declare(
            SDL_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-3.4.2
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL_image/src"
            BINARY_DIR "${CMAKE_BINARY_DIR}/lib/SDL_image/build"
    )
    FetchContent_MakeAvailable(SDL_image)
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/lib/SDL_image-tmp")
endif ()

add_executable(Tetris src/main.cpp
        src/Game.cpp
        src/Game.h
        src/BlockManager.cpp
        src/BlockManager.h
        src/Shapes.h
        src/Renderer.cpp
        src/Renderer.h)

target_link_libraries(Tetris PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)