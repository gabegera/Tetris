#include "Application.h"

#include <iostream>

#include "Game.h"
#include "SDL3/SDL_events.h"
#include "UI/MainMenu.h"

Application::Application() : m_renderer(*this)
{

}

Application::~Application()
{

}

void Application::UpdateDeltaTime()
{
    const Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - m_lastTime) / 1000.0f;
    m_lastTime = currentTime;
}

void Application::ProcessEvents()
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
                    OnExitInput();
                    break;
                case SDLK_LEFT:
                case SDLK_A:
                    OnLeftInput();
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    OnRightInput();
                    break;
                case SDLK_UP:
                case SDLK_W:
                    OnUpInput();
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    OnDownInput();
                    break;
                case SDLK_SPACE:
                    OnRotateInput();
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
        {
            switch (event.gbutton.button)
            {
                case SDL_GAMEPAD_BUTTON_START:
                    OnExitInput();
                    break;
                case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                    OnLeftInput();
                    break;
                case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                    OnRightInput();
                    break;
                case SDL_GAMEPAD_BUTTON_DPAD_UP:
                    OnUpInput();
                    break;
                case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                    OnDownInput();
                    break;
                case SDL_GAMEPAD_BUTTON_SOUTH:
                    OnRotateInput();
                default:
                    break;
            }
        }
    }
}

void Application::OnExitInput()
{
    if (m_game)
    {
        StopGame();
    }
}

void Application::OnLeftInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->MoveShapeLeft();
    }
}

void Application::OnRightInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->MoveShapeRight();
    }
}

void Application::OnUpInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->DropShape();
    }
    else if (m_mainMenu)
    {
        m_mainMenu->ReceiveUpInput();
    }
}

void Application::OnDownInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->MoveShapeDown();
    }
    else if (m_mainMenu)
    {
        m_mainMenu->ReceiveDownInput();
    }
}

void Application::OnRotateInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->RotateShape();
    }
    else if (m_mainMenu)
    {
        m_mainMenu->ReceiveSelectInput();
    }
}

void Application::StartMainMenu()
{
    m_mainMenu = std::make_unique<MainMenu>(*this);
    m_mainMenu->Init();
}

void Application::StartGame()
{
    m_game = std::make_unique<Game>(*this);
    m_game->Start();

    m_mainMenu->SetVisibility(false);
}

void Application::StopGame()
{
    if (m_game)
    {
        m_game->Stop();
        m_game = nullptr;
    }

    m_mainMenu->SetVisibility(true);
    m_renderer.SetRendererSizeToWindowSize();
}

void Application::Start()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::cerr << "Game::Start::SDL failed to Initialize." << std::endl;
        Stop();
    }

    m_renderer.Init();

    StartMainMenu();

    // StartGame();
}

void Application::Stop()
{
    m_isRunning = false;
    m_renderer.Stop();
    SDL_Quit();
}

void Application::Update()
{
    UpdateDeltaTime();

    ProcessEvents();

    m_renderer.ClearRenderer();

    if (m_mainMenu)
    {
        m_mainMenu->Update(m_deltaTime);
    }

    if (m_game)
    {
        m_game->Update(m_deltaTime);
    }

    m_renderer.Update(m_deltaTime);
}

void Application::RestartGame()
{
    m_game.reset();
    m_game = std::make_unique<Game>(*this);
}

bool Application::IsRunning() const
{
    return m_isRunning;
}

bool Application::IsGameRunning() const
{
    return m_game != nullptr;
}

float Application::GetDeltaTime() const
{
    return m_deltaTime;
}

Renderer* Application::GetRenderer()
{
    return &m_renderer;
}

Game* Application::GetGame() const
{
    return m_game.get();
}
