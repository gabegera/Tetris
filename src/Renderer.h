#pragma once

#include <string>

#include "ColorPalettes.h"
#include "SDL3/SDL.h"


struct ColorPalette;
struct Block;
class Game;
class Renderer
{
public:
    explicit Renderer(Game& game);
    ~Renderer();

protected:
    Game& m_game;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;

    // Width of the blocks in pixels.
    const unsigned int m_blockWidth = 16;

    SDL_Texture* m_blockTexture;

    // If set to true, the game will be drawn using text in the console.
    const bool m_shouldDrawInConsole = false;

    void SetBlockTexture(const std::string& texturePath);

    void DrawInConsole() const;

    void DrawBorders() const;
    void DrawBlocks() const;

    void DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, Color color) const;

public:
    void Init();
    void Stop() const;
    void Update(const float deltaTime);

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetRenderer() const;

    unsigned int GetRenderWidth() const;
    unsigned int GetRenderHeight() const;

};
