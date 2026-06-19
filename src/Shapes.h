#pragma once

#include <random>
#include <string>

#include "SDL3/SDL_stdinc.h"

struct Shape
{
    Uint8 width;

    std::string blocks;

    Shape(const Uint8 inWidth, const std::string& inBlocks) : width(inWidth), blocks(inBlocks)
    {

    }

    Uint8 GetHeight() const
    {
        return std::ceil((blocks.size() * 1.0f) / width);
    }

    bool operator==(const Shape& other) const
    {
        return width == other.width && blocks == other.blocks;
    }
};

struct ClassicShapes
{
protected:
    inline static const std::vector<Shape> m_shapes =
    {
        {2, "x "    // L
            "x "
            "xx"},

        {2, " x"    // J
            " x"
            "xx"},

        {4, "xxxx"}, // I

        {3, " xx"   // S
            "xx "},

        {3, "xx "   // Z
            " xx"},

        {3, " x "   // T
            "xxx"},

        {2, "xx"    // O
            "xx"}
    };

public:
    inline static const Shape* L = &m_shapes[0];
    inline static const Shape* J = &m_shapes[1];
    inline static const Shape* I = &m_shapes[2];
    inline static const Shape* S = &m_shapes[3];
    inline static const Shape* Z = &m_shapes[4];
    inline static const Shape* T = &m_shapes[5];
    inline static const Shape* O = &m_shapes[6];

    // Get a vector of pointers to the shapes.
    static std::vector<const Shape*> Get()
    {
        std::vector<const Shape*> out(m_shapes.size());
        for (int i = 0; i < m_shapes.size(); i++)
        {
            out[i] = &m_shapes[i];
        }
        return out;
    }

    static Uint8 Num()
    {
        return m_shapes.size();
    }

};

