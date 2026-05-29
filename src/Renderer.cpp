#include "Renderer.h"

#include <iostream>

#include "Application.h"
#include "UI/Button.h"

Renderer::Renderer(Application& app) : m_application(app)
{

}

Renderer::~Renderer()
{

}

void Renderer::SetBlockTexture(const std::filesystem::path& texturePath)
{
    SDL_Surface* blockSurface = SDL_LoadSurface(texturePath.generic_string().c_str());
    m_blockTexture = SDL_CreateTextureFromSurface(m_renderer, blockSurface);
    SDL_DestroySurface(blockSurface);
}

void Renderer::SetTransparentBlockTexture(const std::filesystem::path& texturePath)
{
    SDL_Surface* blockSurface = SDL_LoadSurface(texturePath.generic_string().c_str());
    m_transparentBlockTexture = SDL_CreateTextureFromSurface(m_renderer, blockSurface);
    SDL_DestroySurface(blockSurface);
}

void Renderer::SetFont(const std::filesystem::path& fontPath)
{
    m_font = TTF_OpenFont(fontPath.generic_string().c_str(), 16);
}

void Renderer::SetColorPalette(const ColorPalette& newPalette)
{
    m_colorPalette = newPalette;
}

void Renderer::ResetDrawColor() const
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
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

void Renderer::DrawShapeGuideAtPos(const Uint8 xPos, const Uint8 yPos, const Shape& shape, const Uint16 colorID) const
{
    const Uint16 index = colorID > 0 ? colorID % m_colorPalette.blockColors.size() : 0;
    DrawShapeGuideAtPos(xPos, yPos, shape, m_colorPalette.blockColors[index]);
}

void Renderer::DrawShapeGuideAtPos(const Uint8 xPos, const Uint8 yPos, const Shape& shape, const Color color) const
{
    SDL_FRect block;
    block.w = m_blockResolution;
    block.h = m_blockResolution;

    for (int i = 0; i < shape.blocks.size(); i++)
    {
        if (shape.blocks[i] != ' ')
        {
            block.x = (xPos + (i % shape.width)) * m_blockResolution;
            block.x += m_blockResolution; // offset the border.
            block.y = (yPos + std::floor((i * 1.0f) / shape.width)) * m_blockResolution;
            block.y += m_blockResolution;
            SDL_SetTextureColorMod(m_transparentBlockTexture, color.red, color.green, color.blue);
            SDL_SetTextureAlphaMod(m_transparentBlockTexture, 150);
            SDL_RenderTexture(m_renderer, m_transparentBlockTexture, nullptr, &block);
        }
    }

    SDL_SetTextureColorMod(m_transparentBlockTexture, 255, 255, 255);
}

void Renderer::DrawText(const std::string& inString, Uint32 xPos, Uint32 yPos, const Color color,
    const Uint32 size, const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment)
{
    TTF_SetFontSize(m_font, size);
    TTF_Text* text = TTF_CreateText(m_textEngine, m_font, inString.c_str(), sizeof(inString));
    TTF_SetTextColor(text, color.red, color.green, color.blue, 255);

    int textWidth;
    int textHeight;
    TTF_GetTextSize(text, &textWidth, &textHeight);
    switch (horizontalAlignment)
    {
        case HorizontalAlignment::Left:
            break;
        case HorizontalAlignment::Center:
            xPos -= textWidth / 2;
            break;
        case HorizontalAlignment::Right:
            xPos -= textWidth;
            break;
    }

    switch (verticalAlignment)
    {
        case VerticalAlignment::Top:
            yPos -= textHeight;
            break;
        case VerticalAlignment::Center:
            yPos -= textHeight / 2;
            break;
        case VerticalAlignment::Bottom:
            break;
    }

    TTF_DrawRendererText(text, xPos, yPos);
}

void Renderer::DrawRectangle(const Uint32 width, const Uint32 height, const Uint32 xPos, const Uint32 yPos,
    const Color& outlineColor, const Color& fillColor,
    const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment) const
{
    SDL_FRect rectangle;
    rectangle.w = width;
    rectangle.h = height;
    switch (horizontalAlignment)
    {
        case HorizontalAlignment::Left:
            rectangle.x = xPos;
            break;
        case HorizontalAlignment::Center:
            rectangle.x = xPos - width / 2;
            break;
        case HorizontalAlignment::Right:
            rectangle.x = xPos - width;
            break;
    }

    switch (verticalAlignment)
    {
        case VerticalAlignment::Top:
            rectangle.y = yPos - height;
            break;
        case VerticalAlignment::Center:
            rectangle.y = yPos - height / 2;
            break;
        case VerticalAlignment::Bottom:
            rectangle.y = yPos;
            break;
    }

    SDL_SetRenderDrawColor(m_renderer, fillColor.red, fillColor.green, fillColor.blue, 255);
    SDL_RenderFillRect(m_renderer, &rectangle);

    SDL_SetRenderDrawColor(m_renderer, outlineColor.red, outlineColor.green, outlineColor.blue, 255);
    SDL_RenderRect(m_renderer, &rectangle);

    ResetDrawColor();
}

void Renderer::Init()
{
    m_window = SDL_CreateWindow("Tetris", 500, 1000, SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Window." << std::endl;
        m_application.Stop();
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Renderer." << std::endl;
        m_application.Stop();
        return;
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    if (!TTF_Init())
    {
        std::cerr << "Renderer::Init::SDL-ttf failed to initialize." << std::endl;
        m_application.Stop();
        return;
    }

    m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
    if (!m_textEngine)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Text Renderer Engine" << std::endl;
        m_application.Stop();
        return;
    }

    SDL_SetRenderVSync(m_renderer, 1);

    SetBlockTexture(m_blockTexturePath);
    SetTransparentBlockTexture(m_transparentBlockTexturePath);
    SetFont(m_defaultFontPath);

    SDL_SetTextureScaleMode(m_blockTexture, SDL_SCALEMODE_PIXELART);
    SDL_SetTextureScaleMode(m_transparentBlockTexture, SDL_SCALEMODE_PIXELART);
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

void Renderer::SetRendererSizeToWindowSize() const
{
    int width;
    int height;
    SDL_GetWindowSize(m_window, &width, &height);
    SDL_SetRenderLogicalPresentation(m_renderer, width, height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
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

Uint32 Renderer::GetRenderWidth() const
{
    int width;
    if (!SDL_GetCurrentRenderOutputSize(m_renderer, &width, nullptr))
    {
        return 0;
    }
    return width;
}

Uint32 Renderer::GetRenderHeight() const
{
    int height;
    if (!SDL_GetCurrentRenderOutputSize(m_renderer, nullptr, &height))
    {
        return 0;
    }
    return height;
}

TTF_Font* Renderer::GetDefaultFont() const
{
    return m_font;
}
