#pragma once

#include <string>

#include "ColorPalettes.h"
#include "SDL3/SDL.h"

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

    // Width and Height of the blocks in pixels.
    const unsigned int m_blockResolution = 16;

    SDL_Texture* m_blockTexture = nullptr;

    ColorPalette m_colorPalette = ColorPalettes::Classic;

    void SetBlockTexture(const std::string& texturePath);

    void SetColorPalette(const ColorPalette& newPalette);

public:
    void Init();
    void Stop() const;
    void Update(const float deltaTime) const;

    void SetRendererSizeInPixels(Uint16 width, Uint16 height) const;
    void SetRendererSizeInGameDimensions(Uint16 gameWidth, Uint16 gameHeight) const;

    void ClearRenderer() const;

    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, Uint16 colorID) const;
    void DrawBlockAtPos(unsigned int xPos, unsigned int yPos, Color color) const;

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetSDLRenderer() const;

    const ColorPalette& GetColorPalette() const;

};
