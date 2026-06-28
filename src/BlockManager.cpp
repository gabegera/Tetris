#include "BlockManager.h"

#include <algorithm>
#include <iostream>
#include <map>

#include "Application.h"
#include "Game.h"
#include "Shapes.h"

BlockManager::BlockManager(Game& game)
    : m_game(game), m_blocks(game.GetGameWidth() * game.GetGameHeight())
{
    m_shapesQueue.reserve(ClassicShapes::Num());
}

BlockManager::~BlockManager()
{
}

unsigned int BlockManager::GetIndexFromPos(const Position inPos) const
{
    return inPos.x + (inPos.y * m_game.GetGameWidth());
}

unsigned int BlockManager::GetXPosFromIndex(const unsigned int index) const
{
    return index % m_game.GetGameWidth();
}

unsigned int BlockManager::GetYPosFromIndex(const unsigned int index) const
{
    return std::floor(index / m_game.GetGameWidth());
}

Position BlockManager::GetPosFromIndex(const unsigned int index) const
{
    return Position(GetXPosFromIndex(index), GetYPosFromIndex(index));
}

void BlockManager::FillShapesBag()
{
    m_shapesBag = ClassicShapes::Get();
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::ranges::shuffle(m_shapesBag, gen);
}

void BlockManager::FillShapesQueue()
{
    const unsigned int emptySpacesInQueue = m_shapesQueue.capacity() - m_shapesQueue.size();
    for (int i = 0; i < emptySpacesInQueue; i++)
    {
        if (m_shapesBag.empty())
        {
            FillShapesBag();
        }

        m_shapesQueue.emplace_back(m_shapesBag.back());
        m_shapesBag.pop_back();
    }
}

bool BlockManager::CreateBlockAtPos(const Position targetPos, const Shape* owningShape)
{
    return CreateBlockAtIndex(GetIndexFromPos(targetPos), owningShape);
}

bool BlockManager::CreateBlockAtIndex(const unsigned int index, const Shape* owningShape)
{
    try
    {
        m_blocks.at(index) = owningShape;
        return true;
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "BlockManager::CreateBlockAtIndex::Failed to create block at index. Index is out of range." << std::endl;
        return false;
    }
}

void BlockManager::DeleteBlockAtPos(const Position targetPos)
{
    DeleteBlockAtIndex(GetIndexFromPos(targetPos));
}

void BlockManager::DeleteBlockAtIndex(const unsigned int index)
{
    if (index > m_blocks.size())
    {
        std::cerr << "BlockManager::DeleteBlockAtIndex::Invalid target index." << std::endl;
    }

    m_blocks[index] = nullptr;
}

void BlockManager::DeleteShape()
{
    if (m_fallingBlockIndices.empty())
    {
        std::cerr << "BlockManager::DeleteShape::No shape to destroy." << std::endl;
        return;
    }

    for (const unsigned int index : m_fallingBlockIndices)
    {
        DeleteBlockAtIndex(index);
    }

    m_fallingBlockIndices.clear();
    m_fallingShape = nullptr;
    m_fallingShapePosition = Position();
}

bool BlockManager::MoveBlockAtPos(const Position currentPos, const Position targetPos)
{
    return MoveBlockAtIndex(GetIndexFromPos(currentPos), GetIndexFromPos(targetPos));
}

bool BlockManager::MoveBlockAtIndexToPos(const unsigned int currentIndex, const Position targetPos)
{
    return MoveBlockAtIndex(currentIndex, GetIndexFromPos(targetPos));
}

bool BlockManager::MoveBlockAtPosToIndex(const Position currentPos, const unsigned int targetIndex)
{
    return MoveBlockAtIndex(GetIndexFromPos(currentPos), targetIndex);
}

bool BlockManager::MoveBlockAtIndex(const unsigned int currentIndex, const unsigned int targetIndex)
{
    if (m_blocks[targetIndex] != nullptr)
    {
        std::cerr << "BlockManager::MoveBlockAtIndex::Can't move block, target index is occupied." << std::endl;
        return false;
    }

    if (m_blocks[currentIndex] == nullptr)
    {
        std::cerr << "BlockManager::MoveBlockAtIndex::Target Block Index is null.";
        return false;
    }

    // If the block being moved is a falling block, update it's stored index.
    if (std::ranges::contains(m_fallingBlockIndices, currentIndex))
    {
        for (int i = 0; i < m_fallingBlockIndices.size(); i++)
        {
            if (m_fallingBlockIndices[i] == currentIndex)
            {
                m_fallingBlockIndices[i] = targetIndex;
            }
        }
    }

    m_blocks[targetIndex] = m_blocks[currentIndex];
    m_blocks[currentIndex] = nullptr;
    return true;
}

