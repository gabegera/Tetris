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
    m_shapesQueue.reserve(ClassicShapes::numOfShapes);
}

BlockManager::~BlockManager()
{

}

Uint16 BlockManager::GetBlockIndexFromPos(const Uint8 xPos, const Uint8 yPos) const
{
    return xPos + (yPos * m_game.GetGameWidth());
}

Uint8 BlockManager::GetBlockXPosFromIndex(const Uint16 index) const
{
    return index % m_game.GetGameWidth();
}

Uint8 BlockManager::GetBlockYPosFromIndex(const Uint16 index) const
{
    return std::floor(index / m_game.GetGameWidth());
}

void BlockManager::GetBlockPosFromIndex(const Uint16 index, Uint8& outXPos, Uint8& outYPos) const
{
    outXPos = GetBlockXPosFromIndex(index);
    outYPos = GetBlockYPosFromIndex(index);
}

void BlockManager::FillShapesBag()
{
    m_shapesBag =
    {
        ClassicShapes::I,
        ClassicShapes::O,
        ClassicShapes::L,
        ClassicShapes::J,
        ClassicShapes::Z,
        ClassicShapes::S,
        ClassicShapes::T,
    };
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::ranges::shuffle(m_shapesBag, gen);
}

void BlockManager::FillShapesQueue()
{
    const Uint16 emptySpacesInQueue = m_shapesQueue.capacity() - m_shapesQueue.size();
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

Uint16 BlockManager::CreateBlockAtPos(const Uint8 xPos, const Uint8 yPos, const Uint16 colorID)
{
    return CreateBlockAtIndex(GetBlockIndexFromPos(xPos, yPos), colorID);
}

Uint16 BlockManager::CreateBlockAtIndex(const Uint16 index, const Uint16 colorID)
{
    m_blocks[index] = std::make_unique<Block>(colorID);
    return index;
}

void BlockManager::DeleteBlockAtPos(const Uint8 xPos, const Uint8 yPos)
{
    DeleteBlockAtIndex(GetBlockIndexFromPos(xPos, yPos));
}

void BlockManager::DeleteBlockAtIndex(const Uint16 index)
{
    if (index < 0 || index > m_blocks.size())
    {
        std::cerr << "BlockManager::DeleteBlockAtIndex::Invalid target index." << std::endl;
    }

    // std::ranges::remove(m_fallingBlockIndices, index);

    m_blocks[index] = nullptr;
}

void BlockManager::DeleteShape()
{
    if (m_fallingBlockIndices.empty())
    {
        std::cerr << "BlockManager::DeleteShape::No shape to destroy." << std::endl;
        return;
    }

    for (const Uint16 index : m_fallingBlockIndices)
    {
        DeleteBlockAtIndex(index);
    }

    m_fallingBlockIndices.clear();
}

bool BlockManager::MoveBlockAtPos(const Uint8 xPos, const Uint8 yPos, const Uint8 newXPos, const Uint8 newYPos)
{
    return MoveBlockAtIndex(GetBlockIndexFromPos(xPos, yPos), GetBlockIndexFromPos(newXPos, newYPos));
}

bool BlockManager::MoveBlockAtIndexToPos(const Uint16 index, const Uint8 newXPos, const Uint8 newYPos)
{
    return MoveBlockAtIndex(index, GetBlockIndexFromPos(newXPos, newYPos));
}

bool BlockManager::MoveBlockAtPosToIndex(const Uint8 xPos, const Uint8 yPos, const Uint16 newIndex)
{
    return MoveBlockAtIndex(GetBlockIndexFromPos(xPos, yPos), newIndex);
}

bool BlockManager::MoveBlockAtIndex(const Uint16 index, const Uint16 newIndex)
{
    if (m_blocks[newIndex] != nullptr)
    {
        std::cerr << "BlockManager::MoveBlockAtIndex::Can't move block, target index is occupied." << std::endl;
        return false;
    }

    if (m_blocks[index] == nullptr)
    {
        std::cerr << "BlockManager::MoveBlockAtIndex::Target Block Index is null.";
        return false;
    }

    if (std::ranges::contains(m_fallingBlockIndices, index))
    {
        for (int i = 0; i < m_fallingBlockIndices.size(); i++)
        {
            if (m_fallingBlockIndices[i] == index)
            {
                m_fallingBlockIndices[i] = newIndex;
            }
        }
    }

    m_blocks[newIndex] = std::move(m_blocks[index]);
    return true;
}

void BlockManager::CreateShapeAtPos(const Uint8 xPos, const Uint8 yPos, const Shape& shape, const bool increaseColorID)
{
    if (shape.width < 1)
    {
        std::cerr << "BlockManager::CreateShape::Input Shape has an invalid width, must be greater than 0." << std::endl;
        return;
    }

    if (shape.blocks.size() < 1)
    {
        std::cerr << "BlockManager::CreateShape::The shape doesn't have any blocks." << std::endl;
        return;
    }

    m_fallingBlockIndices.reserve(shape.blocks.size());
    for (int i = 0; i < shape.blocks.size(); i++)
    {
        if (shape.blocks[i] != ' ')
        {
            m_fallingBlockIndices.push_back(CreateBlockAtPos(xPos + (i % shape.width), yPos + std::floor((i * 1.0f) / shape.width), m_colorIDCounter));
        }
    }
    m_fallingBlockIndices.shrink_to_fit();

    m_colorIDCounter++;
    m_fallingShape = shape;
}

void BlockManager::CreateShapeAtTopCenter(const Shape& shape)
{
    const unsigned int spawnXPos = std::ceil(m_game.GetGameWidth() / 2.0f) - shape.width + 1; // Sets the X position of spawn as close to center as possible.
    CreateShapeAtPos(spawnXPos, 0, shape);
}

void BlockManager::CreateNextShapeInQueue()
{
    if (m_shapesQueue.empty())
    {
        FillShapesQueue();
    }

    CreateShapeAtTopCenter(m_shapesQueue.front());
    m_shapesQueue.erase(m_shapesQueue.begin());
}

void BlockManager::ClearLine(const Uint8 yPos)
{
    const Uint16 startingIndex = GetBlockIndexFromPos(0, yPos);
    for (int i = 0; i < m_game.GetGameWidth(); i++)
    {
        DeleteBlockAtIndex(startingIndex + i);
    }

    for (int i = m_blocks.size() - 1; i >= 0; i--)
    {
        if (IsBlockAtIndex(i) && !IsBlockAtBottomBorder(GetBlockYPosFromIndex(i)))
        {
            MoveBlockAtIndex(i, i + m_game.GetGameWidth());
        }
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
}

void BlockManager::MoveShapeLeft()
{
    SortFallingBlockIndicesAscending();

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        if (IsIndexAtLeftBorder(fallingBlockIndex)
            || (IsBlockLeftOfIndex(fallingBlockIndex) && !IsFallingBlockLeftOfIndex(fallingBlockIndex)))
        {
            std::cerr << "BlockManager::MoveShapeLeft::Can't move left, obstructed.";
            return;
        }
    }

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndex(fallingBlockIndex, fallingBlockIndex - 1);
    }
}

