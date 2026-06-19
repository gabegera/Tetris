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
                case SDLK_E:
                case SDLK_RETURN:
                    OnRotateClockwiseInput();
                    break;
                case SDLK_LALT:
                case SDLK_Q:
                    OnRotateCounterClockwiseInput();
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
                    OnRotateClockwiseInput();
                case SDL_GAMEPAD_BUTTON_EAST:
                    OnRotateCounterClockwiseInput();
                default:
                    break;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            OnMouseMoved();
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                OnLeftMouseButtonUp();
                break;
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
    else if (m_activeMenu)
    {
        m_activeMenu->ReceiveUpInput();
    }
}

void Application::OnDownInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->MoveShapeDown();
    }
    else if (m_activeMenu)
    {
        m_activeMenu->ReceiveDownInput();
    }
}

void Application::OnRotateClockwiseInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->RotateShapeClockwise();
    }
    else if (m_activeMenu)
    {
        m_activeMenu->ReceiveSelectInput();
    }
}

void Application::OnRotateCounterClockwiseInput()
{
    if (IsGameRunning())
    {
        m_game->GetBlockManager()->RotateShapeCounterClockwise();
    }
    else if (m_activeMenu)
    {

    }
}

void Application::OnMouseMoved()
{
    if (IsGameRunning())
    {

    }
    else if (m_activeMenu)
    {
        m_activeMenu->ReceiveMouseInput();
    }
}

void Application::OnLeftMouseButtonUp()
{
    if (IsGameRunning())
    {

    }
    else if (m_activeMenu)
    {
        m_activeMenu->ReceiveLeftMouseButtonUpInput();
    }
}

void Application::StartGame()
{
    m_game = std::make_unique<Game>(*this);
    m_game->Start();

    m_activeMenu->SetVisibility(false);
}

void Application::StopGame()
{
    if (m_game)
    {
        m_game->Stop();
        m_game = nullptr;
    }

    m_activeMenu->SetVisibility(true);
    m_renderer.SetRendererSizeToWindowSize();
}

void Application::Start()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::cerr << "Application::Start::SDL failed to Initialize." << std::endl;
        Stop();
    }

    if (!TTF_Init())
    {
        std::cerr << "Application::Start::SDL-ttf failed to initialize." << std::endl;
        Stop();
        return;
    }

    m_theme.Init();

    m_renderer.Init();

    OpenMenu<MainMenu>();
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

    if (m_activeMenu)
    {
        m_activeMenu->Update(m_deltaTime);
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

const Theme* Application::GetTheme() const
{
    return &m_theme;
}