bool BlockManager::CreateShapeAtPos(const Position targetPos, const Shape* shape, const RotationState shapeRotationState)
{
    if (targetPos.x < 0 || targetPos.y < 0)
    {
        std::cerr << "BlockManager::CreateShapeAtPos::Received negative position(s). Make sure input is only positive." << std::endl;
        return false;
    }

    const std::vector<bool> stateShape = shape->GetRotationSateData(shapeRotationState);
    const unsigned int shapeWidth = shape->GetShapeDimensions();

    std::vector<Position> createdBlocks;
    createdBlocks.reserve(stateShape.size());

    m_fallingBlockIndices.clear();

    for (int i = 0; i < stateShape.size(); i++)
    {
        if (stateShape[i] == false) continue;

        Position targetBlockPos;

        targetBlockPos.x = targetPos.x + i % shapeWidth;
        targetBlockPos.y = targetPos.y + std::floor(i / shapeWidth);

        if (!IsPositionInBounds(targetBlockPos) || (IsBlockAtPos(targetBlockPos) && !IsFallingBlockAtPos(targetBlockPos)))
        {
            for (const Position pos : createdBlocks)
            {
                DeleteBlockAtPos(pos);
            }

            m_fallingShape = nullptr;

            // std::cerr << "BlockManager::CreateShapeAtPos::Failed to create shape, obstructed." << std::endl;
            return false;
        }

        createdBlocks.emplace_back(targetBlockPos);
        CreateBlockAtPos(targetBlockPos, shape);
        m_fallingBlockIndices.push_back(GetIndexFromPos(targetBlockPos));
    }

    m_fallingShape = shape;
    m_fallingShapeRotationState = shapeRotationState;
    m_fallingShapePosition = targetPos;

    return true;
}

bool BlockManager::CreateShapeAtTopCenter(const Shape* shape)
{
    return CreateShapeAtPos(Position((m_game.GetGameWidth() / 2) - (std::floor(shape->GetShapeDimensions() / 2)) , 0), shape);
}

bool BlockManager::CreateNextShapeInQueue()
{
    if (m_shapesQueue.empty())
    {
        FillShapesQueue();
    }

    if (CreateShapeAtTopCenter(m_shapesQueue.front()))
    {
        m_shapesQueue.erase(m_shapesQueue.begin());
        m_canHold = true;
        ResetLockDelay();
        return true;
    }

    return false;
}

void BlockManager::ClearLine(const unsigned int yPos)
{
    const unsigned int startingIndex = GetIndexFromPos(Position(0, yPos));
    for (int i = 0; i < m_game.GetGameWidth(); i++)
    {
        DeleteBlockAtIndex(startingIndex + i);
    }

    for (int i = GetIndexFromPos(Position(m_game.GetGameWidth() - 1, yPos - 1)); i >= 0; i--)
    {
        if (IsBlockAtIndex(i))
        {
            MoveBlockAtIndex(i, i + m_game.GetGameWidth());
        }
    }
}

void BlockManager::ClearClearableLines()
{
    std::vector<unsigned int> lines = GetClearableLines();
    std::ranges::sort(lines);

    for (const unsigned int line : lines)
    {
        ClearLine(line);
    }
}

void BlockManager::SortFallingBlockIndicesAscending()
{
    std::ranges::sort(m_fallingBlockIndices);
}

void BlockManager::SortFallingBlockIndicesDescending()
{
    std::ranges::sort(m_fallingBlockIndices, std::greater<int>());
}

void BlockManager::Init()
{
    CreateNextShapeInQueue();
}

void BlockManager::Update(const float deltaTime)
{
    m_timeSinceShapeFell += deltaTime;

    if (m_timeSinceShapeFell >= m_shapeFallingRate)
    {
        MoveShapeDown();
        m_timeSinceShapeFell = 0;
    }

    if (!CanFallingShapeMoveDown())
    {
        m_lockDelayTracker += deltaTime;
    }
}

void BlockManager::Reset()
{
    std::ranges::fill(m_blocks, nullptr);
    m_fallingBlockIndices.clear();
    m_shapesBag.clear();
    m_shapesQueue.clear();
    m_shapesQueue.reserve(ClassicShapes::Num());
    m_timeSinceShapeFell = 0.0f;

    CreateNextShapeInQueue();
}

