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
    std::unique_ptr<BlockManager> m_blockManager = std::make_unique<BlockManager>(*this);
    std::unique_ptr<Renderer> m_renderer = std::make_unique<Renderer>(*this);

    const uint8_t m_gameWidth = 10;
    const uint8_t m_gameHeight = 20;

    const bool m_shouldDrawInConsole = true;

    bool m_isRunning = false;

    void ProcessInput();

public:
    void Start();
    void Stop();

    void Update();

    [[nodiscard]] bool IsRunning() const;

    BlockManager* GetBlockManager() const;
    Renderer* GetRenderer() const;

    uint8_t GetGameWidth() const;
    uint8_t GetGameHeight() const;
};
