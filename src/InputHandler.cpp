#include "InputHandler.h"

#include <iostream>
#include <ranges>

#include "Application.h"

InputHandler::InputHandler(Application& app) : m_app(app)
{
    InputAction* ExitAction = CreateInputAction("IA_Exit");
    ExitAction->AssignKey(SDLK_ESCAPE, InputType::Pressed);
    ExitAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_START, InputType::Pressed);
    BindFunctionToAction(ExitAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.StopGame();
        });

    InputAction* MoveShapeRightAction = CreateInputAction("IA_MoveShapeRight");
    MoveShapeRightAction->AssignKey(SDLK_D, InputType::Pressed);
    MoveShapeRightAction->AssignKey(SDLK_D, InputType::Held);
    MoveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Pressed);
    MoveShapeRightAction->AssignKey(SDLK_RIGHT, InputType::Held);
    MoveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Pressed);
    MoveShapeRightAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Held);
    MoveShapeRightAction->SetRepeatDelay(0.1f);
    BindFunctionToAction(MoveShapeRightAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeRight();
        });

    InputAction* MoveShapeLeftAction = CreateInputAction("IA_MoveShapeLeft");
    MoveShapeLeftAction->AssignKey(SDLK_A, InputType::Pressed);
    MoveShapeLeftAction->AssignKey(SDLK_A, InputType::Held);
    MoveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Pressed);
    MoveShapeLeftAction->AssignKey(SDLK_LEFT, InputType::Held);
    MoveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Pressed);
    MoveShapeLeftAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Held);
    MoveShapeLeftAction->SetRepeatDelay(0.1f);
    BindFunctionToAction(MoveShapeLeftAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeLeft();
        });

    InputAction* MoveShapeDownAction = CreateInputAction("IA_MoveShapeDown");
    MoveShapeDownAction->AssignKey(SDLK_S, InputType::Pressed);
    MoveShapeDownAction->AssignKey(SDLK_S, InputType::Held);
    MoveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    MoveShapeDownAction->AssignKey(SDLK_DOWN, InputType::Held);
    MoveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    MoveShapeDownAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Held);
    MoveShapeDownAction->SetRepeatDelay(0.1f);
    BindFunctionToAction(MoveShapeDownAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->MoveShapeDown();
        });

    InputAction* DropShapeAction = CreateInputAction("IA_DropShape");
    DropShapeAction->AssignKey(SDLK_W, InputType::Pressed);
    DropShapeAction->AssignKey(SDLK_UP, InputType::Pressed);
    DropShapeAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_UP, InputType::Pressed);
    BindFunctionToAction(DropShapeAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->DropShape();
        });

    InputAction* RotateClockwiseAction = CreateInputAction("IA_RotateClockwise");
    RotateClockwiseAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    RotateClockwiseAction->AssignKey(SDLK_E, InputType::Pressed);
    RotateClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    BindFunctionToAction(RotateClockwiseAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->RotateShapeClockwise();
        });

    InputAction* RotateCounterClockwiseAction = CreateInputAction("IA_RotateCounterClockwise");
    RotateCounterClockwiseAction->AssignKey(SDLK_LALT, InputType::Pressed);
    RotateCounterClockwiseAction->AssignKey(SDLK_Q, InputType::Pressed);
    RotateCounterClockwiseAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_EAST, InputType::Pressed);
    BindFunctionToAction(RotateCounterClockwiseAction->GetName(),
        [this]()
        {
            if (!m_app.IsGameRunning()) return;
            m_app.GetGame()->GetBlockManager()->RotateShapeCounterClockwise();
        });

    InputAction* SelectNextUIElementAction = CreateInputAction("IA_SelectNextUIElement");
    SelectNextUIElementAction->AssignKey(SDLK_S, InputType::Pressed);
    SelectNextUIElementAction->AssignKey(SDLK_DOWN, InputType::Pressed);
    SelectNextUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    BindFunctionToAction(SelectNextUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->SelectNextElement();
        });

    InputAction* SelectPreviousUIElementAction = CreateInputAction("IA_SelectPreviousUIElement");
    SelectPreviousUIElementAction->AssignKey(SDLK_W, InputType::Pressed);
    SelectPreviousUIElementAction->AssignKey(SDLK_UP, InputType::Pressed);
    SelectPreviousUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Pressed);
    BindFunctionToAction(SelectPreviousUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->SelectPreviousElement();
        });

    InputAction* TriggerUIElementAction = CreateInputAction("IA_TriggerUIElement");
    TriggerUIElementAction->AssignKey(SDLK_SPACE, InputType::Pressed);
    TriggerUIElementAction->AssignKey(SDLK_RETURN, InputType::Pressed);
    TriggerUIElementAction->AssignGamepadButton(SDL_GAMEPAD_BUTTON_SOUTH, InputType::Pressed);
    BindFunctionToAction(TriggerUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->TriggerSelectedElement();
        });

    InputAction* ClickUIElementAction = CreateInputAction("IA_ClickUIElement");
    ClickUIElementAction->AssignMouseButton(SDL_BUTTON_LEFT, InputType::Released);
    BindFunctionToAction(ClickUIElementAction->GetName(),
        [this]()
        {
            if (!m_app.GetActiveMenu() || m_app.IsGameRunning()) return;
            m_app.GetActiveMenu()->TriggerElementBelowCursor();
        });
}

InputHandler::~InputHandler()
{

}

InputAction* InputHandler::CreateInputAction(const std::string& inName)
{
    if (GetInputAction(inName))
    {
        std::cerr << "InputHandler::CreateInputAction::Action with the name " << inName << " already exists." << std::endl;
        return nullptr;
    }

    return &m_actions.emplace_back(InputAction(inName));
}

