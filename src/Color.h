#pragma once

#include "SDL3/SDL_stdinc.h"

struct Color
{
    Uint8 red = 255;
    Uint8 green = 255;
    Uint8 blue = 255;

    bool operator==(const Color& other) const
    {
        return red == other.red && green == other.green && blue == other.blue;
    }
};