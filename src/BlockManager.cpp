#include "BlockManager.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Game.h"
#include "Shapes.h"

BlockManager::BlockManager(Game& game) : m_game(game)
{

}

BlockManager::~BlockManager()
{

}

unsigned int BlockManager::GetBlockIndexFromPos(const Uint8 xPos, const Uint8 yPos) const
{
    return xPos + (yPos * m_gameWidth);
}

std::pair<unsigned int, unsigned int> BlockManager::GetBlockPosFromIndex(const unsigned int index) const
{
    std::pair<unsigned int, unsigned int> output;

    output.first = index % m_gameWidth;
    output.second = std::floor(index / m_gameWidth);

    return output;
}

void BlockManager::CreateShape(const Shape& inShape)
{
    if (inShape.width < 1)
    {
        std::cerr << "BlockManager::CreateShape::Input Shape has an invalid width, must be greater than 0." << std::endl;
        return;
    }

    if (inShape.blocks.size() < 1)
    {
        std::cerr << "BlockManager::CreateShape::The shape doesn't have any blocks." << std::endl;
        return;
    }

    StopFallingBlocks();

    const unsigned int spawnXPos = std::ceil(m_gameWidth / 2.0f) - inShape.width + 1; // Sets the X position of spawn as close to center as possible.
    for (int i = 0; i < inShape.blocks.size(); i++)
    {
        if (inShape.blocks[i] != ' ')
        {
            m_fallingBlocks.push_back(CreateBlock(spawnXPos + (i % inShape.width), std::floor((i * 1.0f) / inShape.width), m_colorPalette.blockColors[m_colorPalletCounter]));
        }
    }

    m_colorPalletCounter++;
    if (m_colorPalletCounter >= m_colorPalette.blockColors.size())
    {
        m_colorPalletCounter = 0;
    }
}

void BlockManager::CreateNextShapeInQueue()
{
    CreateShape(m_shapesQueue[0]);
    std::shift_left(m_shapesQueue.begin(), m_shapesQueue.end(), 1);
    m_shapesQueue[ClassicShapes::numOfShapes - 1] = ClassicShapes::GetRandomShape();
}

unsigned int BlockManager::CreateBlock(const Uint8 xPos, const Uint8 yPos, const Color& color)
{
    const unsigned int blockIndex = GetBlockIndexFromPos(xPos, yPos);
    m_blocks[blockIndex] = std::make_unique<Block>(color);
    return blockIndex;
}

bool BlockManager::MoveBlock(const unsigned int targetBlockIndex, const Uint8 newXPos, const Uint8 newYPos)
{
    if (newXPos >= m_gameWidth || newYPos >= m_gameHeight)
    {
        std::cerr << "BlockManager::MoveBlock::The new position is outside of the game bounds." << std::endl;
        return false;
    }

    if (IsBlockAtPosition(newXPos, newYPos))
    {
        std::cerr << "BlockManager::MoveBlock::There is a block already in the new position." << std::endl;
        return false;
    }

    m_blocks[GetBlockIndexFromPos(newXPos, newYPos)] = std::move(m_blocks[targetBlockIndex]);
    return true;
}

void BlockManager::ClearLine(const unsigned int rowYPos)
{
    if (rowYPos >= m_gameHeight)
    {
        std::cerr << "BlockManager::ClearLine::Line Y Position is greater than game height." << std::endl;
    }

    for (int i = 0; i < m_gameWidth; i++)
    {
        m_blocks[GetBlockIndexFromPos(i, rowYPos)] = nullptr;
    }

    for (int i = m_blocks.size() - 1; i >= 0; i--)
    {
        const auto [xPos, yPos] = GetBlockPosFromIndex(i);
        MoveBlock(i, xPos, yPos + 1);
    }
}

void BlockManager::StopFallingBlocks()
{
    m_fallingBlocks.clear();
    m_fallingBlocks.resize(0);
}

void BlockManager::Init()
{
    m_blocks.resize(m_gameWidth * m_gameHeight);

    m_shapesQueue.resize(ClassicShapes::numOfShapes);
    for (int i = 0; i < ClassicShapes::numOfShapes; i++)
    {
        m_shapesQueue[i] = ClassicShapes::GetRandomShape();
    }

    CreateNextShapeInQueue();
}

void BlockManager::Update(const float deltaTime)
{
    m_blockFallingRateTracker += deltaTime;

    if (m_blockFallingRateTracker > m_blockFallingRate)
    {
        MoveFallingBlocksDown();
    }
}