void BlockManager::MoveShapeRight()
{
    SortFallingBlockIndicesDescending();

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        if (IsIndexAtRightBorder(fallingBlockIndex)
            || (IsBlockRightOfIndex(fallingBlockIndex) && !IsFallingBlockRightOfIndex(fallingBlockIndex)))
        {
            std::cout << "BlockManager::MoveShapeLeft::Can't move Right, obstructed." << std::endl;
            return;
        }
    }

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndex(fallingBlockIndex, fallingBlockIndex + 1);
    }
}

bool BlockManager::MoveShapeDown()
{
    SortFallingBlockIndicesDescending();

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        if (GetBlockYPosFromIndex(fallingBlockIndex) == m_game.GetGameHeight() - 1
            || (IsBlockBelowIndex(fallingBlockIndex))
                && !IsFallingBlockBelowIndex(fallingBlockIndex))
        {
            m_fallingBlockIndices.clear();
            CreateNextShapeInQueue();
            for (const Uint8 line : GetClearableLines())
            {
                ClearLine(line);
            }
            std::cout << "BlockManager::MoveShapeDown::Can't move down, obstructed." << std::endl;
            return false;
        }
    }

    for (const Uint16 fallingBlockIndex : m_fallingBlockIndices)
    {
        MoveBlockAtIndex(fallingBlockIndex, fallingBlockIndex + m_game.GetGameWidth());
    }

    m_timeSinceShapeFell = 0.0f;

    return true;
}

