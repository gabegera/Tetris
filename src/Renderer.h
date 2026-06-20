#pragma once

#include <filesystem>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Theme.h"
#include "UI/UIElement.h"

class Button;
struct Shape;
struct Block;
class Application;

class Renderer
{
public:
    explicit Renderer(Application& app);
    ~Renderer();

protected:
    Application& m_application;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_TextEngine* m_textEngine = nullptr;

    Uint8 m_blockResolution = 16;

    const Uint8 m_shapeGuideOpacity = 100;

    std::map<const Shape*, SDL_Texture*> m_shapeTextures;
    SDL_Texture* m_borderTexture;

    void ResetDrawColor() const;

public:
    void Init();
    void Stop() const;
    void Update(const float deltaTime) const;

    void SetRendererSizeToWindowSize() const;
    void SetRendererSizeInPixels(Uint16 width, Uint16 height) const;
    void SetRendererSizeInGameDimensions(Uint16 gameWidth, Uint16 gameHeight) const;

    void ClearRenderer() const;

    void CreateTexturesFromTheme();

    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, const Shape* owningShape) const;
    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, SDL_Texture* texture, const Color& color) const;

    void DrawShapeGuideAtPos(Uint8 xPos, Uint8 yPos, const Shape* currentFallingShape, const Shape* originalFallingShape) const;

    void DrawText(const std::string& inString, Uint32 xPos, Uint32 yPos, Color color, Uint32 size,
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
        VerticalAlignment verticalAlignment = VerticalAlignment::Top) const;

    void DrawRectangle(Uint32 width, Uint32 height, Uint32 xPos, Uint32 yPos,
        const Color& outlineColor = {0, 0, 0}, const Color& fillColor = {255, 255, 255},
        HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left,
        VerticalAlignment verticalAlignment = VerticalAlignment::Bottom) const;

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetSDLRenderer() const;

    SDL_Texture* GetShapeTexture(const Shape* shape) const;
    SDL_Texture* GetBorderTexture() const;

    Uint32 GetRenderWidth() const;
    Uint32 GetRenderHeight() const;

};
