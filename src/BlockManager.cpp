#include "BlockManager.h"

#include "Game.h"
#include "Shapes.h"

BlockManager::BlockManager(const Game& game) : m_game(game)
{
    m_blocks.resize(game.GetGameWidth() * game.GetGameHeight());

    CreateBlock(5, 0, 255, 0, 0);
}

BlockManager::~BlockManager()
{

}

unsigned int BlockManager::GetBlockIndexFromPos(const unsigned int xPos, const unsigned int yPos) const
{
    return xPos + (yPos * m_game.GetGameWidth());
}

std::pair<unsigned int, unsigned int> BlockManager::GetBlockPosFromIndex(const unsigned int index) const
{
    std::pair<unsigned int, unsigned int> output;

    output.first = index % m_game.GetGameWidth();
    if (index < m_game.GetGameWidth())
    {
        output.second = 0;
    }
    else
    {
        output.second = -(output.first / m_game.GetGameWidth()) + index;
    }

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

void BlockManager::CreateBlock(const uint8_t xPos, const uint8_t yPos, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    const uint8_t blockIndex = GetBlockIndexFromPos(xPos, yPos);
    if (m_blocks[blockIndex] == nullptr)
    {
        m_blocks.emplace(m_blocks.begin() + blockIndex, std::make_unique<Block>(red, green, blue));
    }
}

void BlockManager::ClearLine(int yPos)
{

}

void BlockManager::Update()
{

}

bool BlockManager::IsBlockAtPosition(const uint8_t xPos, const uint8_t yPos) const
{
    return m_blocks[GetBlockIndexFromPos(xPos, yPos)] != nullptr;
}

Block* BlockManager::GetBlockAtPosition(const uint8_t xPos, const uint8_t yPos) const
{
    return m_blocks[GetBlockIndexFromPos(xPos, yPos)].get();
}

std::vector<std::tuple<Block*, unsigned int, unsigned int>> BlockManager::GetAllBlocks() const
{
    std::vector<std::tuple<Block*, unsigned int, unsigned int>> output;

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