void BlockManager::DropShape()
{
    SortFallingBlockIndicesDescending();

    while (MoveShapeDown());
}

void BlockManager::RotateShape()
{
    Uint8 cornerXPos = m_game.GetGameWidth();
    Uint8 cornerYPos = m_game.GetGameHeight();

    for (const Uint16 index : m_fallingBlockIndices)
    {
        const Uint8 xPos = GetBlockXPosFromIndex(index);
        const Uint8 yPos = GetBlockYPosFromIndex(index);

        if (xPos < cornerXPos) cornerXPos = xPos;
        if (yPos < cornerYPos) cornerYPos = yPos;
    }

    Shape rotatedShape;
    rotatedShape.width = m_fallingShape.GetHeight();
    rotatedShape.blocks.reserve(m_fallingShape.blocks.size());

    std::map<int, char> rotatedShapeIndices;
    for (int i = 0; i < m_fallingShape.blocks.size(); i++)
    {
        const int shapeBlockXPos = (i % m_fallingShape.width);
        const int shapeBlockYPos = -std::floor(i / m_fallingShape.width);

        const int rotatedXPos = shapeBlockYPos + rotatedShape.width - 1;
        const int rotatedYPos = -shapeBlockXPos;
        rotatedShapeIndices.emplace(rotatedXPos + (-rotatedYPos * rotatedShape.width), m_fallingShape.blocks[i]);
    }

    for (const auto& [index, character] : rotatedShapeIndices)
    {
        rotatedShape.blocks.push_back(character);
    }

    DeleteShape();
    CreateShapeAtPos(cornerXPos, cornerYPos, rotatedShape);
}

void BlockManager::GetAllBlocks(std::vector<Uint8>& xPositions, std::vector<Uint8>& yPositions,
                                std::vector<Block*>& blockPtrs) const
{
    std::vector<Uint8> outXPositions;
    std::vector<Uint8> outYPositions;
    std::vector<Block*> outPtrs;

    for (int i = 0; i < m_blocks.size(); i++)
    {
        if (m_blocks[i] == nullptr) continue;

        Uint8 xPos;
        Uint8 yPos;
        GetBlockPosFromIndex(i, xPos, yPos);
        outXPositions.push_back(xPos);
        outYPositions.push_back(yPos);
        outPtrs.push_back(m_blocks[i].get());
    }

    outXPositions.shrink_to_fit();
    outYPositions.shrink_to_fit();
    outPtrs.shrink_to_fit();

    xPositions = outXPositions;
    yPositions = outYPositions;
    blockPtrs = outPtrs;
}

void BlockManager::GetFallingShapeBlocks(std::vector<Uint8>& xPositions, std::vector<Uint8>& yPositions,
        std::vector<Block*>& blockPtrs) const
{
    const Uint16 vectorSize = m_fallingBlockIndices.size();

    std::vector<Uint8> outXPositions(vectorSize);
    std::vector<Uint8> outYPositions(vectorSize);
    std::vector<Block*> outPtrs(vectorSize);

    for (int i = 0; i < m_fallingBlockIndices.size(); i++)
    {
        const Uint16 fallingBlockIndex = m_fallingBlockIndices[i];

        if (!IsBlockAtIndex(fallingBlockIndex))
        {
            std::cerr << "BlockManager::GetFallingShapeBlocks::There is not block at index " << fallingBlockIndex << std::endl;
            continue;
        }

        Uint8 xPos;
        Uint8 yPos;
        GetBlockPosFromIndex(fallingBlockIndex, xPos, yPos);
        outXPositions.push_back(xPos);
        outYPositions.push_back(yPos);
        outPtrs.push_back(m_blocks[fallingBlockIndex].get());
    }

    xPositions = outXPositions;
    yPositions = outYPositions;
    blockPtrs = outPtrs;
}

