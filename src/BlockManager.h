#pragma once

#include <memory>
#include <vector>

#include "SDL3/SDL_stdinc.h"

struct Shape;
class Game;

struct Block
{
    explicit Block(const Shape* inShape) : m_owningShape(inShape) {}

    const Shape* m_owningShape;
};

class BlockManager
{
public:
    explicit BlockManager(Game& game);
    ~BlockManager();

protected:
    Game& m_game;

    // How many times in each direction the shape is allowed to be nudged when rotating.
    const Uint8 m_maxNumberOfRotationNudges = 3;

    // How many seconds until the falling blocks get lowered.
    const float m_shapeFallingRate = 0.5f;
    float m_timeSinceShapeFell = 0.0f;

    std::vector<const Shape*> m_shapesBag;
    std::vector<const Shape*> m_shapesQueue;

    std::vector<Uint16> m_fallingBlockIndices;
    const Shape* m_fallingShape = nullptr;

    std::vector<std::unique_ptr<Block>> m_blocks;

    Uint16 GetBlockIndexFromPos(Uint8 xPos, Uint8 yPos) const;
    Uint8 GetBlockXPosFromIndex(Uint16 index) const;
    Uint8 GetBlockYPosFromIndex(Uint16 index) const;
    void GetBlockPosFromIndex(Uint16 index, Uint8& outXPos, Uint8& outYPos) const;

    void FillShapesBag();
    void FillShapesQueue();

    Uint16 CreateBlockAtPos(Uint8 xPos, Uint8 yPos, const Shape* owningShape);
    Uint16 CreateBlockAtIndex(Uint16 index, const Shape* owningShape);

    void DeleteBlockAtPos(Uint8 xPos, Uint8 yPos);
    void DeleteBlockAtIndex(Uint16 index);

    void DeleteShape();

    bool MoveBlockAtPos(Uint8 xPos, Uint8 yPos, Uint8 newXPos, Uint8 newYPos);
    bool MoveBlockAtIndexToPos(Uint16 index, Uint8 newXPos, Uint8 newYPos);
    bool MoveBlockAtPosToIndex(Uint8 xPos, Uint8 yPos, Uint16 newIndex);
    bool MoveBlockAtIndex(Uint16 index, Uint16 newIndex);

    // Creates a shape where the top left corner will reside at the target position.
    bool CreateShapeAtPos(Uint8 xPos, Uint8 yPos, const Shape* shape);
    // Creates a shape at the very top center of the game borders.
    bool CreateShapeAtTopCenter(const Shape* shape);
    // Creates the next shape in queue at the very top center of the game borders.
    bool CreateNextShapeInQueue();

    void ClearLine(Uint8 yPos);
    void ClearClearableLines();

    void SortFallingBlockIndicesAscending();
    void SortFallingBlockIndicesDescending();

public:
    void Init();
    void Update(float deltaTime);

    void Reset();

    void MoveShapeLeft();
    void MoveShapeRight();
    bool MoveShapeDown();
    void DropShape();
    void RotateShape();

    void GetAllBlocks(std::vector<Uint8>& xPositions, std::vector<Uint8>& yPositions, std::vector<Block*>& blockPtrs) const;
    void GetFallingShapeBlocks(std::vector<Uint8>& xPositions, std::vector<Uint8>& yPositions, std::vector<Block*>& blockPtrs) const;

    const Shape* GetFallingShape() const;

    bool IsBlockAtPos(Uint8 xPos, Uint8 yPos) const;
    bool IsBlockAtIndex(Uint16 index) const;

    bool IsFallingBlockAtPos(Uint8 xPos, Uint8 yPos) const;
    bool IsFallingBlockAtIndex(Uint16 index) const;

    bool IsBlockBelowPos(Uint8 xPos, Uint8 yPos) const;
    bool IsBlockBelowIndex(Uint16 index) const;

    bool IsFallingBlockBelowPos(Uint8 xPos, Uint8 yPos) const;
    bool IsFallingBlockBelowIndex(Uint16 index) const;

    bool IsBlockLeftOfPos(Uint8 xPos, Uint8 yPos) const;
    bool IsBlockLeftOfIndex(Uint16 index) const;

    bool IsFallingBlockLeftOfPos(Uint8 xPos, Uint8 yPos) const;
    bool IsFallingBlockLeftOfIndex(Uint16 index) const;

    bool IsBlockRightOfPos(Uint8 xPos, Uint8 yPos) const;
    bool IsBlockRightOfIndex(Uint16 index) const;

    bool IsFallingBlockRightOfPos(Uint8 xPos, Uint8 yPos) const;
    bool IsFallingBlockRightOfIndex(Uint16 index) const;

    bool IsBlockAtBottomBorder(Uint8 yPos) const;
    bool IsIndexAtBottomBorder(Uint16 index) const;

    bool IsBlockAtLeftBorder(Uint8 xPos) const;
    bool IsIndexAtLeftBorder(Uint16 index) const;

    bool IsBlockAtRightBorder(Uint8 xPos) const;
    bool IsIndexAtRightBorder(Uint16 index) const;

    bool CanLineBeCleared(Uint8 yPos) const;

    bool CanShapeBeCreatedAtPos(Uint8 xPos, Uint8 yPos, const Shape* shape) const;

    void GetShapeTargetPos(Uint8& outXPos, Uint8& outYPos);

    /**
     * @returns Vector containing Y positions of all clearable lines in the game.
     */
    std::vector<Uint8> GetClearableLines() const;
};