void BlockManager::MoveShapeLeft()
{
    SortFallingBlockIndicesAscending();

    for (const unsigned int fallingBlockIndex : m_fallingBlockIndices)
    {
        if (IsIndexAtLeftBorder(fallingBlockIndex)
            || (IsBlockLeftOfIndex(fallingBlockIndex) && !IsFallingBlockLeftOfIndex(fallingBlockIndex)))
        {
            std::cout << "BlockManager::MoveShapeLeft::Can't move left, obstructed." << std::endl;
            return;
        }
    }

    for (const unsigned int fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndex(fallingBlockIndex, fallingBlockIndex - 1);
    }

    m_fallingShapePosition += Position(-1, 0);
    m_lockDelayTracker = 0.0f;
    m_lockActionsCounter++;
}

void BlockManager::MoveShapeRight()
{
    SortFallingBlockIndicesDescending();

    for (const unsigned int fallingBlockIndex : m_fallingBlockIndices)
    {
        if (IsIndexAtRightBorder(fallingBlockIndex)
            || (IsBlockRightOfIndex(fallingBlockIndex) && !IsFallingBlockRightOfIndex(fallingBlockIndex)))
        {
            std::cout << "BlockManager::MoveShapeLeft::Can't move Right, obstructed." << std::endl;
            return;
        }
    }

    for (const unsigned int fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndex(fallingBlockIndex, fallingBlockIndex + 1);
    }

    m_fallingShapePosition += Position(1, 0);
    m_lockDelayTracker = 0.0f;
    m_lockActionsCounter++;
}

bool BlockManager::MoveShapeDown()
{
    SortFallingBlockIndicesDescending();

    if (!CanFallingShapeMoveDown())
    {
        if (HasLockDelayBeenPassed() || HasLockActionsBeenPassed())
        {
            CreateNextShapeInQueue();
            ClearClearableLines();
            ResetLockDelay();
            return false;
        }

        std::cout << "BlockManager::MoveShapeDown::Can't move down, obstructed." << std::endl;
        return false;
    }

    for (const unsigned int fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndexToPos(fallingBlockIndex, GetPosFromIndex(fallingBlockIndex) + Position(0, 1));
    }

    m_timeSinceShapeFell = 0.0f;
    ResetLockDelay();

    m_fallingShapePosition += Position(0, 1);
    return true;
}

void BlockManager::DropShape()
{
    SortFallingBlockIndicesDescending();

    while (MoveShapeDown()) {}

    CreateNextShapeInQueue();
}

void BlockManager::RotateShapeClockwise()
{
    if (!m_fallingShape) return;

    RotationState newRotationState = RotationState::Default;

    switch (m_fallingShapeRotationState)
    {
        case RotationState::Default:
            newRotationState = RotationState::Clockwise;
            break;
        case RotationState::Clockwise:
            newRotationState = RotationState::Double;
            break;
        case RotationState::Double:
            newRotationState = RotationState::CounterClockwise;
            break;
        case RotationState::CounterClockwise:
            newRotationState = RotationState::Default;
            break;
    }

    const Shape* fallingShape = m_fallingShape;
    const Position fallingShapePosition = m_fallingShapePosition;
    const RotationState fallingShapeRotationState = m_fallingShapeRotationState;

    DeleteShape();

    if (CreateShapeAtPos(fallingShapePosition, fallingShape, newRotationState))
    {
        m_fallingShapeRotationState = newRotationState;
        m_lockDelayTracker = 0.0f;
        m_lockActionsCounter++;
        return;
    }

    const std::array<Position, 5> kickOffsetA = fallingShape->GetKickOffset(fallingShapeRotationState);
    const std::array<Position, 5> kickOffsetB = fallingShape->GetKickOffset(newRotationState);

    for (int i = 0; i < 5; i++)
    {
        const Position kickedPosition = fallingShapePosition + (kickOffsetA[i] - kickOffsetB[i]);
        if (CreateShapeAtPos(kickedPosition, fallingShape, newRotationState))
        {
            m_fallingShapePosition = kickedPosition;
            m_fallingShapeRotationState = newRotationState;
            m_lockDelayTracker = 0.0f;
            m_lockActionsCounter++;

            return;
        }
    }

    CreateShapeAtPos(fallingShapePosition, fallingShape, fallingShapeRotationState);
}

