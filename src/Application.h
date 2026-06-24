#pragma once

#include "Game.h"
#include "InputHandler.h"
#include "PlayerController.h"
#include "Renderer.h"
#include "SDL3/SDL_stdinc.h"

class MainMenu;

class Application
{
public:
    Application();
    ~Application();

protected:
    InputHandler m_inputHandler;
    PlayerController m_playerController;
    Renderer m_renderer;

    std::unique_ptr<Theme> m_theme = std::make_unique<Theme>();

    std::unique_ptr<Menu> m_activeMenu;
    std::unique_ptr<Game> m_game;

    bool m_isRunning = true;

    Uint32 m_lastTime = 0;
    float m_deltaTime = 0.0f;

    void UpdateDeltaTime();

    void ProcessEvents();

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

    InputHandler* GetInputHandler();
    const InputHandler* GetInputHandler() const;

    PlayerController* GetPlayerController();
    const PlayerController* GetPlayerController() const;

    Renderer* GetRenderer();

    Menu* GetActiveMenu() const;

    Game* GetGame() const;

    template<typename T>
    void SetTheme() requires std::is_base_of_v<Theme, T>
    {
        m_theme = std::make_unique<T>();
        m_theme->Init();
        m_renderer.CreateTexturesFromTheme();
    }

    const Theme* GetTheme() const;
};
