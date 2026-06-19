#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:
    explicit MainMenu(Application& app);
    ~MainMenu() override;};
