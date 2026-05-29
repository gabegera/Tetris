#include "UIElement.h"

#include "../Renderer.h"

UIElement::UIElement(Menu& menu, const bool isSelectable)
    : m_menu(menu), m_isSelectable(isSelectable)
{

}

UIElement::~UIElement()
{

}

bool UIElement::SetVisibility(const bool input)
{
    return m_isVisible = input;
}

void UIElement::SetXOffset(const Uint32 inXPos)
{
    m_xOffset = inXPos;
}

void UIElement::SetYOffset(const Uint32 inYPos)
{
    m_yOffset = inYPos;
}

void UIElement::SetHorizontalAlignment(const HorizontalAlignment inAlignment)
{
    m_horizontalAlignment = inAlignment;
}

void UIElement::SetVerticalAlignment(const VerticalAlignment inAlignment)
{
    m_verticalAlignment = inAlignment;
}

void UIElement::SetHorizontalAnchor(HorizontalAlignment inAnchor)
{
    m_horizontalAnchor = inAnchor;
}

void UIElement::SetVerticalAnchor(VerticalAlignment inAnchor)
{
    m_verticalAnchor = inAnchor;
}

void UIElement::Init()
{

}

void UIElement::Update(float deltaTime)
{
    if (!IsVisible()) return;

    Render();
}

Renderer* UIElement::GetRenderer() const
{
    return m_menu.GetRenderer();
}

Menu* UIElement::GetOwningMenu() const
{
    return &m_menu;
}

void UIElement::TriggerElement()
{
    if (!m_isSelectable) return;
}

bool UIElement::IsVisible() const
{
    return m_isVisible;
}

bool UIElement::IsSelectable() const
{
    return m_isSelectable;
}

Uint32 UIElement::GetXPos() const
{
    switch (m_horizontalAnchor)
    {
        case HorizontalAlignment::Center:
            return GetRenderer()->GetRenderWidth() / 2 + m_xOffset;
        case HorizontalAlignment::Right:
            return GetRenderer()->GetRenderWidth() - m_xOffset;
        default: // Left
            return m_xOffset;
    }
}

Uint32 UIElement::GetYPos() const
{
    switch (m_verticalAnchor)
    {
        case VerticalAlignment::Center:
            return GetRenderer()->GetRenderHeight() / 2 + m_yOffset;
        case VerticalAlignment::Bottom:
            return GetRenderer()->GetRenderHeight() - m_yOffset;
        default: // Top
            return m_yOffset;
    }
}

HorizontalAlignment UIElement::GetHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

VerticalAlignment UIElement::GetVerticalAlignment() const
{
    return m_verticalAlignment;
}

HorizontalAlignment UIElement::GetHorizontalAnchor() const
{
    return m_horizontalAnchor;
}

VerticalAlignment UIElement::GetVerticalAnchor() const
{
    return m_verticalAnchor;
}
