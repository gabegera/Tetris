#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:
    explicit MainMenu(Application& app);
    ~MainMenu();

protected:
    void RenderTitle();

public:
    void Init() override;
    void Update(float deltaTime) override;

};