void BlockManager::RotateShapeCounterClockwise()
{
    RotationState newRotationState = RotationState::Default;

    switch (m_fallingShapeRotationState)
    {
        case RotationState::Default:
            newRotationState = RotationState::CounterClockwise;
            break;
        case RotationState::CounterClockwise:
            newRotationState = RotationState::Double;
            break;
        case RotationState::Double:
            newRotationState = RotationState::Clockwise;
            break;
        case RotationState::Clockwise:
            newRotationState = RotationState::Default;
            break;
    }

    const Shape* fallingShape = m_fallingShape;
    const Position fallingShapePosition = m_fallingShapePosition;
    const RotationState fallingShapeRotationState = m_fallingShapeRotationState;

    DeleteShape();

    if (CreateShapeAtPos(fallingShapePosition, fallingShape, newRotationState))
    {
        m_fallingShapeRotationState = newRotationState;
        m_lockDelayTracker = 0.0f;
        m_lockActionsCounter++;
        return;
    }

    const std::array<Position, 5> kickOffsetA = fallingShape->GetKickOffset(fallingShapeRotationState);
    const std::array<Position, 5> kickOffsetB = fallingShape->GetKickOffset(newRotationState);

    for (int i = 0; i < 5; i++)
    {
        const Position kickedPosition = fallingShapePosition + (kickOffsetA[i] - kickOffsetB[i]);
        if (CreateShapeAtPos(kickedPosition, fallingShape, newRotationState))
        {
            m_fallingShapePosition = kickedPosition;
            m_fallingShapeRotationState = newRotationState;
            m_lockDelayTracker = 0.0f;
            m_lockActionsCounter++;

            return;
        }
    }

    CreateShapeAtPos(fallingShapePosition, fallingShape, fallingShapeRotationState);
}

void BlockManager::HoldShape()
{
    if (!m_canHold) return;

    if (!m_heldShape)
    {
        m_heldShape = m_fallingShape;
        DeleteShape();
        CreateNextShapeInQueue();
    }
    else
    {
        const Shape* fallingShape = m_fallingShape;
        DeleteShape();
        CreateShapeAtTopCenter(m_heldShape);
        m_heldShape = fallingShape;
    }

    m_timeSinceShapeFell = 0.0f;
    m_canHold = false;
}

void BlockManager::ResetLockDelay()
{
    m_lockDelayTracker = 0.0f;
    m_lockActionsCounter = 0;
}

std::vector<std::pair<Position, const Shape*>> BlockManager::GetAllBlocks() const
{
    std::vector<std::pair<Position, const Shape*>> output;

    for (int i = 0; i < m_blocks.size(); i++)
    {
        if (m_blocks[i] == nullptr) continue;

        output.emplace_back(std::pair(GetPosFromIndex(i), m_blocks[i]));
    }

    return output;
}

std::vector<std::pair<Position, const Shape*>> BlockManager::GetFallingShapeBlocks() const
{
    std::vector<std::pair<Position, const Shape*>> output;
    output.reserve(m_fallingBlockIndices.size());

    for (int i = 0; i < m_fallingBlockIndices.size(); i++)
    {
        const unsigned int fallingBlockIndex = m_fallingBlockIndices[i];

        if (!IsBlockAtIndex(fallingBlockIndex))
        {
            std::cerr << "BlockManager::GetFallingShapeBlocks::There is not block at index " << fallingBlockIndex <<
                std::endl;
            continue;
        }

        output.emplace_back(GetPosFromIndex(fallingBlockIndex), m_blocks[fallingBlockIndex]);
    }

    return output;
}

const Shape* BlockManager::GetFallingShape() const
{
    return m_fallingShape;
}

Position BlockManager::GetFallingShapePosition() const
{
    return m_fallingShapePosition;
}

RotationState BlockManager::GetFallingShapeRotationState() const
{
    return m_fallingShapeRotationState;
}

