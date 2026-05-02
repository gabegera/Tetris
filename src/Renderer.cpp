#include "Renderer.h"

#include <iostream>

#include "Game.h"

Renderer::Renderer(Game& game) : m_game(game)
{

}

Renderer::~Renderer()
{

}

void Renderer::SetBlockTexture(const std::string& texturePath)
{
    SDL_Surface* blockSurface = SDL_LoadSurface("../res/Tetris_Block.png");
    m_blockTexture = SDL_CreateTextureFromSurface(m_renderer, blockSurface);
    SDL_DestroySurface(blockSurface);
}

void Renderer::DrawInConsole() const
{
    // Top Border
    for (int x = 0; x < m_game.GetBlockManager()->GetGameWidth() + 2; x++)
    {
        std::cout << "#";
    }
    std::cout << std::endl;

    // Game Area
    for (int y = 0; y < m_game.GetBlockManager()->GetGameHeight(); y++)
    {
        std::cout << "#";
        for (int x = 0; x < m_game.GetBlockManager()->GetGameWidth(); x++)
        {
            m_game.GetBlockManager()->IsBlockAtPosition(x, y) ? std::cout << "x" : std::cout << " ";
        }
        std::cout << "#" << std::endl;
    }

    // Bottom Border
    for (int x = 0; x < m_game.GetBlockManager()->GetGameWidth() + 2; x++)
    {
        std::cout << "#";
    }
    std::cout << std::endl;
}
void Renderer::DrawBorders() const
{
    SDL_SetTextureColorMod(m_blockTexture, 150, 150, 150);

    // Top and Bottom Border
    for (int x = 0; x < m_game.GetBlockManager()->GetGameWidth() + 2; x++)
    {
        DrawBlockAtPos(x * m_blockWidth, 0);
        DrawBlockAtPos(x * m_blockWidth, GetRenderHeight() - m_blockWidth);
    }

    // Left and Right Borders
    for (int y = 0; y < m_game.GetBlockManager()->GetGameHeight(); y++)
    {
        DrawBlockAtPos(0, (y * m_blockWidth) + m_blockWidth);
        DrawBlockAtPos(GetRenderWidth() - m_blockWidth, (y * m_blockWidth) + m_blockWidth);
    }
}
void Renderer::DrawBlocks() const
{
    const std::vector<std::tuple<Block*, Uint8, Uint8>> blocks = m_game.GetBlockManager()->GetAllBlocks();
    for (const auto& block : blocks)
    {
        const Block* blockPtr = std::get<0>(block);
        const unsigned int xPos = std::get<1>(block) * m_blockWidth + m_blockWidth;
        const unsigned int yPos = std::get<2>(block) * m_blockWidth + m_blockWidth;
        DrawBlockAtPos(xPos, yPos, blockPtr->red, blockPtr->green, blockPtr->blue);
    }
}

void Renderer::DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos) const
{
    SDL_FRect block;
    block.w = m_blockWidth;
    block.h = m_blockWidth;
    block.x = xPos;
    block.y = yPos;

    SDL_RenderTexture(m_renderer, m_blockTexture, nullptr, &block);
}

void Renderer::DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, const Uint8 red, const Uint8 green, const Uint8 blue) const
{
    SDL_SetTextureColorMod(m_blockTexture, red, green, blue);

    DrawBlockAtPos(xPos, yPos);

    SDL_SetTextureColorMod(m_blockTexture, 255, 255, 255);
}

void Renderer::Init()
{
    m_window = SDL_CreateWindow("Tetris", 500, 1000, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Window." << std::endl;
        m_game.Stop();
    }

    m_renderer = SDL_CreateRenderer(m_window, NULL);
    if (!m_renderer)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Renderer." << std::endl;
        m_game.Stop();
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    SetBlockTexture("../res/Tetris_Block.png");
    SDL_SetTextureScaleMode(m_blockTexture, SDL_SCALEMODE_PIXELART);

    SDL_SetRenderLogicalPresentation(m_renderer, GetRenderWidth(), GetRenderHeight(), SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

void Renderer::Stop() const
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void Renderer::Update(const float deltaTime)
{
    if (m_shouldDrawInConsole)
    {
        DrawInConsole();
    }

    SDL_RenderClear(m_renderer);

    DrawBorders();
    DrawBlocks();

    SDL_RenderPresent(m_renderer);
}

SDL_Window* Renderer::GetWindow() const
{
    return m_window;
}

SDL_Renderer* Renderer::GetRenderer() const
{
    return m_renderer;
}
unsigned int Renderer::GetRenderWidth() const
{
    return (m_game.GetBlockManager()->GetGameWidth() + 2) * m_blockWidth;
}

unsigned int Renderer::GetRenderHeight() const
{
    return (m_game.GetBlockManager()->GetGameHeight() + 2) * m_blockWidth;
}
