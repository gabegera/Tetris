#pragma once

#include <map>
#include <memory>
#include <filesystem>

#include "Color.h"
#include "Shapes.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

class Theme
{
public:
    Theme();
    virtual ~Theme();

protected:
    std::string m_themeName = "Classic";

    std::map<const Shape*, Color> m_shapeColors =
    {
        {ClassicShapes::I, Color(0, 230, 255)},
        {ClassicShapes::J, Color(25, 0, 255)},
        {ClassicShapes::L, Color(255, 115, 0)},
        {ClassicShapes::O, Color(255, 225, 0)},
        {ClassicShapes::S, Color(100, 255, 0)},
        {ClassicShapes::Z, Color(255, 0, 0)},
        {ClassicShapes::T, Color(185, 0, 255)},
    };
    Color m_borderColor = Color(150, 150, 150);
    Color m_backgroundColor = Color(0, 0, 0);

    std::map<const Shape*, SDL_Surface*> m_shapeSurfaces;
    SDL_Surface* m_borderSurface;

    TTF_Font* m_font;
    Uint8 m_fontSize = 16;

public:
    virtual void Init();

    const std::string& GetName() const;

    Color GetShapeColor(const Shape* inShape) const;
    const Color& GetBorderColor() const;

    const Color& GetBackgroundColor() const;

    SDL_Surface* GetShapeSurface(const Shape* inShape) const;
    SDL_Surface* GetBorderSurface() const;

    TTF_Font* GetFont() const;
    Uint8 GetFontSize() const;
};
