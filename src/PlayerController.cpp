#include "PlayerController.h"

#include "Application.h"

PlayerController::PlayerController(Application& app) : m_app(app)
{
    m_exitAction = GetInputHandler()->CreateInputAction("IA_Exit");
    m_exitAction->AssignKey(SDLK_ESCAPE, InputType::Pressed);
    m_exitAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_START, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_exitAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.StopGame();
        });

    m_moveShapeRightAction = GetInputHandler()->CreateInputAction("IA_MoveShapeRight");
    m_moveShapeRightAction->AssignKey(SDLK_D, InputType::Pressed);
    m_moveShapeRightAction->AssignKey(SDLK_D, InputType::Held);
    m_moveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Pressed);
    m_moveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Held);
    m_moveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Pressed);
    m_moveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Held);
    m_moveShapeRightAction->SetRepeatDelay(0.1f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeRightAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeRight();
        });

    m_moveShapeLeftAction = GetInputHandler()->CreateInputAction("IA_MoveShapeLeft");
    m_moveShapeLeftAction->AssignKey(SDLK_A, InputType::Pressed);
    m_moveShapeLeftAction->AssignKey(SDLK_A, InputType::Held);
    m_moveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Pressed);
    m_moveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Held);
    m_moveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Pressed);
    m_moveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Held);
    m_moveShapeLeftAction->SetRepeatDelay(0.1f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeLeftAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeLeft();
        });

    m_moveShapeDownAction = GetInputHandler()->CreateInputAction("IA_MoveShapeDown");
    m_moveShapeDownAction->AssignKey(SDLK_S, InputType::Pressed);
    m_moveShapeDownAction->AssignKey(SDLK_S, InputType::Held);
    m_moveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    m_moveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Held);
    m_moveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    m_moveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Held);
    m_moveShapeDownAction->SetRepeatDelay(0.1f);
    GetInputHandler()->BindFunctionToAction(m_moveShapeDownAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeDown();
        });

    m_dropShapeAction = GetInputHandler()->CreateInputAction("IA_DropShape");
    m_dropShapeAction->AssignKey(SDLK_W, InputType::Pressed);
    m_dropShapeAction->AssignKey(SDLK_UP, InputType::Pressed);
    m_dropShapeAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_UP, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_dropShapeAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->DropShape();
        });

    m_rotateClockwiseAction = GetInputHandler()->CreateInputAction("IA_RotateClockwise");
    m_rotateClockwiseAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    m_rotateClockwiseAction->AssignKey(SDLK_E, InputType::Pressed);
    m_rotateClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_rotateClockwiseAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->RotateShapeClockwise();
        });

    m_rotateCounterClockwiseAction = GetInputHandler()->CreateInputAction("IA_RotateCounterClockwise");
    m_rotateCounterClockwiseAction->AssignKey(SDLK_LALT, InputType::Pressed);
    m_rotateCounterClockwiseAction->AssignKey(SDLK_Q, InputType::Pressed);
    m_rotateCounterClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_EAST, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_rotateCounterClockwiseAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->RotateShapeCounterClockwise();
        });

    m_selectNextUIElementAction = GetInputHandler()->CreateInputAction("IA_SelectNextUIElement");
    m_selectNextUIElementAction->AssignKey(SDLK_S, InputType::Pressed);
    m_selectNextUIElementAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    m_selectNextUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_selectNextUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->SelectNextElement();
        });

    m_selectPreviousUIElementAction = GetInputHandler()->CreateInputAction("IA_SelectPreviousUIElement");
    m_selectPreviousUIElementAction->AssignKey(SDLK_W, InputType::Pressed);
    m_selectPreviousUIElementAction->AssignKey(SDLK_UP, InputType::Pressed);
    m_selectPreviousUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_selectPreviousUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->SelectPreviousElement();
        });

    m_triggerUIElementAction = GetInputHandler()->CreateInputAction("IA_TriggerUIElement");
    m_triggerUIElementAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    m_triggerUIElementAction->AssignKey(SDLK_RETURN, InputType::Pressed);
    m_triggerUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    GetInputHandler()->BindFunctionToAction(m_triggerUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->TriggerSelectedElement();
        });

    m_clickUIElementAction = GetInputHandler()->CreateInputAction("IA_ClickUIElement");
    m_clickUIElementAction->AssignMouseButton(SDL_BUTTON_LEFT, InputType::Released);
    GetInputHandler()->BindFunctionToAction(m_clickUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->TriggerElementBelowCursor();
        });
}

PlayerController::~PlayerController()
{

}

InputHandler* PlayerController::GetInputHandler() const
{
    return m_app.GetInputHandler();
}