void InputHandler::TriggerInputAction(const std::string& actionName) const
{
    try
    {
        m_actionBindings.at(actionName)();
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "InputHandler::TriggerInputAction::Unable to find action: " << actionName << " in bindings" << std::endl;
    }
}

void InputHandler::TriggerHeldInputs()
{
    // Key Inputs
    for (auto [key, heldDuration] : m_heldKeys)
    {
        for (InputAction& action : m_actions)
        {
            if (!action.IsInputTypeAssignedToKey(key, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(m_app.GetDeltaTime());

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                TriggerInputAction(action.GetName());
                action.ClearRepeatDelayTracker();
            }
        }
    }

    // Mouse Button Inputs
    for (auto [button, heldDuration] : m_heldMouseButtons)
    {
        if (heldDuration <= 0) continue;

        for (InputAction action : m_actions)
        {
            if (!action.IsInputTypeAssignedToMouseButton(button, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(m_app.GetDeltaTime());

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                TriggerInputAction(action.GetName());
                action.ClearRepeatDelayTracker();
            }
        }
    }

    // Gamepad Button Inputs
    for (auto [button, heldDuration] : m_heldGamepadButtons)
    {
        if (heldDuration <= 0) continue;

        for (InputAction action : m_actions)
        {
            if (!action.IsInputTypeAssignedToGamepadButton(button, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(m_app.GetDeltaTime());

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                TriggerInputAction(action.GetName());
                action.ClearRepeatDelayTracker();
            }
        }
    }

}

void InputHandler::Update(const float deltaTime)
{
    for (auto [key, heldDuration] : m_heldKeys)
    {
        if (IsKeyBeingHeld(key)) m_heldKeys[key] = heldDuration + deltaTime;
    }

    for (auto [button, heldDuration] : m_heldMouseButtons)
    {
        if (IsKeyBeingHeld(button)) m_heldMouseButtons[button] = heldDuration + deltaTime;
    }

    for (auto [button, heldDuration] : m_heldGamepadButtons)
    {
        if (IsKeyBeingHeld(button)) m_heldGamepadButtons[button] = heldDuration + deltaTime;
    }

    TriggerHeldInputs();
}

void InputHandler::KeyUp(const SDL_Keycode key)
{
    const auto iterator = m_heldKeys.find(key);
    if (iterator == m_heldKeys.end()) return;

    m_heldKeys.erase(key);

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToKey(key, InputType::Released))
        {
            TriggerInputAction(action.GetName());
        }

        action.ClearRepeatDelayTracker();
    }
}

void InputHandler::KeyDown(const SDL_Keycode key)
{
    if (IsKeyBeingHeld(key)) return;

    if (!m_heldKeys.contains(key))
    {
        m_heldKeys.try_emplace(key, 0.0f);
    }

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToKey(key, InputType::Pressed))
        {
            TriggerInputAction(action.GetName());
        }
    }
}

void InputHandler::MouseButtonUp(const Uint8 button)
{
    const auto iterator = m_heldMouseButtons.find(button);
    if (iterator == m_heldMouseButtons.end()) return;

    m_heldMouseButtons.erase(button);

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToMouseButton(button, InputType::Released))
        {
            TriggerInputAction(action.GetName());
        }
    }
}

void InputHandler::MouseButtonDown(const Uint8 button)
{
    if (IsMouseButtonBeingHeld(button)) return;

    if (!m_heldMouseButtons.contains(button))
    {
        m_heldMouseButtons.try_emplace(button, 0.0f);
    }

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToMouseButton(button, InputType::Pressed))
        {
            TriggerInputAction(action.GetName());
        }
    }
}

void InputHandler::MouseMoved()
{

}

void InputHandler::GamepadButtonUp(const Uint8 button)
{
    const auto iterator = m_heldGamepadButtons.find(button);
    if (iterator == m_heldGamepadButtons.end()) return;

    m_heldGamepadButtons.erase(button);

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToGamepadButton(button, InputType::Released))
        {
            TriggerInputAction(action.GetName());
        }
    }
}

void InputHandler::GamepadButtonDown(const Uint8 button)
{
    if (IsGamepadButtonBeingHeld(button)) return;

    if (!m_heldGamepadButtons.contains(button))
    {
        m_heldGamepadButtons.try_emplace(button, 0.0f);
    }

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToGamepadButton(button, InputType::Pressed))
        {
            TriggerInputAction(action.GetName());
        }
    }
}

bool InputHandler::IsKeyBeingHeld(const SDL_Keycode key) const
{
    return m_heldKeys.contains(key);
}

bool InputHandler::IsMouseButtonBeingHeld(const Uint8 button) const
{
    return m_heldMouseButtons.contains(button);
}

bool InputHandler::IsGamepadButtonBeingHeld(const Uint8 button) const
{
    return m_heldGamepadButtons.contains(button);
}

void InputHandler::BindFunctionToAction(const std::string& actionName, const std::function<void()>& function)
{
    const InputAction* foundAction = GetInputAction(actionName);
    if (!foundAction)
    {
        std::cerr << "InputHandler::BindFunctionToAction::Unable to find Input Action with the name " << actionName << std::endl;
        return;
    }

    m_actionBindings.try_emplace(actionName, function);
}

InputAction* InputHandler::GetInputAction(const std::string& name)
{
    for (int i = 0; i < m_actions.size(); i++)
    {
        if (m_actions[i].GetName() == name)
        {
            return &m_actions[i];
        }
    }

    return nullptr;
}
