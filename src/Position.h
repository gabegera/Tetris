#pragma once

struct Position
{
    Position() {};

    Position(const int inX, const int inY) : x(inX), y(inY) {};

    int x = 0;
    int y = 0;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    Position operator+(const Position& other) const
    {
        return Position(x + other.x, y + other.y);
    }

    Position operator-(const Position& other) const
    {
        return Position(x - other.x, y - other.y);
    }

    Position operator+=(const Position& other)
    {
        x += other.x;
        y += other.y;
        return Position(x, y);
    }

    Position operator-=(const Position& other)
    {
        x -= other.x;
        y -= other.y;
        return Position(x, y);
    }
};
