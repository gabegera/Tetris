#pragma once

#include "SDL3/SDL_stdinc.h"
#include "Menu.h"
#include "../ColorPalettes.h"

class Menu;

enum class HorizontalAlignment : Uint8
{
    Left = 0,
    Center = 1,
    Right = 2
};

enum class VerticalAlignment : Uint8
{
    Top = 0,
    Center = 1,
    Bottom = 2
};

class UIElement
{
public:
    explicit UIElement(Menu& menu, bool isSelectable = false);
    virtual ~UIElement();

protected:
    Menu& m_menu;

    Uint32 m_xOffset = 0;
    Uint32 m_yOffset = 0;
    HorizontalAlignment m_horizontalAnchor = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAnchor = VerticalAlignment::Top;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment = VerticalAlignment::Top;

    bool m_isVisible = true;

    const bool m_isSelectable = false;

    virtual void Render() = 0;

public:
    virtual void Init();
    virtual void Update(float deltaTime);

    Renderer* GetRenderer() const;
    Menu* GetOwningMenu() const;

    virtual void TriggerElement();

    bool SetVisibility(bool input);

    void SetXOffset(Uint32 inXPos);
    void SetYOffset(Uint32 inYPos);
    void SetHorizontalAlignment(HorizontalAlignment inAlignment);
    void SetVerticalAlignment(VerticalAlignment inAlignment);
    void SetHorizontalAnchor(HorizontalAlignment inAnchor);
    void SetVerticalAnchor(VerticalAlignment inAnchor);

    bool IsVisible() const;

    bool IsSelectable() const;

    Uint32 GetXPos() const;
    Uint32 GetYPos() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAnchor() const;
    VerticalAlignment GetVerticalAnchor() const;
};
