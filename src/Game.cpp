#include "Game.h"

#include <iostream>

#include "Application.h"

Game::Game(Application& app, const Uint8 gameWidth, const Uint8 gameHeight)
    : m_gameWidth(gameWidth), m_gameHeight(gameHeight), m_application(app), m_blockManager(*this)
{

}

Game::~Game()
{

}

void Game::RenderBorders() const
{
    SDL_Texture* borderTexture = GetRenderer()->GetBorderTexture();
    const Color& borderColor = m_application.GetTheme()->GetBorderColor();

    // Top and Bottom Border
    for (int x = 0; x < m_gameWidth + 2; x++)
    {
        GetRenderer()->DrawBlockAtPos(x, 0, borderTexture, borderColor);
        GetRenderer()->DrawBlockAtPos(x, m_gameHeight + 1, borderTexture, borderColor);
    }

    // Left and Right Borders
    for (int y = 0; y < m_gameHeight; y++)
    {
        GetRenderer()->DrawBlockAtPos(0, y + 1, borderTexture, borderColor);
        GetRenderer()->DrawBlockAtPos(m_gameWidth + 1, y + 1, borderTexture, borderColor);
    }
}

void Game::RenderBlocks()
{
    std::vector<Uint8> xPositions;
    std::vector<Uint8> yPositions;
    std::vector<Block*> blockPtrs;
    GetBlockManager()->GetAllBlocks(xPositions, yPositions, blockPtrs);

    for (int i = 0; i < blockPtrs.size(); i++)
    {
        if (!blockPtrs[i])
        {
            std::cerr << "Game::RenderBlocks::Failed to render block. Invalid Block Pointer" << std::endl;
            continue;
        }

        GetRenderer()->DrawBlockAtPos(xPositions[i] + 1, yPositions[i] + 1, blockPtrs[i]->m_owningShape);
    }
}

void Game::RenderShapeGuide()
{
    Uint8 xPos;
    Uint8 yPos;
    m_blockManager.GetShapeTargetPos(xPos, yPos);
    GetRenderer()->DrawShapeGuideAtPos(xPos, yPos, m_blockManager.GetFallingShape(), m_blockManager.GetOriginalFallingShape());
}

void Game::Start()
{
    m_blockManager.Init();

    GetRenderer()->SetRendererSizeInGameDimensions(m_gameWidth, m_gameHeight);
}

void Game::Stop()
{

}

void Game::Update(const float deltaTime)
{
    m_blockManager.Update(deltaTime);

    RenderBorders();
    RenderShapeGuide();
    RenderBlocks();

    // m_application.GetRenderer()->DrawText("Points: ", 10, 10);
}

void Game::TriggerGameOver()
{
    m_blockManager.Reset();
}

Renderer* Game::GetRenderer() const
{
    return m_application.GetRenderer();
}

BlockManager* Game::GetBlockManager()
{
    return &m_blockManager;
}

Uint8 Game::GetGameWidth() const
{
    return m_gameWidth;
}

Uint8 Game::GetGameHeight() const
{
    return m_gameHeight;
}