bool BlockManager::IsBlockAtPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsBlockAtIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsBlockAtIndex(const Uint16 index) const
{
    return m_blocks[index] != nullptr;
}

bool BlockManager::IsFallingBlockAtPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsFallingBlockAtIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsFallingBlockAtIndex(const Uint16 index) const
{
    return std::ranges::contains(m_fallingBlockIndices, index);
}

bool BlockManager::IsBlockBelowPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsBlockBelowIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsBlockBelowIndex(const Uint16 index) const
{
    if (IsBlockAtBottomBorder(GetBlockYPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index + m_game.GetGameWidth()))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockBelowPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsFallingBlockBelowIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsFallingBlockBelowIndex(const Uint16 index) const
{
    if (IsBlockAtBottomBorder(GetBlockYPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index + m_game.GetGameWidth());
}

bool BlockManager::IsBlockLeftOfPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsBlockLeftOfIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsBlockLeftOfIndex(const Uint16 index) const
{
    if (IsBlockAtLeftBorder(GetBlockXPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index - 1))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockLeftOfPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsFallingBlockLeftOfIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsFallingBlockLeftOfIndex(const Uint16 index) const
{
    if (IsBlockAtLeftBorder(GetBlockXPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index - 1);
}

bool BlockManager::IsBlockRightOfPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsBlockRightOfIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsBlockRightOfIndex(const Uint16 index) const
{
    if (IsBlockAtRightBorder(GetBlockXPosFromIndex(index)))
    {
        return true;
    }

    if (IsBlockAtIndex(index + 1))
    {
        return true;
    }

    return false;
}

bool BlockManager::IsFallingBlockRightOfPos(const Uint8 xPos, const Uint8 yPos) const
{
    return IsFallingBlockRightOfIndex(GetBlockIndexFromPos(xPos, yPos));
}

bool BlockManager::IsFallingBlockRightOfIndex(const Uint16 index) const
{
    if (IsBlockAtRightBorder(GetBlockXPosFromIndex(index)))
    {
        return false;
    }

    return std::ranges::contains(m_fallingBlockIndices, index + 1);
}

bool BlockManager::IsBlockAtBottomBorder(const Uint8 yPos) const
{
    return yPos >= m_game.GetGameHeight() - 1;
}

bool BlockManager::IsIndexAtBottomBorder(const Uint16 index) const
{
    return IsBlockAtBottomBorder(GetBlockYPosFromIndex(index));
}

bool BlockManager::IsBlockAtLeftBorder(const Uint8 xPos) const
{
    return xPos == 0;
}

bool BlockManager::IsIndexAtLeftBorder(const Uint16 index) const
{
    return IsBlockAtLeftBorder(GetBlockXPosFromIndex(index));
}

bool BlockManager::IsBlockAtRightBorder(const Uint8 xPos) const
{
    return xPos >= m_game.GetGameWidth() - 1;
}

bool BlockManager::IsIndexAtRightBorder(const Uint16 index) const
{
    return IsBlockAtRightBorder(GetBlockXPosFromIndex(index));
}

bool BlockManager::CanLineBeCleared(const Uint8 yPos) const
{
    if (yPos > m_game.GetGameHeight()) return false;

    const Uint16 startingIndex = GetBlockIndexFromPos(0, yPos);
    for (int i = 0; i < m_game.GetGameWidth(); i++)
    {
        if (!IsBlockAtIndex(startingIndex + i)) return false;
    }

    return true;
}

std::vector<Uint8> BlockManager::GetClearableLines() const
{
    std::vector<Uint8> outLines;

    for (int i = 0; i < m_game.GetGameHeight(); i++)
    {
        if (CanLineBeCleared(i))
        {
            outLines.push_back(i);
        }
    }

    return outLines;
}
