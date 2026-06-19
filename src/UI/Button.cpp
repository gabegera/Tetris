#include "Button.h"

#include <iostream>

#include "../Application.h"
#include "Menu.h"

Button::Button(Menu& menu) : UIElement(menu, true)
{

}

Button::~Button()
{

}

void Button::Render()
{
    const Color backgroundColor = IsSelected() ? m_highlightedBackgroundColor : m_backgroundColor;
    const Color outlineColor = IsSelected() ? m_highlightedOutlineColor : m_outlineColor;
    GetRenderer()->DrawRectangle(m_width, m_height, GetXPos(), GetYPos(), backgroundColor, outlineColor,
        m_horizontalAlignment, m_verticalAlignment);

    if (m_buttonText.empty()) return;

    Uint32 textXPos = GetXPos();
    switch (m_horizontalAlignment)
    {
    case HorizontalAlignment::Left:
        textXPos += m_width / 2;
        break;
    case HorizontalAlignment::Center:
        break;
    case HorizontalAlignment::Right:
        textXPos -= m_width / 2;
        break;
    }

    Uint32 textYPos = GetYPos();
    switch (m_verticalAlignment)
    {
    case VerticalAlignment::Top:
        textYPos -= m_height / 2;
        break;
    case VerticalAlignment::Center:
        break;
    case VerticalAlignment::Bottom:
        textYPos += m_height / 2;
        break;
    }

    GetRenderer()->DrawText(m_buttonText, textXPos, textYPos, m_textColor, m_fontSize,
        HorizontalAlignment::Center, VerticalAlignment::Center);
}

void Button::BindFunction(const std::function<void()>& inFunction)
{
    m_boundFunction = inFunction;
}

void Button::TriggerElement()
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

void Button::GetBounds(Uint32& outLowerX, Uint32& outUpperX, Uint32& outLowerY, Uint32& outUpperY) const
{
    switch (m_horizontalAlignment)
    {
    case HorizontalAlignment::Left:
        outLowerX = GetXPos();
        outUpperX = GetXPos() + m_width;
        break;
    case HorizontalAlignment::Center:
        outLowerX = GetXPos() - (m_width / 2);
        outUpperX = GetXPos() + (m_width / 2);
        break;
    case HorizontalAlignment::Right:
        outLowerX = GetXPos() - m_width;
        outUpperX = GetXPos();
        break;
    }

    switch (m_verticalAlignment)
    {
    case VerticalAlignment::Top:
        outLowerY = GetYPos() - m_height;;
        outUpperY = GetYPos();
        break;
    case VerticalAlignment::Center:
        outLowerY = GetYPos() - (m_height / 2);
        outUpperY = GetYPos() + (m_height / 2);
        break;
    case VerticalAlignment::Bottom:
        outLowerY = GetYPos();
        outUpperY = GetYPos() + m_height;
        break;
    }
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

