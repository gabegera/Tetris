#include "Menu.h"

#include <iostream>

#include "Button.h"
#include "TextBlock.h"
#include "../Application.h"

Menu::Menu(Application& app) : m_application(app)
{

}

Menu::~Menu()
{

}

TextBlock* Menu::CreateTextBlock()
{
    TextBlock* newTextBlock = dynamic_cast<TextBlock*>(m_children.emplace_back(std::make_unique<TextBlock>(*this)).get());

    if (!newTextBlock)
    {
        std::cerr << "Menu::CreateTextBlock::Failed to create Text Block." << std::endl;
        m_application.Stop();
        return nullptr;
    }

    return newTextBlock;
}

Button* Menu::CreateButton()
{
    Button* newButton = dynamic_cast<Button*>(m_children.emplace_back(std::make_unique<Button>(*this)).get());

    if (!newButton)
    {
        std::cerr << "Menu::CreateButton::Failed to create Button." << std::endl;
        m_application.Stop();
        return nullptr;
    }

    newButton->SetWidth(m_defaultButtonWidth);
    newButton->SetHeight(m_defaultButtonHeight);
    return newButton;
}

void Menu::SelectNextElement()
{
    if (m_children.empty()) return;

    if (!m_isElementSelected)
    {
        m_isElementSelected = true;
        return;
    }

    m_selectedUIElementIndex++;

    for (int i = 0; i < m_children.size(); i++)
    {
        if (m_selectedUIElementIndex >= m_children.size()) m_selectedUIElementIndex = 0;

        if (m_children[m_selectedUIElementIndex].get()->IsSelectable())
        {
            m_isElementSelected = true;
            return;
        }

        m_selectedUIElementIndex++;
    }

    m_selectedUIElementIndex = 0;
    m_isElementSelected = false;
}

void Menu::SelectPreviousElement()
{
    if (m_children.empty()) return;

    if (!m_isElementSelected)
    {
        m_isElementSelected = true;
        return;
    }

    m_selectedUIElementIndex--;

    for (int i = 0; i < m_children.size(); i++)
    {
        if (m_selectedUIElementIndex >= m_children.size()) m_selectedUIElementIndex = m_children.size() - 1;

        if (m_children[m_selectedUIElementIndex].get()->IsSelectable())
        {
            m_isElementSelected = true;
            return;
        }

        m_selectedUIElementIndex--;
    }

    m_selectedUIElementIndex = 0;
    m_isElementSelected = false;
}

void Menu::TriggerSelectedElement() const
{
    if (m_children.empty()) return;

    try
    {
        UIElement* selectableElement = m_children.at(m_selectedUIElementIndex).get();
        if (!selectableElement) return;
        selectableElement->TriggerElement();
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "Menu::ReceiveSelectInput::Selected element out of range at index: " << m_selectedUIElementIndex << std::endl;
    }
}

void Menu::TriggerElementBelowCursor()
{
    UIElement* selectedElement = m_children[m_selectedUIElementIndex].get();
    if (!selectedElement)
    {
        std::cerr << "Menu::ReceiveLeftMouseButtonUpInput::Selected element is invalid at index " << m_selectedUIElementIndex << std::endl;
        return;
    }

    float mouseX;
    float mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (selectedElement->IsPositionWithinBounds(mouseX, mouseY))
    {
        selectedElement->TriggerElement();
    }
}

void Menu::Init()
{
    for (int i = 0; i < m_children.size(); i++)
    {
        try
        {
            if (m_children.at(i)->IsSelectable())
            {
                m_selectedUIElementIndex = i;
                break;
            }
        } catch (const std::out_of_range& error) {}
    }
}

void Menu::Update(const float deltaTime)
{
    if (!IsVisible()) return;

    CheckMousePosition();

    for (const auto& child : m_children)
    {
        child.get()->Update(deltaTime);
    }
}

void Menu::CheckMousePosition()
{
    float mouseX;
    float mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    for (int i = 0; i < m_children.size(); i++)
    {
        if (!m_children[i])
        {
            std::cerr << "Menu::ReceiveMouseInput::Passed over nullptr at index " << i << std::endl;
        }

        const UIElement* element = m_children[i].get();
        if (!element->IsSelectable()) continue;

        if (element->IsPositionWithinBounds(mouseX, mouseY))
        {
            m_selectedUIElementIndex = i;
            m_isElementSelected = true;
            return;
        }
    }

    // m_isElementSelected = false;
    // m_selectedUIElementIndex = 0;
}

bool Menu::SetVisibility(const bool input)
{
    return m_isVisible = input;
}

bool Menu::IsVisible() const
{
    return m_isVisible;
}

const UIElement* Menu::GetSelectedElement() const
{
    if (!m_isElementSelected) return nullptr;

    try
    {
        const UIElement* element = m_children.at(m_selectedUIElementIndex).get();
        return element->IsSelectable() ? element : nullptr;
    }
    catch (const std::out_of_range& error)
    {
        return nullptr;
    }
}

Application* Menu::GetApplication() const
{
    return &m_application;
}

Renderer* Menu::GetRenderer() const
{
    return m_application.GetRenderer();
}
