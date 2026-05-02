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

    std::vector<Shape> m_shapesQueue;

    std::vector<unsigned int> m_fallingBlocks;

    std::vector<std::unique_ptr<Block>> m_blocks;

    // Converts a block's x and y position into an index.
    unsigned int GetBlockIndexFromPos(Uint8 xPos, Uint8 yPos) const;

    // Converts a block's index into an x and y position.
    std::pair<unsigned int, unsigned int> GetBlockPosFromIndex(unsigned int index) const;

    // Creates blocks in a shape at the top middle of the screen.
    void CreateShape(const Shape& inShape);

    void CreateNextShapeInQueue();

    /**
     * Creates a single block at a specified position.
     * @param xPos Number of blocks to the right of the origin.
     * @param yPos Number of blocks down from the origin.
     * @param color The 8-bit (0-255 values) color of the block.
     * @returns The Index of the newly created block.
     */
    unsigned int CreateBlock(Uint8 xPos, Uint8 yPos, const Color& color);

    /**
     * Moves a block to a new position.
     * @param targetBlockIndex The index of the block that should be moved.
     * @param newXPos Number of blocks to the right of the origin.
     * @param newYPos Number of blocks down from the origin.
     * @returns If Successful
     */
    bool MoveBlock(unsigned int targetBlockIndex, Uint8 newXPos, Uint8 newYPos);

    /**
     * Deletes all blocks in the specified row.
     * @param rowYPos Number of blocks down from the origin.
     */
    void ClearLine(unsigned int rowYPos);

    void StopFallingBlocks();

public:
    void Init();
    void Update(float deltaTime);

    /**
     * Shifts the falling blocks either to the right or left by 1 position.
     * @param direction -1 for left, 1 for right.
     * @returns Was Successful.
     */
    bool MoveFallingBlocksHorizontal(int direction);

    /**
     * Shifts the falling blocks down by 1 position.
     * @returns Was Successful.
     */
    bool MoveFallingBlocksDown();

    void DropFallingBlocks();

    Uint8 GetGameWidth() const;
    Uint8 GetGameHeight() const;

    const ColorPalette& GetColorPalette() const;

    bool IsBlockAtPosition(Uint8 xPos, Uint8 yPos) const;

    Block* GetBlockAtPosition(Uint8 xPos, Uint8 yPos) const;

    /**
     * Gets all blocks that currently exist in the game.
     * @returns Vector of tuples containing a pointer to the block, it's X-Position, and it's Y-Position.
     */
    std::vector<std::tuple<Block*, Uint8, Uint8>> GetAllBlocks() const;

    /**
     * Returns true if the falling blocks can move in the specified direction by 1 position.
     * @param direction -1 for left, 1 for right.
     */
    bool CanFallingBlocksMoveHorizontal(int direction) const;

    // Returns true if the falling blocks can move down by 1 position.
    bool CanFallingBlocksMoveDown() const;

    bool CanLineBeCleared(Uint8 lineYPos) const;
};
