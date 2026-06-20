#include "GameboyTheme.h"

#include "battery/embed.hpp"

GameboyTheme::GameboyTheme()
{

}

GameboyTheme::~GameboyTheme()
{

}

void GameboyTheme::Init()
{
    m_themeName = "Gameboy";

    constexpr bool autoDeleteIO = false;

    SDL_IOStream* textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/Border-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/Border-Texture-Gameboy.png">().length());
    m_borderSurface = SDL_LoadPNG_IO(textureIO, autoDeleteIO);

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/I-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/I-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::I, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/J-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/J-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::J, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/L-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/L-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::L, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/O-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/O-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::O, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/S-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/S-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::S, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/T-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/T-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::T, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    textureIO = SDL_IOFromConstMem(b::embed<"res/Gameboy/Z-Texture-Gameboy.png">().data(), b::embed<"res/Gameboy/Z-Texture-Gameboy.png">().length());
    m_shapeSurfaces.emplace(ClassicShapes::Z, SDL_LoadPNG_IO(textureIO, autoDeleteIO));

    m_backgroundColor = Color(215, 255, 200);
    constexpr Color blockColor(170, 207, 158);
    m_borderColor = blockColor;
    for (auto shape : ClassicShapes::Get())
    {
        m_shapeColors[shape] = blockColor;
    }

    SDL_CloseIO(textureIO);

    SDL_IOStream* fontIO = SDL_IOFromConstMem(b::embed<"res/Default-Font.ttf">().data(), b::embed<"res/Default-Font.ttf">().length());
    m_font = TTF_OpenFontIO(fontIO, autoDeleteIO, m_fontSize);
}
