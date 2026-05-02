#pragma once

#include <string>
#include <vector>

#include "SDL3/SDL_stdinc.h"

struct Color
{
    Uint8 red = 255;
    Uint8 green = 255;
    Uint8 blue = 255;
};

struct ColorPalette
{
    std::string name;

    std::vector<Color> blockColors;

    Color borderColor;
};

struct ColorPalettes
{
    inline static const ColorPalette Classic =
    {
        "Classic",
        {
            {255, 0, 0},    // red
            {0, 255, 0},    // green
            {0, 0, 255},    // blue
            {0, 255, 255},  // teal
            {255, 255, 0},  // yellow
            {255, 137, 0},  // orange
            {205, 0, 255}   // purple
        },
        {150, 150, 150} // grey borders.
    };
};