bool BlockManager::IsBlockAtPos(const Position inPos) const
{
    return IsBlockAtIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsBlockAtIndex(const unsigned int index) const
{
    try
    {
        return m_blocks.at(index) != nullptr;
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "BlockManager::IsBlockAtIndex::Index is out of range." << std::endl;
        return false;
    }
}

bool BlockManager::IsFallingBlockAtPos(const Position inPos) const
{
    return IsFallingBlockAtIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsFallingBlockAtIndex(const unsigned int index) const
{
    return std::ranges::contains(m_fallingBlockIndices, index);
}

bool BlockManager::IsBlockBelowPos(const Position inPos) const
{
    return IsBlockBelowIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsBlockBelowIndex(const unsigned int index) const
{
    if (IsBlockAtBottomBorder(GetYPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index + m_game.GetGameWidth()))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockBelowPos(const Position inPos) const
{
    return IsFallingBlockBelowIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsFallingBlockBelowIndex(const unsigned int index) const
{
    if (IsBlockAtBottomBorder(GetYPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index + m_game.GetGameWidth());
}

bool BlockManager::IsBlockLeftOfPos(const Position inPos) const
{
    return IsBlockLeftOfIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsBlockLeftOfIndex(const unsigned int index) const
{
    if (IsBlockAtLeftBorder(GetXPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index - 1))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockLeftOfPos(const Position inPos) const
{
    return IsFallingBlockLeftOfIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsFallingBlockLeftOfIndex(const unsigned int index) const
{
    if (IsBlockAtLeftBorder(GetXPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index - 1);
}

bool BlockManager::IsBlockRightOfPos(const Position inPos) const
{
    return IsBlockRightOfIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsBlockRightOfIndex(const unsigned int index) const
{
    if (IsBlockAtRightBorder(GetXPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index + 1))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockRightOfPos(const Position inPos) const
{
    return IsFallingBlockRightOfIndex(GetIndexFromPos(inPos));
}

bool BlockManager::IsFallingBlockRightOfIndex(const unsigned int index) const
{
    if (IsBlockAtRightBorder(GetXPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index + 1);
}

bool BlockManager::IsBlockAtBottomBorder(const unsigned int yPos) const
{
    return yPos >= m_game.GetGameHeight() - 1;
}

bool BlockManager::IsIndexAtBottomBorder(const unsigned int index) const
{
    return IsBlockAtBottomBorder(GetYPosFromIndex(index));
}

bool BlockManager::IsBlockAtLeftBorder(const unsigned int xPos) const
{
    return xPos == 0;
}

bool BlockManager::IsIndexAtLeftBorder(const unsigned int index) const
{
    return IsBlockAtLeftBorder(GetXPosFromIndex(index));
}

bool BlockManager::IsBlockAtRightBorder(const unsigned int xPos) const
{
    return xPos >= m_game.GetGameWidth() - 1;
}

bool BlockManager::IsIndexAtRightBorder(const unsigned int index) const
{
    return IsBlockAtRightBorder(GetXPosFromIndex(index));
}

bool BlockManager::CanFallingShapeMoveDown() const
{
    for (int i = 0; i < m_fallingBlockIndices.size(); i++)
    {
        if (IsBlockAtBottomBorder(GetYPosFromIndex(m_fallingBlockIndices[i])) || (IsBlockBelowIndex(m_fallingBlockIndices[i]) && !IsFallingBlockBelowIndex(m_fallingBlockIndices[i])))
            return false;
    }

    return true;
}

bool BlockManager::HasLockDelayBeenPassed() const
{
    return m_lockDelayTracker >= m_lockDelay;
}

bool BlockManager::HasLockActionsBeenPassed() const
{
    return m_lockActionsCounter >= m_numOfActionsBeforeLock;
}

bool BlockManager::IsPositionInBounds(const Position inPos) const
{
    if (inPos.x > m_game.GetGameWidth() - 1) return false;
    if (inPos.y > m_game.GetGameHeight() - 1) return false;

    return true;
}

bool BlockManager::CanLineBeCleared(const unsigned int yPos) const
{
    if (yPos > m_game.GetGameHeight()) return false;

    const unsigned int startingIndex = GetIndexFromPos(Position(0, yPos));
    for (int i = 0; i < m_game.GetGameWidth(); i++)
    {
        if (!IsBlockAtIndex(startingIndex + i)) return false;
    }

    return true;
}

bool BlockManager::CanShapeBeCreatedAtPos(const Position targetPos, const Shape* shape, const RotationState rotationState) const
{
    const std::vector<bool> stateShape = shape->GetRotationSateData(rotationState);
    const unsigned int shapeWidth = shape->GetShapeDimensions();

    for (int i = 0; i < stateShape.size(); i++)
    {
        if (stateShape[i] == false) continue;

        Position targetBlockPos;

        targetBlockPos.x = targetPos.x + i % shapeWidth;
        targetBlockPos.y = targetPos.y + std::floor(i / shapeWidth);

        if (!IsPositionInBounds(targetBlockPos) || (IsBlockAtPos(targetBlockPos) && !IsFallingBlockAtPos(targetBlockPos)))
        {
            return false;
        }
    }

    return true;
}

Position BlockManager::GetShapeTargetPos() const
{
    if (!m_fallingShape) return Position();

    Position outPosition = m_fallingShapePosition + Position(0, 1);
    while (CanShapeBeCreatedAtPos(outPosition, m_fallingShape, m_fallingShapeRotationState))
    {
        outPosition += Position(0, 1);
    }

    return outPosition;
}

std::vector<unsigned int> BlockManager::GetClearableLines() const
{
    std::vector<unsigned int> outLines;

    for (int i = 0; i < m_game.GetGameHeight(); i++)
    {
        if (CanLineBeCleared(i))
        {
            outLines.push_back(i);
        }
    }

    std::ranges::sort(outLines);

    return outLines;
}
