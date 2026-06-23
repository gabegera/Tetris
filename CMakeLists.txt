cmake_minimum_required(VERSION 4.2)
project(Tetris)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(BUILD_SHARED_LIBS OFF)
set(SDLTTF_VENDORED ON)
set(SDLTTF_HARFBUZZ OFF)

include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
        GIT_TAG         release-3.4.8
        GIT_SHALLOW     TRUE       # only fetch the target commit, not full history
#        SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/SDL3"
#        BINARY_DIR "${FETCHCONTENT_BASE_DIR}/SDL3/build"
)
set(SDL_STATIC ON)
FetchContent_MakeAvailable(SDL3)
#file(REMOVE_RECURSE "${FETCHCONTENT_BASE_DIR}/sdl3-tmp")

FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
        GIT_TAG         release-3.4.4
        GIT_SHALLOW     TRUE       # only fetch the target commit, not full history
#        SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/SDL_image"
#        BINARY_DIR "${FETCHCONTENT_BASE_DIR}/SDL_image/build"
)
FetchContent_MakeAvailable(SDL_image)
#file(REMOVE_RECURSE "${FETCHCONTENT_BASE_DIR}/sdl_image-tmp")

FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG         release-3.2.2
        GIT_SHALLOW     TRUE       # only fetch the target commit, not full history
#        SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/SDL_ttf"
#        BINARY_DIR "${FETCHCONTENT_BASE_DIR}/SDL_ttf/build"
)
FetchContent_MakeAvailable(SDL_ttf)
#file(REMOVE_RECURSE "${FETCHCONTENT_BASE_DIR}/SDL_ttf-tmp")

FetchContent_Declare(
        battery-embed
        GIT_REPOSITORY https://github.com/batterycenter/embed.git
        GIT_TAG        v1.2.19
)
FetchContent_MakeAvailable(battery-embed)

add_executable(Tetris WIN32 src/main.cpp
        src/Game.cpp
        src/Game.h
        src/BlockManager.cpp
        src/BlockManager.h
        src/Shapes.h
        src/Renderer.cpp
        src/Renderer.h
        src/Application.cpp
        src/Application.h
        src/Application.cpp
        src/Application.h
        src/UI/Menu.cpp
        src/UI/Menu.h
        src/UI/Button.cpp
        src/UI/Button.h
        src/UI/MainMenu.cpp
        src/UI/MainMenu.h
        src/UI/UIElement.cpp
        src/UI/UIElement.h
        src/UI/TextBlock.cpp
        src/UI/TextBlock.h
        src/Theme.h
        src/Theme.cpp
        src/Color.h
        src/UI/UIAlignment.h
        src/UI/SettingsMenu.cpp
        src/UI/SettingsMenu.h
        src/GameboyTheme.cpp
        src/GameboyTheme.h
        src/InputHandler.cpp
        src/InputHandler.h
        src/InputAction.cpp
        src/InputAction.h
)

target_link_libraries(Tetris PRIVATE
        SDL3::SDL3-static
        SDL3_image::SDL3_image-static
        SDL3_ttf::SDL3_ttf-static
)

target_compile_definitions(Tetris PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>
        $<$<CONFIG:Release>:NDEBUG>
)

b_embed(Tetris res/Default-Block-Texture.png)
b_embed(Tetris res/Default-Font.ttf)

b_embed(Tetris res/Gameboy/I-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/J-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/L-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/O-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/S-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/T-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/Z-Texture-Gameboy.png)
b_embed(Tetris res/Gameboy/Border-Texture-Gameboy.png)