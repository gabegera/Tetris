#include "Renderer.h"

#include <iostream>

#include "Application.h"

Renderer::Renderer(Application& app) : m_application(app)
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

void Renderer::SetColorPalette(const ColorPalette& newPalette)
{
    m_colorPalette = newPalette;
}

void Renderer::DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, const Uint16 colorID) const
{
    const Uint16 index = colorID > 0 ? colorID % m_colorPalette.blockColors.size() : 0;
    DrawBlockAtPos(xPos, yPos, m_colorPalette.blockColors[index]);
}

void Renderer::DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, const Color color) const
{
    SDL_FRect block;
    block.w = m_blockResolution;
    block.h = m_blockResolution;
    block.x = xPos * m_blockResolution;
    block.y = yPos * m_blockResolution;

    SDL_SetTextureColorMod(m_blockTexture, color.red, color.green, color.blue);
    SDL_RenderTexture(m_renderer, m_blockTexture, nullptr, &block);
    SDL_SetTextureColorMod(m_blockTexture, 255, 255, 255);
}

void Renderer::Init()
{
    m_window = SDL_CreateWindow("Tetris", 500, 1000, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Window." << std::endl;
        m_application.Stop();
    }

    m_renderer = SDL_CreateRenderer(m_window, NULL);
    if (!m_renderer)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Renderer." << std::endl;
        m_application.Stop();
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    SetBlockTexture("../res/Tetris_Block.png");
    SDL_SetTextureScaleMode(m_blockTexture, SDL_SCALEMODE_PIXELART);
}

void Renderer::Stop() const
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void Renderer::Update(const float deltaTime) const
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::SetRendererSizeInPixels(const Uint16 width, const Uint16 height) const
{
    SDL_SetRenderLogicalPresentation(m_renderer, width, height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

void Renderer::SetRendererSizeInGameDimensions(const Uint16 gameWidth, const Uint16 gameHeight) const
{
    SetRendererSizeInPixels((gameWidth + 2) * m_blockResolution, (gameHeight + 2) * m_blockResolution);
}

void Renderer::ClearRenderer() const
{
    SDL_RenderClear(m_renderer);
}

SDL_Window* Renderer::GetWindow() const
{
    return m_window;
}

SDL_Renderer* Renderer::GetSDLRenderer() const
{
    return m_renderer;
}

const ColorPalette& Renderer::GetColorPalette() const
{
    return m_colorPalette;
}
