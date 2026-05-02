#pragma once

#include <memory>
#include <vector>

#include "ColorPalettes.h"
#include "SDL3/SDL_stdinc.h"


class Game;
struct Shape;

struct Block
{
    Color color;
};

class BlockManager
{
public:
    explicit BlockManager(Game& game);
    ~BlockManager();

protected:

    Game& m_game;

    // Game Width in number of blocks.
    const Uint8 m_gameWidth = 10;
    // Game Height in number of blocks.
    const Uint8 m_gameHeight = 20;

    ColorPalette m_colorPalette = ColorPalettes::Classic;
    unsigned int m_colorPalletCounter = 0;

    // How many seconds until the falling blocks get lowered.
    float m_blockFallingRate = 0.5f;
    float m_blockFallingRateTracker = 0.0f;

    std::vector<unsigned int> m_fallingBlocks;

    std::vector<std::unique_ptr<Block>> m_blocks;

    // Converts a block's x and y position into an index.
    unsigned int GetBlockIndexFromPos(const Uint8 xPos, const Uint8 yPos) const;

    // Converts a block's index into an x and y position.
    std::pair<unsigned int, unsigned int> GetBlockPosFromIndex(const unsigned int index) const;

    void CreateShape(const Shape& inShape);

    /**
     * Creates a single block at a specified position.
     * @param xPos Number of blocks to the right of the origin.
     * @param yPos Number of blocks down from the origin.
     * @param color The 8-bit (0-255 values) color of the block.
     * @returns The Index of the newly created block.
     */
    unsigned int CreateBlock(const Uint8 xPos, const Uint8 yPos, const Color& color);

    /**
     * Moves a block to a new position.
     * @param targetBlockIndex The index of the block that should be moved.
     * @param newXPos Number of blocks to the right of the origin.
     * @param newYPos Number of blocks down from the origin.
     * @returns If Successful
     */
    bool MoveBlock(const unsigned int targetBlockIndex, const Uint8 newXPos, const Uint8 newYPos);

    /**
     * Deletes all blocks in the specified row.
     * @param rowYPos Number of blocks down from the origin.
     */
    void ClearLine(const unsigned int rowYPos);

public:
    void Init();
    void Update(const float deltaTime);

    /**
     * Shits the falling blocks either to the right or left by one.
     * @param direction -1 for left, 1 for right.
     */
    void MoveFallingBlocksHorizontal(int direction);
    void MoveFallingBlocksDown();

    void DropFallingBlocks();

    Uint8 GetGameWidth() const;
    Uint8 GetGameHeight() const;

    const ColorPalette& GetColorPalette() const;

    bool IsBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const;

    Block* GetBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const;

    std::vector<std::tuple<Block*, Uint8, Uint8>> GetAllBlocks() const;
};
