#include "SettingsMenu.h"

#include "Button.h"
#include "TextBlock.h"
#include "MainMenu.h"
#include "../GameboyTheme.h"
#include "../Application.h"

SettingsMenu::SettingsMenu(Application& app) : Menu(app)
{
    TextBlock* settingsText = CreateTextBlock();
    settingsText->SetText("Settings");
    settingsText->SetFontSize(64);
    settingsText->SetHorizontalAlignment(HorizontalAlignment::Left);
    settingsText->SetVerticalAlignment(VerticalAlignment::Bottom);
    settingsText->SetHorizontalAnchor(HorizontalAlignment::Left);
    settingsText->SetVerticalAnchor(VerticalAlignment::Top);
    settingsText->SetXOffset(32);
    settingsText->SetYOffset(32);

    m_currentThemeText = CreateTextBlock();
    m_currentThemeText->SetText("Current Theme: " + m_application.GetTheme()->GetName());
    m_currentThemeText->SetFontSize(32);
    m_currentThemeText->SetHorizontalAlignment(HorizontalAlignment::Left);
    m_currentThemeText->SetVerticalAlignment(VerticalAlignment::Center);
    m_currentThemeText->SetHorizontalAnchor(HorizontalAlignment::Left);
    m_currentThemeText->SetVerticalAnchor(VerticalAlignment::Center);
    m_currentThemeText->SetXOffset(32);
    m_currentThemeText->SetYOffset(-108);

    Button* defaultThemeButton = CreateButton();
    defaultThemeButton->SetButtonText("Default Theme");
    defaultThemeButton->SetHorizontalAlignment(HorizontalAlignment::Left);
    defaultThemeButton->SetVerticalAlignment(VerticalAlignment::Center);
    defaultThemeButton->SetHorizontalAnchor(HorizontalAlignment::Left);
    defaultThemeButton->SetVerticalAnchor(VerticalAlignment::Center);
    defaultThemeButton->SetXOffset(32);
    defaultThemeButton->SetYOffset(-36);
    defaultThemeButton->BindFunction([this]() { m_application.SetTheme<Theme>(); });

    Button* gameboyThemeButton = CreateButton();
    gameboyThemeButton->SetButtonText("Gameboy Theme");
    gameboyThemeButton->SetHorizontalAlignment(HorizontalAlignment::Left);
    gameboyThemeButton->SetVerticalAlignment(VerticalAlignment::Center);
    gameboyThemeButton->SetHorizontalAnchor(HorizontalAlignment::Left);
    gameboyThemeButton->SetVerticalAnchor(VerticalAlignment::Center);
    gameboyThemeButton->SetXOffset(32);
    gameboyThemeButton->SetYOffset(36);
    gameboyThemeButton->BindFunction([this]() { m_application.SetTheme<GameboyTheme>(); });

    Button* backButton = CreateButton();
    backButton->SetButtonText("Back");
    backButton->SetHorizontalAlignment(HorizontalAlignment::Right);
    backButton->SetVerticalAlignment(VerticalAlignment::Top);
    backButton->SetHorizontalAnchor(HorizontalAlignment::Right);
    backButton->SetVerticalAnchor(VerticalAlignment::Bottom);
    backButton->SetXOffset(32);
    backButton->SetYOffset(32);
    backButton->BindFunction([this]() { m_application.OpenMenu<MainMenu>(); });
}

SettingsMenu::~SettingsMenu()
{

}

void SettingsMenu::Update(float deltaTime)
{
    Menu::Update(deltaTime);

    m_currentThemeText->SetText("Current Theme: " + m_application.GetTheme()->GetName());
}

