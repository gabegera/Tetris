#include "Theme.h"

#include <iostream>
#include <fstream>

#include "SDL3/SDL.h"
#include "battery/embed.hpp"

Theme::Theme()
{

}

Theme::~Theme()
{

}

void Theme::Init()
{
    constexpr bool autoDeleteIO = true;

    SDL_IOStream* textureIO = SDL_IOFromConstMem(b::embed<"res/Default_Block_Texture.png">().data(), b::embed<"res/Default_Block_Texture.png">().length());
    SDL_Surface* defaultTextureSurface = SDL_LoadPNG_IO(textureIO, autoDeleteIO);

    for (const Shape* shape : ClassicShapes::Get())
    {
        m_shapeSurfaces.try_emplace(shape, defaultTextureSurface);
    }
    m_borderSurface = defaultTextureSurface;

    SDL_IOStream* fontIO = SDL_IOFromConstMem(b::embed<"res/Default_Font.ttf">().data(), b::embed<"res/Default_Font.ttf">().length());
    m_font = TTF_OpenFontIO(fontIO, autoDeleteIO, m_fontSize);
}

Color Theme::GetShapeColor(const Shape* inShape) const
{
    try
    {
        return m_shapeColors.at(inShape);
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "Theme::GetShapeColor::Failed to get shape color" << std::endl;
        return Color(255, 255, 255);
    }
}

const Color& Theme::GetBorderColor() const
{
    return m_borderColor;
}

SDL_Surface* Theme::GetShapeSurface(const Shape* inShape) const
{
    auto it = m_shapeSurfaces.find(inShape);
    if (it != m_shapeSurfaces.end())
    {
        return it->second;
    }

    return nullptr;
}

SDL_Surface* Theme::GetBorderSurface() const
{
    return m_borderSurface;
}

TTF_Font* Theme::GetFont() const
{
    return m_font;
}

Uint8 Theme::GetFontSize() const
{
    return m_fontSize;
}

const Color& Theme::GetBackgroundColor() const
{
    return m_backgroundColor;
}
