#include "Menu.h"

#include <iostream>

#include "../Application.h"
#include "Button.h"

Menu::Menu(Application& app) : m_application(app)
{

}

Menu::~Menu()
{

}

Button* Menu::CreateButton()
{
    Button* newButton = m_buttons.emplace_back(std::make_unique<Button>(*this)).get();
    newButton->SetWidth(m_defaultButtonWidth);
    newButton->SetHeight(m_defaultButtonHeight);
    return newButton;
}

void Menu::RenderButtons() const
{
    for (const std::unique_ptr<Button>& button : m_buttons)
    {
        GetRenderer()->DrawButton(button.get());
    }
}

void Menu::SelectNextButton()
{
    if (m_buttons.empty()) return;

    if (m_selectedButtonIndex >= m_buttons.size() - 1)
    {
        m_selectedButtonIndex = 0;
    }
    else m_selectedButtonIndex++;
}

void Menu::SelectPreviousButton()
{
    if (m_buttons.empty()) return;

    if (m_selectedButtonIndex == 0)
    {
        m_selectedButtonIndex = m_buttons.size() - 1;
    }
    else m_selectedButtonIndex--;
}

void Menu::Init()
{

}

void Menu::Update(const float deltaTime)
{
    if (!IsVisible()) return;

    RenderButtons();
}

void Menu::ReceiveUpInput()
{
    SelectPreviousButton();
}

void Menu::ReceiveDownInput()
{
    SelectNextButton();
}

void Menu::ReceiveLeftInput()
{
    if (m_buttons.empty()) return;
}

void Menu::ReceiveRightInput()
{
    if (m_buttons.empty()) return;
}

void Menu::ReceiveSelectInput()
{
    if (m_buttons.empty()) return;

    try
    {
        Button* button = m_buttons.at(m_selectedButtonIndex).get();
        if (!button) return;
        button->PressButton();
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "Menu::ReceiveSelectInput::Selected button out of range at index: " << m_selectedButtonIndex << std::endl;
    }
}

bool Menu::SetVisibility(const bool input)
{
    return m_isVisible = input;
}

bool Menu::IsVisible() const
{
    return m_isVisible;
}

Button* Menu::GetSelectedButton() const
{
    try
    {
        return m_buttons.at(m_selectedButtonIndex).get();
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
