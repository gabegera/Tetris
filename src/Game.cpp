#include "Game.h"

#include <iostream>

Game::Game() : m_blockManager(*this), m_renderer(*this) {}

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

        if (event.type == SDL_EVENT_GAMEPAD_ADDED)
        {
            SDL_OpenGamepad(event.gdevice.which);
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
                    m_blockManager.MoveFallingBlocksHorizontal(-1);
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    m_blockManager.MoveFallingBlocksHorizontal(1);
                    break;
                case SDLK_UP:
                case SDLK_W:
                    m_blockManager.DropFallingBlocks();
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    m_blockManager.MoveFallingBlocksDown();
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
        {
            switch (event.gbutton.button)
            {
                case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                    m_blockManager.MoveFallingBlocksHorizontal(-1);
                    break;
                case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                    m_blockManager.MoveFallingBlocksHorizontal(1);
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

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::cerr << "Game::Start::SDL failed to Initialize." << std::endl;
        Stop();
    }



    m_blockManager.Init();

    m_renderer.Init();
}

void Game::Stop()
{
    m_renderer.Stop();
    SDL_Quit();
    m_isRunning = false;
}

void Game::Update()
{
    const Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - m_lastTime) / 1000.0f;
    m_lastTime = currentTime;

    ProcessInput();

    m_blockManager.Update(m_deltaTime);

    m_renderer.Update(m_deltaTime);
}

bool Game::IsRunning() const
{
    return m_isRunning;
}
float Game::GetDeltaTime() const
{
    return m_deltaTime;
}

BlockManager* Game::GetBlockManager()
{
    return &m_blockManager;
}

Renderer* Game::GetRenderer()
{
    return &m_renderer;
}


