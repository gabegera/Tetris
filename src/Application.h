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
    std::unique_ptr<MainMenu> m_mainMenu;
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

    void StartMainMenu();

public:
    void Start();
    void Stop();
    void Update();

    void StartGame();
    void StopGame();
    void RestartGame();

    bool IsRunning() const;

    bool IsGameRunning() const;

    float GetDeltaTime() const;

    Renderer* GetRenderer();
    Game* GetGame() const;
};
