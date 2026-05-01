#include "Game.h"

#include <iostream>

Game::Game()
{

}

Game::~Game()
{

}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            Stop();
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
                case SDLK_ESCAPE:
                    Stop();
                    break;
                case SDLK_LEFT:
                case SDLK_A:
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    break;
                case SDLK_UP:
                case SDLK_W:
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::Start()
{
    m_isRunning = true;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Game::Start::SDL failed to Initialize." << std::endl;
        Stop();
    }

    m_renderer->Init();
}

void Game::Stop()
{
    m_renderer->Stop();
    SDL_Quit();
    m_isRunning = false;
}

void Game::Update()
{
    ProcessInput();

    m_blockManager->Update();

    m_renderer->Update();
}

bool Game::IsRunning() const
{
    return m_isRunning;
}

BlockManager* Game::GetBlockManager() const
{
    return m_blockManager.get();
}

Renderer* Game::GetRenderer() const
{
    return m_renderer.get();
}

uint8_t Game::GetGameWidth() const
{
    return m_gameWidth;
}

uint8_t Game::GetGameHeight() const
{
    return m_gameHeight;
}
