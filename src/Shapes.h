#pragma once

#include <random>
#include <string>
#include <vector>
#include <array>
#include <map>

#include "SDL3/SDL_stdinc.h"
#include "Position.h"

enum class RotationState : unsigned short
{
    Default = 0,
    Clockwise = 1, // The state the shape is in when rotated once clockwise from default.
    Double = 2, // The state the shape is in when rotated twice from default.
    CounterClockwise = 3 // The state the shape is in when rotated once counter-clockwise from default.
};

struct Shape
{
    Shape() = delete;

    /**
     * @param inName The name of the shape.
     * @param defaultState The state the shape will be in when created.
     * @param clockwiseState The state the shape will be in when rotated clockwise once from its default state.
     * @param counterClockwiseState The state the shape will be in when rotated counter-clockwise from its default state.
     * @param doubleRotationState  The state the shape will be in when rotated twice from its default state.
     */
    Shape(const std::string& inName,
          const std::vector<bool>& defaultState,
          const std::vector<bool>& clockwiseState,
          const std::vector<bool>& doubleRotationState,
          const std::vector<bool>& counterClockwiseState)
    : name(inName), rotationStates({defaultState, clockwiseState, doubleRotationState, counterClockwiseState})
    {
        const unsigned int matrixSize = rotationStates[0].size();
        for (int i = 0; i < rotationStates.size(); i++)
        {
            if (rotationStates[i].empty())
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". At least one of the rotation states was empty");
            }

            if (matrixSize != rotationStates[i].size())
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". Not all rotation states are the same dimensions.");
            }

            if (std::sqrt(rotationStates[i].size()) == std::floor(rotationStates[i].size()))
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". Make sure all rotation states have square dimensions.");
            }
        }
    };

    /**
     * @param inName The name of the shape.
     * @param defaultState The state the shape will be in when created.
     * @param clockwiseState The state the shape will be in when rotated clockwise once from its default state.
     * @param counterClockwiseState The state the shape will be in when rotated counter-clockwise from its default state.
     * @param inKickOffsets The offsets used to calculate wall kicks.
     * @param doubleRotationState  The state the shape will be in when rotated twice from its default state.
     */
    Shape(const std::string& inName,
          const std::vector<bool>& defaultState,
          const std::vector<bool>& clockwiseState,
          const std::vector<bool>& doubleRotationState,
          const std::vector<bool>& counterClockwiseState,
          const std::array<std::array<Position, 5> ,4>& inKickOffsets)
    : name(inName), rotationStates({defaultState, clockwiseState, doubleRotationState, counterClockwiseState}), kickOffsets(inKickOffsets)
    {
        const Uint8 matrixSize = rotationStates[0].size();
        for (int i = 0; i < rotationStates.size(); i++)
        {
            if (rotationStates[i].empty())
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". At least one of the rotation states was empty");
            }

            if (matrixSize != rotationStates[i].size())
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". Not all rotation states are the same dimensions.");
            }

            if (std::sqrt(rotationStates[i].size()) == std::floor(rotationStates[i].size()))
            {
                throw std::invalid_argument("Failed to create shape: " + name + ". Make sure all rotation states have square dimensions.");
            }
        }
    };

    const std::string name = "Unnamed Shape";

protected:
    const std::array<std::vector<bool>, 4> rotationStates;
    const std::array<std::array<Position, 5>, 4> kickOffsets =
    {{
        {{ Position(0, 0), Position(0, 0),  Position(0, 0),  Position(0, 0),   Position(0, 0) }},
        {{ Position(0, 0), Position(1, 0),  Position(1, 1),  Position(0, -2),  Position(1, -2) }},
        {{ Position(0, 0), Position(0, 0),  Position(0, 0),  Position(0, 0),   Position(0, 0) }},
        {{ Position(0, 0), Position(-1, 0), Position(-1, -1), Position(0, -2), Position(-1, -2) }}
    }};

