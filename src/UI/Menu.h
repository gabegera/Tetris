#pragma once

#include <memory>
#include <vector>

#include "SDL3/SDL_stdinc.h"

class TextBlock;
class Button;
class UIElement;
class Renderer;
class Application;

class Menu
{
public:
    explicit Menu(Application& app);
    virtual ~Menu();

protected:
    Application& m_application;

    std::vector<std::unique_ptr<UIElement>> m_children;
    bool m_isElementSelected = false;
    Uint32 m_selectedUIElementIndex = 0;

    Uint32 m_defaultButtonWidth = 256;
    Uint32 m_defaultButtonHeight = 64;

    bool m_isVisible = true;

    TextBlock* CreateTextBlock();

    Button* CreateButton();

public:
    virtual void Init();
    virtual void Update(float deltaTime);

    void SelectNextElement();
    void SelectPreviousElement();
    void TriggerSelectedElement() const;
    void TriggerElementBelowCursor();

    void CheckMousePosition();

    bool SetVisibility(bool input);

    bool IsVisible() const;

    const UIElement* GetSelectedElement() const;

    Application* GetApplication() const;
    Renderer* GetRenderer() const;
};
