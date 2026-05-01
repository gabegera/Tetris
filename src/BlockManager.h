#pragma once

#include <cstdint>
#include <memory>
#include <vector>


class Game;
struct Shape;

struct Block
{
    Block(const uint8_t redTint, const uint8_t greenTint, const uint8_t blueTint)
    {
        red = redTint;
        green = greenTint;
        blue = blueTint;
    }

    Block()
    {
        red = 255;
        green = 255;
        blue = 255;
    }

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class BlockManager
{
public:
    explicit BlockManager(const Game& game);
    ~BlockManager();

protected:

    const Game& m_game;

    std::vector<Block*> m_fallingBlocks;

    std::vector<std::unique_ptr<Block>> m_blocks;

    unsigned int GetBlockIndexFromPos(const unsigned int xPos, const unsigned int yPos) const;
    std::pair<unsigned int, unsigned int> GetBlockPosFromIndex(const unsigned int index) const;

    void CreateShape(const Shape& inShape);
    void CreateBlock(const uint8_t xPos, const uint8_t yPos, const uint8_t red = 255, const uint8_t green = 255, const uint8_t blue = 255);

    void ClearLine(int yPos);

public:
    void Update();

    bool IsBlockAtPosition(const uint8_t xPos, const uint8_t yPos) const;

    Block* GetBlockAtPosition(const uint8_t xPos, const uint8_t yPos) const;

    std::vector<std::tuple<Block*, unsigned int, unsigned int>> GetAllBlocks() const;
};
