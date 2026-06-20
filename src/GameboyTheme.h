#pragma once

#include "Theme.h"

class GameboyTheme : public Theme
{
public:
    GameboyTheme();
    ~GameboyTheme() override;

    void Init() override;
};
