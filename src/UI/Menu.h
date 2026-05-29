#pragma once

#include <memory>
#include <vector>

#include "SDL3/SDL_stdinc.h"
#include "../ColorPalettes.h"

class Renderer;
class Button;
class Application;

class Menu
{
public:
    explicit Menu(Application& app);
    virtual ~Menu();

protected:
    Application& m_application;

    std::vector<std::unique_ptr<Button>> m_buttons;
    Uint32 m_selectedButtonIndex = 0;

    Uint32 m_defaultButtonWidth = 256;
    Uint32 m_defaultButtonHeight = 64;

    bool m_isVisible = true;

    Button* CreateButton();

    void RenderButtons() const;

    void SelectNextButton();
    void SelectPreviousButton();

public:
    virtual void Init();
    virtual void Update(float deltaTime);

    virtual void ReceiveUpInput();
    virtual void ReceiveDownInput();
    virtual void ReceiveLeftInput();
    virtual void ReceiveRightInput();
    virtual void ReceiveSelectInput();

    bool SetVisibility(bool input);

    bool IsVisible() const;

    Button* GetSelectedButton() const;

    Application* GetApplication() const;
    Renderer* GetRenderer() const;
};
