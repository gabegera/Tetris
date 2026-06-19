#pragma once

#include "Game.h"
#include "Renderer.h"
#include "SDL3/SDL_stdinc.h"

class MainMenu;

class Application
{
public:
    Application();
    ~Application();

protected:
    Theme m_theme;

    std::unique_ptr<Menu> m_activeMenu;
    std::unique_ptr<Game> m_game;
    Renderer m_renderer;

    bool m_isRunning = true;

    Uint32 m_lastTime = 0;
    float m_deltaTime = 0.0f;

    void UpdateDeltaTime();

    void ProcessEvents();

    void OnExitInput();
    void OnLeftInput();
    void OnRightInput();
    void OnUpInput();
    void OnDownInput();
    void OnRotateInput();
    void OnMouseMoved();
    void OnLeftMouseButtonUp();

public:
    void Start();
    void Stop();
    void Update();

    void StartGame();
    void StopGame();
    void RestartGame();

    template<typename T>
    void OpenMenu() requires std::is_base_of_v<Menu, T>
    {
        m_activeMenu = std::make_unique<T>(*this);
        m_activeMenu->Init();
    }

    bool IsRunning() const;

    bool IsGameRunning() const;

    float GetDeltaTime() const;

    Renderer* GetRenderer();
    Game* GetGame() const;

    const Theme* GetTheme() const;
};
