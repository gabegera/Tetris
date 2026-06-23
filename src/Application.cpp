#include "Application.h"

#include <iostream>

#include "Game.h"
#include "SDL3/SDL_events.h"
#include "UI/MainMenu.h"

Application::Application() : m_inputHandler(*this), m_renderer(*this)
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
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            Stop();
            break;
        case SDL_EVENT_GAMEPAD_ADDED:
            SDL_OpenGamepad(event.gdevice.which);
            break;
        case SDL_EVENT_KEY_DOWN:
            m_inputHandler.KeyDown(event.key.key);
            break;
        case SDL_EVENT_KEY_UP:
            m_inputHandler.KeyUp(event.key.key);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            m_inputHandler.MouseButtonDown(event.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            m_inputHandler.MouseButtonUp(event.button.button);
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            m_inputHandler.GamepadButtonDown(event.gbutton.button);
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            m_inputHandler.GamepadButtonUp(event.gbutton.button);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            m_inputHandler.MouseMoved();
            break;
        default:
            break;
        }
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

    m_theme->Init();

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

    m_inputHandler.Update(m_deltaTime);

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

Menu* Application::GetActiveMenu() const
{
    return m_activeMenu.get();
}

Game* Application::GetGame() const
{
    return m_game.get();
}

const Theme* Application::GetTheme() const
{
    return m_theme.get();
}