public:

    // Returns the square width of the shape.
    unsigned int GetShapeDimensions() const
    {
        return std::sqrt(rotationStates[0].size());
    }

    std::vector<bool> GetRotationSateData(const RotationState inState) const
    {
        return rotationStates[static_cast<unsigned short>(inState)];
    }

    std::array<Position, 5> GetKickOffset(const RotationState inState) const
    {
        return kickOffsets[static_cast<unsigned short>(inState)];
    }
};

struct ClassicShapes
{
protected:
    inline static const std::vector<Shape> m_shapes =
    {
        Shape("L",   {0, 0, 1,
                      1, 1, 1,
                      0, 0, 0},

                     {0, 1, 0,
                      0, 1, 0,
                      0, 1, 1},

                     {0, 0, 0,
                      1, 1, 1,
                      1, 0, 0},

                     {1, 1, 0,
                      0, 1, 0,
                      0, 1, 0}),

        Shape("J",   {1, 0, 0,
                      1, 1, 1,
                      0, 0, 0},

                     {0, 1, 1,
                      0, 1, 0,
                      0, 1, 0},

                     {0, 0, 0,
                      1, 1, 1,
                      0, 0, 1},

                     {0, 1, 0,
                      0, 1, 0,
                      1, 1, 0}),

        Shape("S",   {0, 1, 1,
                      1, 1, 0,
                      0, 0, 0},

                     {0, 1, 0,
                      0, 1, 1,
                      0, 0, 1},

                     {0, 0, 0,
                      0, 1, 1,
                      1, 1, 0},

                     {1, 0, 0,
                      1, 1, 0,
                      0, 1, 0}),

        Shape("Z",   {1, 1, 0,
                      0, 1, 1,
                      0, 0, 0},

                     {0, 0, 1,
                      0, 1, 1,
                      0, 1, 0},

                     {0, 0, 0,
                      1, 1, 0,
                      0, 1, 1},

                     {0, 1, 0,
                      1, 1, 0,
                      1, 0, 0}),

        Shape("T",   {0, 1, 0,
                      1, 1, 1,
                      0, 0, 0},

                     {0, 1, 0,
                      0, 1, 1,
                      0, 1, 0},

                     {0, 0, 0,
                      1, 1, 1,
                      0, 1, 0},

                     {0, 1, 0,
                      1, 1, 0,
                      0, 1, 0}),

        Shape("I",   {0, 0, 0, 0,
                      1, 1, 1, 1,
                      0, 0, 0, 0,
                      0, 0, 0, 0},

                     {0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0,
                      0, 0, 1, 0},

                     {0, 0, 0, 0,
                      0, 0, 0, 0,
                      1, 1, 1, 1,
                      0, 0, 0, 0},

                     {0, 1, 0, 0,
                      0, 1, 0, 0,
                      0, 1, 0, 0,
                      0, 1, 0, 0},

                     {{
                     {{ Position(0, 0),   Position(-1, 0), Position(2, 0),   Position(-1, 0), Position(2, 0) }},
                     {{ Position(-1, 0),  Position(0, 0),  Position(0, 0),   Position(0, -1), Position(0, 2) }},
                     {{ Position(-1, -1), Position(1, -1), Position(-2, -1), Position(1, 0),  Position(-2, 0) }},
                     {{ Position(0, -1),  Position(0, -1), Position(0, -1),  Position(0, 1),  Position(-1, -2) }}
                     }}),

        Shape("O",   {1, 1,
                      1, 1,},

                     {1, 1,
                      1, 1,},

                     {1, 1,
                      1, 1,},

                     {1, 1,
                      1, 1,})
    };

public:
    inline static const Shape* L = &m_shapes[0];
    inline static const Shape* J = &m_shapes[1];
    inline static const Shape* S = &m_shapes[2];
    inline static const Shape* Z = &m_shapes[3];
    inline static const Shape* T = &m_shapes[4];
    inline static const Shape* I = &m_shapes[5];
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

    static unsigned int Num()
    {
        return m_shapes.size();
    }

};

