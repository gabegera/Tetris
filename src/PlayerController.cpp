#include "PlayerController.h"

#include "Application.h"

PlayerController::PlayerController(Application& app) : m_app(app)
{
    m_exitAction = GetInputHandler()->CreateInputAction("IA_Exit");
    m_exitAction->AssignKey(SDLK_ESCAPE, InputType::Pressed);
    m_exitAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_START, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_exitAction->GetID(), [this]{ Exit(); });

    m_moveShapeRightAction = GetInputHandler()->CreateInputAction("IA_MoveShapeRight");
    m_moveShapeRightAction->AssignKey(SDLK_D, InputType::Pressed);
    m_moveShapeRightAction->AssignKey(SDLK_D, InputType::Held);
    m_moveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Pressed);
    m_moveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Held);
    m_moveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Pressed);
    m_moveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Held);
    m_moveShapeRightAction->SetHoldDuration(0.2f);
    m_moveShapeRightAction->SetRepeatDelay(0.04f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeRightAction->GetID(), [this]{ MoveShapeRight(); });

    m_moveShapeLeftAction = GetInputHandler()->CreateInputAction("IA_MoveShapeLeft");
    m_moveShapeLeftAction->AssignKey(SDLK_A, InputType::Pressed);
    m_moveShapeLeftAction->AssignKey(SDLK_A, InputType::Held);
    m_moveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Pressed);
    m_moveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Held);
    m_moveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Pressed);
    m_moveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Held);
    m_moveShapeLeftAction->SetHoldDuration(0.2f);
    m_moveShapeLeftAction->SetRepeatDelay(0.04f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeLeftAction->GetID(), [this]{ MoveShapeLeft(); });

    m_moveShapeDownAction = GetInputHandler()->CreateInputAction("IA_MoveShapeDown");
    m_moveShapeDownAction->AssignKey(SDLK_S, InputType::Pressed);
    m_moveShapeDownAction->AssignKey(SDLK_S, InputType::Held);
    m_moveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    m_moveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Held);
    m_moveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    m_moveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Held);
    m_moveShapeDownAction->SetHoldDuration(0.2f);
    m_moveShapeDownAction->SetRepeatDelay(0.04f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeDownAction->GetID(), [this]{ MoveShapeDown(); });

    m_dropShapeAction = GetInputHandler()->CreateInputAction("IA_DropShape");
    m_dropShapeAction->AssignKey(SDLK_W, InputType::Pressed);
    m_dropShapeAction->AssignKey(SDLK_UP, InputType::Pressed);
    m_dropShapeAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_UP, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_dropShapeAction->GetID(), [this]{ DropShape(); });

    m_rotateClockwiseAction = GetInputHandler()->CreateInputAction("IA_RotateClockwise");
    m_rotateClockwiseAction->AssignKey(SDLK_X, InputType::Pressed);
    m_rotateClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_rotateClockwiseAction->GetID(), [this]{ RotateClockwise(); });

    m_rotateCounterClockwiseAction = GetInputHandler()->CreateInputAction("IA_RotateCounterClockwise");
    m_rotateCounterClockwiseAction->AssignKey(SDLK_Z, InputType::Pressed);
    m_rotateCounterClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_EAST, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_rotateCounterClockwiseAction->GetID(), [this]{ RotateCounterClockwise(); });

    m_holdShapeAction = GetInputHandler()->CreateInputAction("IA_HoldShape");
    m_holdShapeAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    m_holdShapeAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, InputType::Pressed);
    m_holdShapeAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_holdShapeAction->GetID(), [this]{ HoldShape(); });

    m_selectNextUIElementAction = GetInputHandler()->CreateInputAction("IA_SelectNextUIElement");
    m_selectNextUIElementAction->AssignKey(SDLK_S, InputType::Pressed);
    m_selectNextUIElementAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    m_selectNextUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_selectNextUIElementAction->GetID(), [this]{ SelectNextUIElement(); });

    m_selectPreviousUIElementAction = GetInputHandler()->CreateInputAction("IA_SelectPreviousUIElement");
    m_selectPreviousUIElementAction->AssignKey(SDLK_W, InputType::Pressed);
    m_selectPreviousUIElementAction->AssignKey(SDLK_UP, InputType::Pressed);
    m_selectPreviousUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_selectPreviousUIElementAction->GetID(), [this]{ SelectPreviousUIElement(); });

    m_triggerUIElementAction = GetInputHandler()->CreateInputAction("IA_TriggerUIElement");
    m_triggerUIElementAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    m_triggerUIElementAction->AssignKey(SDLK_RETURN, InputType::Pressed);
    m_triggerUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_triggerUIElementAction->GetID(), [this]{ TriggerUIElement(); });

    m_clickUIElementAction = GetInputHandler()->CreateInputAction("IA_ClickUIElement");
    m_clickUIElementAction->AssignMouseButton(SDL_BUTTON_LEFT, InputType::Released);
    GetInputHandler()->BindFunctionToAction(m_clickUIElementAction->GetID(), [this]{ ClickUIElement(); });
}

PlayerController::~PlayerController()
{

}

void PlayerController::Exit() const
{
    if (m_app.IsGameRunning())
    {
        m_app.StopGame();
    }
}

void PlayerController::MoveShapeRight() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->MoveShapeRight();
    }
}

void PlayerController::MoveShapeLeft() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->MoveShapeLeft();
    }
}

void PlayerController::MoveShapeDown() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->MoveShapeDown();
    }
}

void PlayerController::DropShape() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->DropShape();
    }
}

void PlayerController::RotateClockwise() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->RotateShapeClockwise();
    }
}

void PlayerController::RotateCounterClockwise() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->RotateShapeCounterClockwise();
    }
}

void PlayerController::HoldShape() const
{
    if (m_app.IsGameRunning())
    {
        m_app.GetGame()->GetBlockManager()->HoldShape();
    }
}

void PlayerController::SelectNextUIElement() const
{
    if (!m_app.IsGameRunning() && m_app.GetActiveMenu())
    {
        m_app.GetActiveMenu()->SelectNextElement();
    }
}

void PlayerController::SelectPreviousUIElement() const
{
    if (!m_app.IsGameRunning() && m_app.GetActiveMenu())
    {
        m_app.GetActiveMenu()->SelectPreviousElement();
    }
}

void PlayerController::TriggerUIElement() const
{
    if (!m_app.IsGameRunning() && m_app.GetActiveMenu())
    {
        m_app.GetActiveMenu()->TriggerSelectedElement();
    }
}

void PlayerController::ClickUIElement() const
{
    if (!m_app.IsGameRunning() && m_app.GetActiveMenu())
    {
        m_app.GetActiveMenu()->TriggerElementBelowCursor();
    }
}

InputHandler* PlayerController::GetInputHandler() const
{
    return m_app.GetInputHandler();
}
