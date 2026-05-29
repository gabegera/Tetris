#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:
    explicit MainMenu(Application& app);
    ~MainMenu() override;

protected:
    TextBlock* m_titleTextBlock = nullptr;

    Button* m_startGameButton = nullptr;

    Button* m_exitButton = nullptr;

};
