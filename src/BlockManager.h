#pragma once

#include <memory>
#include <vector>

#include "Shapes.h"
#include "SDL3/SDL_stdinc.h"

struct Shape;
class Game;

class BlockManager
{
public:
    explicit BlockManager(Game& game);
    ~BlockManager();

protected:
    Game& m_game;

    // How many seconds until the falling blocks get lowered.
    const float m_shapeFallingRate = 0.5f;
    float m_timeSinceShapeFell = 0.0f;

    // The delay until a falling block gets locked into place.
    const float m_lockDelay = 0.5f;
    float m_lockDelayTracker = 0.0f;
    const unsigned short m_numOfActionsBeforeLock = 15;
    unsigned short m_lockActionsCounter = 0;

    std::vector<const Shape*> m_shapesBag;
    std::vector<const Shape*> m_shapesQueue;

    std::vector<unsigned int> m_fallingBlockIndices;

    const Shape* m_fallingShape = nullptr;
    Position m_fallingShapePosition;
    RotationState m_fallingShapeRotationState = RotationState::Default;

    const Shape* m_heldShape = nullptr;
    bool m_canHold = true;

    std::vector<const Shape*> m_blocks;

    unsigned int GetIndexFromPos(Position inPos) const;
    unsigned int GetXPosFromIndex(unsigned int index) const;
    unsigned int GetYPosFromIndex(unsigned int index) const;
    Position GetPosFromIndex(unsigned int index) const;

    void FillShapesBag();
    void FillShapesQueue();

    bool CreateBlockAtPos(Position targetPos, const Shape* owningShape);
    bool CreateBlockAtIndex(unsigned int index, const Shape* owningShape);

    void DeleteBlockAtPos(Position targetPos);
    void DeleteBlockAtIndex(unsigned int index);

    void DeleteShape();

    bool MoveBlockAtPos(Position currentPos, Position targetPos);
    bool MoveBlockAtIndexToPos(unsigned int currentIndex, Position targetPos);
    bool MoveBlockAtPosToIndex(Position currentPos, unsigned int targetIndex);
    bool MoveBlockAtIndex(unsigned int currentIndex, unsigned int targetIndex);

    // Creates a shape where the top left corner will reside at the target position.
    bool CreateShapeAtPos(Position targetPos, const Shape* shape, RotationState shapeRotationState = RotationState::Default);
    // Creates a shape at the very top center of the game borders.
    bool CreateShapeAtTopCenter(const Shape* shape);
    // Creates the next shape in queue at the very top center of the game borders.
    bool CreateNextShapeInQueue();

    void ClearLine(unsigned int yPos);
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
    void RotateShapeClockwise();
    void RotateShapeCounterClockwise();
    void HoldShape();

    void ResetLockDelay();

    std::vector<std::pair<Position, const Shape*>> GetAllBlocks() const;
    std::vector<std::pair<Position, const Shape*>> GetFallingShapeBlocks() const;

    const Shape* GetFallingShape() const;
    Position GetFallingShapePosition() const;
    RotationState GetFallingShapeRotationState() const;

    bool IsBlockAtPos(Position inPos) const;
    bool IsBlockAtIndex(unsigned int index) const;

    bool IsFallingBlockAtPos(Position inPos) const;
    bool IsFallingBlockAtIndex(unsigned int index) const;

    bool IsBlockBelowPos(Position inPos) const;
    bool IsBlockBelowIndex(unsigned int index) const;

    bool IsFallingBlockBelowPos(Position inPos) const;
    bool IsFallingBlockBelowIndex(unsigned int index) const;

    bool IsBlockLeftOfPos(Position inPos) const;
    bool IsBlockLeftOfIndex(unsigned int index) const;

    bool IsFallingBlockLeftOfPos(Position inPos) const;
    bool IsFallingBlockLeftOfIndex(unsigned int index) const;

    bool IsBlockRightOfPos(Position inPos) const;
    bool IsBlockRightOfIndex(unsigned int index) const;

    bool IsFallingBlockRightOfPos(Position inPos) const;
    bool IsFallingBlockRightOfIndex(unsigned int index) const;

    bool IsBlockAtBottomBorder(unsigned int yPos) const;
    bool IsIndexAtBottomBorder(unsigned int index) const;

    bool IsBlockAtLeftBorder(unsigned int xPos) const;
    bool IsIndexAtLeftBorder(unsigned int index) const;

    bool IsBlockAtRightBorder(unsigned int xPos) const;
    bool IsIndexAtRightBorder(unsigned int index) const;

    bool CanFallingShapeMoveDown() const;

    bool HasLockDelayBeenPassed() const;
    bool HasLockActionsBeenPassed() const;

    bool IsPositionInBounds(Position inPos) const;

    bool CanLineBeCleared(unsigned int yPos) const;

    bool CanShapeBeCreatedAtPos(Position targetPos, const Shape* shape, RotationState rotationState = RotationState::Default) const;

    Position GetShapeTargetPos() const;

     /** @returns The Y positions of all clearable lines in the game. */
    std::vector<unsigned int> GetClearableLines() const;
};
