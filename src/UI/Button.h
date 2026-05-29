#pragma once

#include <functional>
#include <string>

#include "UIElement.h"
#include "SDL3/SDL_stdinc.h"

class Renderer;
class Menu;

class Button : public UIElement
{
public:
    explicit Button(Menu& menu);
    ~Button() override;

protected:
    Uint32 m_width = 128;
    Uint32 m_height = 48;

    std::string m_buttonText;
    Uint32 m_fontSize = 32;
    Color m_textColor = {255, 255, 255};

    Color m_backgroundColor = {50, 50, 50};
    Color m_outlineColor = {100, 100, 100};
    Color m_highlightedBackgroundColor = {100, 100, 100};
    Color m_highlightedOutlineColor = {150, 150, 150};

    std::function<void()> m_boundFunction;

    void Render() override;

public:
    void BindFunction(const std::function<void()>& inFunction);

    void TriggerElement() override;

    void SetButtonText(const std::string& inText);
    void SetFontSize(Uint32 inSize);
    void SetTextColor(Color inColor);

    void SetBackgroundColor(Color inColor);
    void SetOutlineColor(Color inColor);

    void SetWidth(Uint32 inWidth);
    void SetHeight(Uint32 inHeight);

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
};
