#include "MainMenu.h"

#include "Button.h"
#include "SettingsMenu.h"
#include "TextBlock.h"
#include "../Application.h"

MainMenu::MainMenu(Application& app) : Menu(app)
{
    TextBlock* m_titleTextBlock = CreateTextBlock();
    m_titleTextBlock->SetText("Tetris");
    m_titleTextBlock->SetFontSize(64);
    m_titleTextBlock->SetHorizontalAlignment(HorizontalAlignment::Center);
    m_titleTextBlock->SetVerticalAlignment(VerticalAlignment::Bottom);
    m_titleTextBlock->SetHorizontalAnchor(HorizontalAlignment::Center);
    m_titleTextBlock->SetVerticalAnchor(VerticalAlignment::Top);
    m_titleTextBlock->SetXOffset(0);
    m_titleTextBlock->SetYOffset(25);

    Button* m_settingsButton = CreateButton();
    m_settingsButton->SetButtonText("Start Game");
    m_settingsButton->SetHorizontalAlignment(HorizontalAlignment::Center);
    m_settingsButton->SetVerticalAlignment(VerticalAlignment::Center);
    m_settingsButton->SetHorizontalAnchor(HorizontalAlignment::Center);
    m_settingsButton->SetVerticalAnchor(VerticalAlignment::Center);
    m_settingsButton->SetXOffset(0);
    m_settingsButton->SetYOffset(-36);
    m_settingsButton->BindFunction([this]() { m_application.StartGame(); });

    Button* m_startGameButton = CreateButton();
    m_startGameButton->SetButtonText("Settings");
    m_startGameButton->SetHorizontalAlignment(HorizontalAlignment::Center);
    m_startGameButton->SetVerticalAlignment(VerticalAlignment::Center);
    m_startGameButton->SetHorizontalAnchor(HorizontalAlignment::Center);
    m_startGameButton->SetVerticalAnchor(VerticalAlignment::Center);
    m_startGameButton->SetXOffset(0);
    m_startGameButton->SetYOffset(36);
    m_startGameButton->BindFunction([this]() { m_application.OpenMenu<SettingsMenu>(); });

    Button* m_exitButton = CreateButton();
    m_exitButton->SetButtonText("Exit");
    m_exitButton->SetHorizontalAlignment(HorizontalAlignment::Center);
    m_exitButton->SetVerticalAlignment(VerticalAlignment::Center);
    m_exitButton->SetHorizontalAnchor(HorizontalAlignment::Center);
    m_exitButton->SetVerticalAnchor(VerticalAlignment::Center);
    m_exitButton->SetXOffset(0);
    m_exitButton->SetYOffset(108);
    m_exitButton->BindFunction([this]() {m_application.Stop(); });
}

MainMenu::~MainMenu()
{

}
