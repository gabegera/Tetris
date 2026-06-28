#include "Renderer.h"

#include <iostream>

#include "Application.h"
#include "SDL3_image/SDL_image.h"
#include "UI/Button.h"

Renderer::Renderer(Application& app) : m_application(app)
{

}

Renderer::~Renderer()
{

}

// void Renderer::SetBlockTexture(const std::filesystem::path& texturePath)
// {
//     SDL_Surface* blockSurface = SDL_LoadSurface(texturePath.generic_string().c_str());
//     m_blockTexture = SDL_CreateTextureFromSurface(m_renderer, blockSurface);
//     SDL_DestroySurface(blockSurface);
// }
//
// void Renderer::SetTransparentBlockTexture(const std::filesystem::path& texturePath)
// {
//     SDL_Surface* blockSurface = SDL_LoadSurface(texturePath.generic_string().c_str());
//     m_transparentBlockTexture = SDL_CreateTextureFromSurface(m_renderer, blockSurface);
//     SDL_DestroySurface(blockSurface);
// }

// void Renderer::SetFont(const std::filesystem::path& fontPath)
// {
//     m_font = TTF_OpenFont(fontPath.generic_string().c_str(), 16);
// }

// void Renderer::SetColorPalette(const ColorPalette& newPalette)
// {
//     m_colorPalette = newPalette;
// }

void Renderer::ResetDrawColor() const
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}

void Renderer::CreateTexturesFromTheme()
{
    m_shapeTextures.clear();
    for (const Shape* shape : ClassicShapes::Get())
    {
        SDL_Texture* shapeTexture = SDL_CreateTextureFromSurface(m_renderer, m_application.GetTheme()->GetShapeSurface(shape));

        if (!shapeTexture)
        {
            std::cerr << "Renderer::CreateTexturesFromTheme::Failed to create shape texture" << std::endl;
            continue;
        }

        SDL_SetTextureScaleMode(shapeTexture, SDL_SCALEMODE_PIXELART);
        m_shapeTextures.try_emplace(shape, shapeTexture);
    }

    SDL_Texture* borderTexture = SDL_CreateTextureFromSurface(m_renderer, m_application.GetTheme()->GetBorderSurface());

    if (!borderTexture)
    {
        std::cerr << "Renderer::CreateTexturesFromTheme::Failed to create border texture" << std::endl;
    }

    SDL_SetTextureScaleMode(borderTexture, SDL_SCALEMODE_PIXELART);

    m_borderTexture = borderTexture;
}

// Draws a transparent guide to show where your shape is going to land.
void Renderer::DrawShapeGuideAtPos(const Position targetPos, const Shape* shape, const RotationState shapeRotationState) const
{
    // SDL_FRect block;
    // block.w = m_blockResolution;
    // block.h = m_blockResolution;
    //
    // const std::vector<bool> stateShape = shape->GetRotationSateData(shapeRotationState);
    // const unsigned int shapeWidth = shape->GetShapeDimensions();
    //
    // for (int i = 0; i < stateShape.size(); i++)
    // {
    //     if (stateShape[i] == false) continue;
    //
    //     block.x = targetPos.x + i % shapeWidth;
    //     block.x *= m_blockResolution;
    //     block.y = targetPos.y + std::floor(i / shapeWidth);
    //     block.y *= m_blockResolution;
    //
    //     if (m_shapeTextures.contains(shape))
    //     {
    //         SDL_Texture* texture = m_shapeTextures.at(shape);
    //         auto [red, green, blue, alpha] = m_application.GetTheme()->GetShapeColor(shape);
    //         SDL_SetTextureColorMod(texture, red, green, blue);
    //         SDL_SetTextureAlphaMod(texture, m_shapeGuideOpacity);
    //         SDL_RenderTexture(m_renderer, texture, nullptr, &block);
    //         SDL_SetTextureAlphaMod(texture, 255);
    //     }
    // }
}

void Renderer::DrawText(const std::string& inString, Uint32 xPos, Uint32 yPos, const Color color,
    const Uint32 size, const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment) const
{
    TTF_Font* font = m_application.GetTheme()->GetFont();
    TTF_SetFontSize(font, size);
    TTF_Text* text = TTF_CreateText(m_textEngine, font, inString.c_str(), sizeof(inString));
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
    ResetDrawColor();

    m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
    if (!m_textEngine)
    {
        std::cerr << "Renderer::Init::SDL failed to Create Text Renderer Engine" << std::endl;
        m_application.Stop();
        return;
    }

    SDL_SetRenderVSync(m_renderer, 1);

    CreateTexturesFromTheme();
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

void Renderer::DrawBlockAtPos(const unsigned int xPos, unsigned int yPos, const Shape* owningShape) const
{
    SDL_Texture* texture = nullptr;
    if (m_shapeTextures.contains(owningShape))
    {
        texture = m_shapeTextures.at(owningShape);
    }

    DrawBlockAtPos(xPos, yPos, texture, m_application.GetTheme()->GetShapeColor(owningShape));
}

void Renderer::DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, SDL_Texture* texture, const Color& color) const
{
    SDL_FRect block;
    block.w = m_blockResolution;
    block.h = m_blockResolution;
    block.x = xPos * m_blockResolution;
    block.y = yPos * m_blockResolution;

    if (texture)
    {
        SDL_SetTextureColorMod(texture, color.red, color.green, color.blue);
        SDL_RenderTexture(m_renderer, texture, nullptr, &block);
    }
    else
    {
        constexpr Uint8 alpha = 255;
        SDL_SetRenderDrawColor(m_renderer, color.red, color.green, color.blue, alpha);
        SDL_RenderFillRect(m_renderer, &block);
    }

    ResetDrawColor();
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

SDL_Window* Renderer::GetWindow() const
{
    return m_window;
}

SDL_Renderer* Renderer::GetSDLRenderer() const
{
    return m_renderer;
}

SDL_Texture* Renderer::GetShapeTexture(const Shape* shape) const
{
    try
    {
        return m_shapeTextures.at(shape);
    }
    catch (const std::out_of_range& error)
    {
        return nullptr;
    }
}

SDL_Texture* Renderer::GetBorderTexture() const
{
    return m_borderTexture;
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
