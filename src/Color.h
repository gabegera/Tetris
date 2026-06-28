#pragma once

#include "SDL3/SDL_stdinc.h"

struct Color
{
    Color() = delete;
    Color(const Uint8 inRed, const Uint8 inGreen, const Uint8 inBlue) : red(inRed), green(inGreen), blue(inBlue) {}
    Color(const Uint8 inRed, const Uint8 inGreen, const Uint8 inBlue, const Uint8 inAlpha) : red(inRed), green(inGreen), blue(inBlue), alpha(inAlpha) {}

    Uint8 red = 255;
    Uint8 green = 255;
    Uint8 blue = 255;
    Uint8 alpha = 255;

    bool operator==(const Color& other) const
    {
        return red == other.red && green == other.green && blue == other.blue;
    }
};