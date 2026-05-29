#include "Button.h"

#include <iostream>

#include "../Application.h"
#include "Menu.h"

Button::Button(Menu& menu) : m_menu(menu)
{

}

Button::~Button()
{

}

void Button::RenderButton()
{
    GetRenderer()->DrawButton(this);
}

void Button::Init()
{

}

void Button::Update(const float deltaTime)
{
    if (!IsVisible()) return;

    RenderButton();
}

void Button::BindFunction(const std::function<void()>& inFunction)
{
    m_boundFunction = inFunction;
}

void Button::PressButton()
{
    try
    {
        m_boundFunction();
    }
    catch (const std::bad_function_call& error)
    {
        std::cerr << "Button::PressButton::Invalid Bound Function." << std::endl;
    }
}

bool Button::SetVisibility(const bool input)
{
    return m_isVisible = input;
}

void Button::SetButtonText(const std::string& inText)
{
    m_buttonText = inText;
}

void Button::SetFontSize(const Uint32 inSize)
{
    m_fontSize = inSize;
}

void Button::SetTextColor(const Color inColor)
{
    m_textColor = inColor;
}

void Button::SetBackgroundColor(const Color inColor)
{
    m_backgroundColor = inColor;
}

void Button::SetOutlineColor(const Color inColor)
{
    m_outlineColor = inColor;
}

void Button::SetWidth(const Uint32 inWidth)
{
    m_width = inWidth;
}

void Button::SetHeight(const Uint32 inHeight)
{
    m_height = inHeight;
}

void Button::SetXPos(const Uint32 inXPos)
{
    m_xPos = inXPos;
}

void Button::SetYPos(const Uint32 inYPos)
{
    m_yPos = inYPos;
}

void Button::SetHorizontalAlignment(const HorizontalAlignment inAlignment)
{
    m_horizontalAlignment = inAlignment;
}

void Button::SetVerticalAlignment(const VerticalAlignment inAlignment)
{
    m_verticalAlignment = inAlignment;
}

Renderer* Button::GetRenderer() const
{
    return m_menu.GetRenderer();
}

Menu* Button::GetOwningMenu() const
{
    return &m_menu;
}

bool Button::IsVisible() const
{
    return m_isVisible;
}

bool Button::IsSelected() const
{
    return m_menu.GetSelectedButton() == this;
}

const std::string& Button::GetButtonText() const
{
    return m_buttonText;
}

Uint32 Button::GetFontSize() const
{
    return m_fontSize;
}

const Color& Button::GetTextColor() const
{
    return m_textColor;
}

const Color& Button::GetBackgroundColor() const
{
    return m_backgroundColor;
}

const Color& Button::GetOutlineColor() const
{
    return m_outlineColor;
}

const Color& Button::GetHighlightedBackgroundColor() const
{
    return m_highlightedBackgroundColor;
}

const Color& Button::GetHighlightedOutlineColor() const
{
    return m_highlightedOutlineColor;
}

Uint32 Button::GetWidth() const
{
    return m_width;
}

Uint32 Button::GetHeight() const
{
    return m_height;
}

Uint32 Button::GetXPos() const
{
    return m_xPos;
}

Uint32 Button::GetYPos() const
{
    return m_yPos;
}

HorizontalAlignment Button::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

VerticalAlignment Button::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}