bool BlockManager::MoveFallingBlocksHorizontal(int direction)
{
    if (direction == 0)
    {
        std::cerr << "BlockManager::MoveFallingBlocksHorizontal::Invalid Direction" << std::endl;
        return false;
    }

    // Clamp direction to either -1 or 1.
    direction < 0 ? direction = -1 : direction = 1;

    if (!CanFallingBlocksMoveHorizontal(direction)) return false;

    if (direction == 1) // starts from the end of the vector if moving right.
    {
        for (int i = m_fallingBlocks.size() - 1; i >= 0; i--)
        {
            const unsigned int targetBlockIndex = m_fallingBlocks[i];
            auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);
            MoveBlock(targetBlockIndex, xPos + direction, yPos);
            m_fallingBlocks[i] = targetBlockIndex + direction;
        }
    }
    else // starts from the beginning of the vector if moving left.
    {
        for (int i = 0; i < m_fallingBlocks.size(); i++)
        {
            const unsigned int targetBlockIndex = m_fallingBlocks[i];
            auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);
            MoveBlock(targetBlockIndex, xPos + direction, yPos);
            m_fallingBlocks[i] = targetBlockIndex + direction;
        }
    }

    return true;
}

bool BlockManager::MoveFallingBlocksDown()
{
    if (!CanFallingBlocksMoveDown())
    {
        for (int i = 0; i < m_fallingBlocks.size(); i++)
        {
            const Uint8 lineYPos = GetBlockPosFromIndex(m_fallingBlocks[i]).second;
            if (CanLineBeCleared(lineYPos))
            {
                ClearLine(lineYPos);
            }
        }
        CreateNextShapeInQueue();
        return false;
    }

    for (int i = m_fallingBlocks.size() - 1; i >= 0; i--)
    {
        const unsigned int targetBlockIndex = m_fallingBlocks[i];
        const auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);
        MoveBlock(targetBlockIndex, xPos, yPos + 1);
        m_fallingBlocks[i] = targetBlockIndex + m_gameWidth;
    }
    m_blockFallingRateTracker = 0.0f;

    return true;
}

void BlockManager::DropFallingBlocks()
{
    while (MoveFallingBlocksDown())
    {

    }
}

Uint8 BlockManager::GetGameWidth() const
{
    return m_gameWidth;
}

Uint8 BlockManager::GetGameHeight() const
{
    return m_gameHeight;
}

const ColorPalette& BlockManager::GetColorPalette() const
{
    return m_colorPalette;
}

bool BlockManager::IsBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const
{
    return m_blocks[GetBlockIndexFromPos(xPos, yPos)] != nullptr;
}

Block* BlockManager::GetBlockAtPosition(const Uint8 xPos, const Uint8 yPos) const
{
    return m_blocks[GetBlockIndexFromPos(xPos, yPos)].get();
}

std::vector<std::tuple<Block*, Uint8, Uint8>> BlockManager::GetAllBlocks() const
{
    std::vector<std::tuple<Block*, Uint8, Uint8>> output;

    for (int i = 0; i < m_blocks.size(); i++)
    {
        if (m_blocks[i] != nullptr)
        {
            auto [xPos, yPos] = GetBlockPosFromIndex(i);
            output.emplace_back(std::tuple(m_blocks[i].get(), xPos, yPos));
        }
    }

    return output;
}

bool BlockManager::CanFallingBlocksMoveHorizontal(int direction) const
{
    // Clamp direction to either -1 or 1.
    direction < 0 ? direction = -1 : direction = 1;

    // Check that moving the blocks won't cause collisions.
    for (int i = 0; i < m_fallingBlocks.size(); i++)
    {
        const unsigned int targetBlockIndex = m_fallingBlocks[i];
        const auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);

        // Check for left boundary.
        if (direction == -1 && xPos == 0)
        {
            std::cout << "Cannot move left due to boundary." << std::endl;
            return false;
        }
        // Check for right boundary.
        if (direction == 1 && xPos == m_gameWidth - 1)
        {
            std::cout << "Cannot move right due to boundary." << std::endl;
            return false;
        }

        // Check for blocks in the way.
        if (m_blocks[targetBlockIndex + direction] != nullptr &&
            !std::ranges::contains(m_fallingBlocks, targetBlockIndex + direction)) // ignore other falling blocks.
        {
            const std::string leftOrRight = direction < 0 ? "left" : "right";
            std::cout << "Cannot move " << leftOrRight << " due to another block being in the way." << std::endl;
            return false;
        }
    }

    return true;
}

bool BlockManager::CanFallingBlocksMoveDown() const
{
    for (int i = 0; i < m_fallingBlocks.size(); i++)
    {
        const unsigned int targetBlockIndex = m_fallingBlocks[i];
        const auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);

        // Check for bottom boundary.
        if (yPos == m_gameHeight - 1)
        {
            std::cout << "Cannot move down due to boundary." << std::endl;
            return false;
        }

        // Check for blocks below shape
        if (IsBlockAtPosition(xPos, yPos + 1) &&
            !std::ranges::contains(m_fallingBlocks, GetBlockIndexFromPos(xPos, yPos + 1))) // ignore other falling blocks.
        {
            std::cout << "Cannot move down due to block." << std::endl;
            return false;
        }
    }

    return true;
}

bool BlockManager::CanLineBeCleared(const Uint8 lineYPos) const
{
    for (int i = 0; i < m_gameWidth; i++)
    {
        if (m_blocks[GetBlockIndexFromPos(i, lineYPos)] == nullptr)
        {
            return false;
        }
    }

    return true;
}
