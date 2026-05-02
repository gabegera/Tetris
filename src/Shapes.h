#pragma once

#include <random>
#include <string>

struct Shape
{
    Uint8 width;

    std::string blocks;
};

struct ClassicShapes
{
    static constexpr Shape L = {2, "x "
                                   "x "
                                   "xx"};

    static constexpr Shape J = {2, " x"
                                   " x"
                                   "xx"};

    static constexpr Shape I = {1, "x"
                                   "x"
                                   "x"
                                   "x"};

    static constexpr Shape S = {3, " xx"
                                   "xx "};

    static constexpr Shape Z = {3, "xx "
                                   " xx"};

    static constexpr Shape T = {3, "xxx"
                                   " x "};

    static constexpr Shape O = {2, "xx"
                                   "xx"};

    static constexpr unsigned int numOfShapes = 7;

    static const Shape& GetRandomShape()
    {
        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> range(1, numOfShapes);

        switch (const int randomNum = range(gen))
        {
            case 1:
                return L;
            case 2:
                return J;
            case 3:
                return I;
            case 4:
                return S;
            case 5:
                return Z;
            case 6:
                return T;
            case 7:
                return O;
            default:
                return O;
        }
    }
};

