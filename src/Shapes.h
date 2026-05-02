#pragma once

#include <string>

struct Shape
{
    Uint8 width;

    std::string blocks;
};

struct Shapes
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
};

