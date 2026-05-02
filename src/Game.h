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

    bool m_isRunning = false;

    Uint32 m_lastTime = 0;
    float m_deltaTime = 0.0f;

    void ProcessInput();

public:
    void Start();
    void Stop();
    void Update();

    BlockManager* GetBlockManager();
    Renderer* GetRenderer();

    [[nodiscard]] bool IsRunning() const;

    float GetDeltaTime() const;
};
