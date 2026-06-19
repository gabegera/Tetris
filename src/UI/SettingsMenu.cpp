#include "SettingsMenu.h"

#include "Button.h"
#include "TextBlock.h"
#include "MainMenu.h"
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
