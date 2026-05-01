#include "BlockManager.h"

#include <cmath>

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
    return xPos + (yPos * m_game.GetGameWidth());
}

std::pair<unsigned int, unsigned int> BlockManager::GetBlockPosFromIndex(const unsigned int index) const
{
    std::pair<unsigned int, unsigned int> output;

    output.first = index % m_game.GetGameWidth();
    output.second = std::floor(index / m_game.GetGameWidth());

    return output;
}

void BlockManager::CreateShape(const Shape& inShape)
{
    // for (const char& block : inShape.blocks)
    // {
    //     if (block != ' ')
    //     {
    //
    //     }
    // }
}

void BlockManager::CreateBlock(const Uint8 xPos, const Uint8 yPos, const Uint8 red, const Uint8 green, const Uint8 blue)
{
    const unsigned int blockIndex = GetBlockIndexFromPos(xPos, yPos);
    if (!m_blocks[blockIndex])
    {
        m_blocks.emplace(m_blocks.begin() + blockIndex, std::make_unique<Block>(red, green, blue));
    }
}

bool BlockManager::MoveBlock(const unsigned int targetBlockIndex, const Uint8 newXPos, const Uint8 newYPos)
{
    if (newXPos >= m_game.GetGameWidth() || newYPos >= m_game.GetGameHeight())
    {
        return false;
    }

    if (IsBlockAtPosition(newXPos, newYPos))
    {
        return false;
    }

    m_blocks[GetBlockIndexFromPos(newXPos, newYPos)] = std::move(m_blocks[targetBlockIndex]);
    return true;
}

void BlockManager::ClearLine(int yPos)
{

}

void BlockManager::Init()
{
    m_blocks.resize(m_game.GetGameWidth() * m_game.GetGameHeight());

    CreateBlock(5, 5, 255, 0, 0);
    // MoveBlock(5, 5, 1);
}

void BlockManager::Stop()
{

}

void BlockManager::Update()
{

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
