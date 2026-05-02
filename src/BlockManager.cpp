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

void BlockManager::CreateShape(const Shape& inShape, const Uint8 red, const Uint8 green, const Uint8 blue)
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

    m_fallingBlocks.clear();
    m_fallingBlocks.resize(0);

    const unsigned int spawnXPos = std::ceil(m_gameWidth / 2.0f) - inShape.width + 1; // Sets the X position of spawn as close to center as possible.
    for (int i = 0; i < inShape.blocks.size(); i++)
    {
        if (inShape.blocks[i] != ' ')
        {
            m_fallingBlocks.push_back(CreateBlock(spawnXPos + (i % inShape.width), std::floor((i * 1.0f) / inShape.width), red, green, blue));
        }
    }
}

unsigned int BlockManager::CreateBlock(const Uint8 xPos, const Uint8 yPos, const Uint8 red, const Uint8 green, const Uint8 blue)
{
    const unsigned int blockIndex = GetBlockIndexFromPos(xPos, yPos);
    m_blocks[blockIndex] = std::make_unique<Block>(red, green, blue);
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

    const unsigned int lineStart = GetBlockIndexFromPos(0, rowYPos);
    for (int i = 0; i < m_gameWidth; i++)
    {
        m_blocks[i + lineStart] = nullptr;
    }
}

void BlockManager::Init()
{
    m_blocks.resize(m_gameWidth * m_gameHeight);

    CreateShape(Shapes::T, 255, 0, 0);
}

void BlockManager::Update(const float deltaTime)
{
    m_blockFallingRateTracker += deltaTime;

    if (m_blockFallingRateTracker > m_blockFallingRate)
    {
        MoveFallingBlocksDown();
    }
}

void BlockManager::MoveFallingBlocksHorizontal(int direction)
{
    if (direction == 0)
    {
        std::cerr << "BlockManager::MoveFallingBlocksHorizontal::Invalid Direction" << std::endl;
        return;
    }

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
            return;
        }
        // Check for right boundary.
        if (direction == 1 && xPos == m_gameWidth - 1)
        {
            std::cout << "Cannot move right due to boundary." << std::endl;
            return;
        }

        // Check for blocks in the way.
        if (m_blocks[targetBlockIndex + direction] != nullptr &&
            !std::ranges::contains(m_fallingBlocks, targetBlockIndex + direction)) // ignore other falling blocks.
        {
            const std::string leftOrRight = direction < 0 ? "left" : "right";
            std::cout << "Cannot move " << leftOrRight << " due to another block being in the way." << std::endl;
            return;
        }
    }

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
}

void BlockManager::MoveFallingBlocksDown()
{
    for (int i = 0; i < m_fallingBlocks.size(); i++)
    {
        const unsigned int targetBlockIndex = m_fallingBlocks[i];
        const auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);

        // Check for bottom boundary.
        if (yPos == m_gameHeight - 1)
        {
            std::cout << "Cannot move down due to boundary." << std::endl;
            return;
        }
    }

    for (int i = m_fallingBlocks.size() - 1; i >= 0; i--)
    {
        const unsigned int targetBlockIndex = m_fallingBlocks[i];
        const auto [xPos, yPos] = GetBlockPosFromIndex(targetBlockIndex);
        MoveBlock(targetBlockIndex, xPos, yPos + 1);
        m_fallingBlocks[i] = targetBlockIndex + m_gameWidth;
    }
    m_blockFallingRateTracker = 0.0f;
}

void BlockManager::DropFallingBlocks()
{

}

Uint8 BlockManager::GetGameWidth() const
{
    return m_gameWidth;
}

Uint8 BlockManager::GetGameHeight() const
{
    return m_gameHeight;
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
