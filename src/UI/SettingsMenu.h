#pragma once

#include "Menu.h"

class SettingsMenu : public Menu
{
public:
    explicit SettingsMenu(Application& app);
    ~SettingsMenu() override;

protected:
    TextBlock* m_currentThemeText;

    void Update(float deltaTime) override;
};
