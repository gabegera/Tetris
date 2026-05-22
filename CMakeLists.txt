cmake_minimum_required(VERSION 4.2)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(BUILD_SHARED_LIBS OFF)
set(SDLTTF_VENDORED ON)

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
    set(SDL_SHARED OFF)
    set(SDL_STATIC ON)
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

find_package(SDL_ttf QUIET)
if (NOT SDL_ttf_FOUND)
    FetchContent_Declare(
            SDL_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-3.2.2
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL_ttf/src"
            BINARY_DIR "${CMAKE_BINARY_DIR}/lib/SDL_ttf/build"
    )
    FetchContent_MakeAvailable(SDL_ttf)
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/lib/SDL_ttf-tmp")
endif ()

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.12.0
            SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/nlohmann_json/src"
            BINARY_DIR "${CMAKE_BINARY_DIR}/lib/nlohmann_json/build"
    )
    FetchContent_MakeAvailable(nlohmann_json)
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/lib/nlohmann_json-tmp")
endif ()

add_executable(Tetris WIN32 src/main.cpp
        src/Game.cpp
        src/Game.h
        src/BlockManager.cpp
        src/BlockManager.h
        src/Shapes.h
        src/Renderer.cpp
        src/Renderer.h
        src/ColorPalettes.h
        src/Application.cpp
        src/Application.h
        src/Application.cpp
        src/Application.h)

target_link_libraries(Tetris PRIVATE
        SDL3::SDL3-static
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        nlohmann_json::nlohmann_json)