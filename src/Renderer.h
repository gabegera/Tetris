#pragma once

#include <string>


#include "SDL3/SDL.h"


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

    const unsigned int m_blockSize = 16;

    SDL_Texture* m_blockTexture;

    const bool m_shouldDrawInConsole = false;

    void SetBlockTexture(const std::string& texturePath);

    void DrawInConsole() const;

    void DrawBorders() const;
    void DrawBlocks() const;

    void DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos) const;
    void DrawBlockAtPos(const unsigned int xPos, const unsigned int yPos, const uint8_t red, const uint8_t green, const uint8_t blue) const;

public:
    void Init();
    void Stop() const;

    void Update();

    SDL_Window* GetWindow() const;
    SDL_Renderer* GetRenderer() const;

    unsigned int GetRenderWidth() const;
    unsigned int GetRenderHeight() const;

};
