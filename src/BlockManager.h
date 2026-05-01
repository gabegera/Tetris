#pragma once

#include <memory>
#include <vector>

#include "SDL3/SDL_stdinc.h"


class Game;
struct Shape;

struct Block
{
    Block(const Uint8 redTint, const Uint8 greenTint, const Uint8 blueTint)
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

    Uint8 red;
    Uint8 green;
    Uint8 blue;
};

class BlockManager
{
public:
    explicit BlockManager(Game& game);
    ~BlockManager();

protected:

    Game& m_game;

    std::vector<unsigned int> m_fallingBlocks;

    std::vector<std::unique_ptr<Block>> m_blocks;

    unsigned int GetBlockIndexFromPos(const Uint8 xPos, const Uint8 yPos) const;
    std::pair<unsigned int, unsigned int> GetBlockPosFromIndex(const unsigned int index) const;

    void CreateShape(const Shape& inShape);
    void CreateBlock(const Uint8 xPos, const Uint8 yPos, const Uint8 red = 255, const Uint8 green = 255, const Uint8 blue = 255);

    bool MoveBlock(const unsigned int targetBlockIndex, const Uint8 newXPos, const Uint8 newYPos);

    void ClearLine(int yPos);

public:
    void Init();

    void Stop();

    void Update();

    bool IsBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const;

    Block* GetBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const;

    std::vector<std::tuple<Block*, Uint8, Uint8>> GetAllBlocks() const;
};
