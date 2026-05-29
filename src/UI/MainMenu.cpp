#include "MainMenu.h"

#include "Button.h"
#include "../Application.h"
#include "../Renderer.h"

MainMenu::MainMenu(Application& app) : Menu(app)
{
    const Uint32 centeredXPos = GetRenderer()->GetRenderWidth() / 2;

    Button* startGameButton = CreateButton();
    startGameButton->SetButtonText("Start Game");
    startGameButton->SetXPos(centeredXPos);
    startGameButton->SetYPos(500);
    startGameButton->BindFunction([this]() { m_application.StartGame(); });

    Button* exitButton = CreateButton();
    exitButton->SetButtonText("Exit");
    exitButton->SetXPos(centeredXPos);
    exitButton->SetYPos(700);
    exitButton->BindFunction([this]() {m_application.Stop(); });
}

MainMenu::~MainMenu()
{

}

void MainMenu::RenderTitle()
{
    if (!GetRenderer() || !IsVisible()) return;

    const Uint32 xPos = GetRenderer()->GetRenderWidth() / 2;
    const Uint32 yPos = 36;
    constexpr Uint32 fontSize = 48;
    GetRenderer()->DrawText("Tetris", xPos, yPos, Color(255, 255, 255), fontSize, HorizontalAlignment::Center, VerticalAlignment::Bottom);
}

void MainMenu::Init()
{
    Menu::Init();

}

void MainMenu::Update(const float deltaTime)
{
    Menu::Update(deltaTime);

    if (!IsVisible()) return;

    RenderTitle();
}
