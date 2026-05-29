#pragma once

#include <functional>
#include <string>

#include "SDL3/SDL_stdinc.h"
#include "../ColorPalettes.h"
#include "../Renderer.h"

class Renderer;
class Menu;
enum class HorizontalAlignment : Uint8;
enum class VerticalAlignment : Uint8;

class Button
{
public:
    explicit Button(Menu& menu);
    virtual ~Button();

protected:
    Menu& m_menu;

    Uint32 m_width = 128;
    Uint32 m_height = 48;
    Uint32 m_xPos = 0;
    Uint32 m_yPos = 0;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;
    VerticalAlignment m_verticalAlignment = VerticalAlignment::Center;

    std::string m_buttonText;
    Uint32 m_fontSize = 32;
    Color m_textColor;

    Color m_backgroundColor = {50, 50, 50};
    Color m_outlineColor = {100, 100, 100};
    Color m_highlightedBackgroundColor = {100, 100, 100};
    Color m_highlightedOutlineColor = {150, 150, 150};

    std::function<void()> m_boundFunction;

    bool m_isVisible = true;

    void RenderButton();

public:
    void Init();
    void Update(float deltaTime);

    void BindFunction(const std::function<void()>& inFunction);

    virtual void PressButton();

    bool SetVisibility(bool input);

    void SetButtonText(const std::string& inText);
    void SetFontSize(Uint32 inSize);
    void SetTextColor(Color inColor);

    void SetBackgroundColor(Color inColor);
    void SetOutlineColor(Color inColor);

    void SetWidth(Uint32 inWidth);
    void SetHeight(Uint32 inHeight);
    void SetXPos(Uint32 inXPos);
    void SetYPos(Uint32 inYPos);
    void SetHorizontalAlignment(HorizontalAlignment inAlignment);
    void SetVerticalAlignment(VerticalAlignment inAlignment);

    Renderer* GetRenderer() const;
    Menu* GetOwningMenu() const;

    bool IsVisible() const;

    bool IsSelected() const;

    const std::string& GetButtonText() const;
    Uint32 GetFontSize() const;
    const Color& GetTextColor() const;

    const Color& GetBackgroundColor() const;
    const Color& GetOutlineColor() const;
    const Color& GetHighlightedBackgroundColor() const;
    const Color& GetHighlightedOutlineColor() const;

    Uint32 GetWidth() const;
    Uint32 GetHeight() const;
    Uint32 GetXPos() const;
    Uint32 GetYPos() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    VerticalAlignment GetVerticalAlignment() const;
};
