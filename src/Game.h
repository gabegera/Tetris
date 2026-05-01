#pragma once

#include <memory>

#include "BlockManager.h"
#include "Renderer.h"
#include "SDL3/SDL.h"

class Game
{
public:
    Game();
    ~Game();

protected:
    BlockManager m_blockManager;
    Renderer m_renderer;

    const Uint8 m_gameWidth = 10;
    const Uint8 m_gameHeight = 20;

    const bool m_shouldDrawInConsole = true;

    bool m_isRunning = false;

    void ProcessInput();

public:
    void Start();
    void Stop();

    void Update();

    [[nodiscard]] bool IsRunning() const;

    BlockManager* GetBlockManager();
    Renderer* GetRenderer();

    Uint8 GetGameWidth() const;
    Uint8 GetGameHeight() const;
};
