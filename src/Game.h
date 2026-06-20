#pragma once

#include "BlockManager.h"

class Renderer;
class Application;

class Game
{
public:
    explicit Game(Application& app, Uint8 gameWidth = 10, Uint8 gameHeight = 20);
    ~Game();

protected:
    // Game Width in number of blocks.
    const Uint8 m_gameWidth;
    // Game Height in number of blocks.
    const Uint8 m_gameHeight;

    Application& m_application;
    BlockManager m_blockManager;

    void RenderBackground() const;
    void RenderBorders() const;
    void RenderBlocks();
    void RenderShapeGuide();

public:
    void Start();
    void Stop();
    void Update(float deltaTime);

    void TriggerGameOver();

    Renderer* GetRenderer() const;
    BlockManager* GetBlockManager();

    Uint8 GetGameWidth() const;
    Uint8 GetGameHeight() const;
};
