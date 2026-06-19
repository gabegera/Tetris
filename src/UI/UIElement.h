#pragma once

#include "SDL3/SDL_stdinc.h"
#include "UIAlignment.h"
#include "Menu.h"

class Menu;

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

    virtual void GetBounds(Uint32& outLowerX, Uint32& outUpperX, Uint32& outLowerY, Uint32& outUpperY) const;
    bool IsPositionWithinBounds(Uint32 xPos, Uint32 yPos) const;

    bool IsVisible() const;

    bool IsSelectable() const;
    virtual bool IsSelected() const;

    Uint32 GetXPos() const;
    Uint32 GetYPos() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAnchor() const;
    VerticalAlignment GetVerticalAnchor() const;
};
