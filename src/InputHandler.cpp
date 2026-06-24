#include "InputHandler.h"

#include <iostream>
#include <ranges>

#include "Application.h"

InputHandler::InputHandler(Application& app) : m_app(app)
{

}

InputHandler::~InputHandler()
{

}

void InputHandler::AddBoundActionToQueue(const std::string& actionName)
{
    try
    {
        m_actionQueue.emplace_back(m_actionBindings.at(actionName));
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "InputHandler::AddBoundActionToQueue::Unable to find bound action: " << actionName << " in bindings" << std::endl;
    }
}

void InputHandler::ExecuteQueuedActions()
{
    for (int i = 0; i < m_actionQueue.size(); i++)
    {
        m_actionQueue.back()();
        m_actionQueue.pop_back();
    }
}

void InputHandler::UpdateHeldInputs(const float deltaTime)
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


    // Key Inputs
    for (auto [key, heldDuration] : m_heldKeys)
    {
        for (InputAction& action : m_actions)
        {
            if (!action.IsInputTypeAssignedToKey(key, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetName());
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

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetName());
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

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetName());
                action.ClearRepeatDelayTracker();
            }
        }
    }

}

void InputHandler::Update(const float deltaTime)
{
    UpdateHeldInputs(deltaTime);

    ExecuteQueuedActions();
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

void InputHandler::KeyUp(const SDL_Keycode key)
{
    const auto iterator = m_heldKeys.find(key);
    if (iterator == m_heldKeys.end()) return;

    m_heldKeys.erase(key);

    for (InputAction action : m_actions)
    {
        if (action.IsInputTypeAssignedToKey(key, InputType::Released))
        {
            AddBoundActionToQueue(action.GetName());
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
            AddBoundActionToQueue(action.GetName());
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
            AddBoundActionToQueue(action.GetName());
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
            AddBoundActionToQueue(action.GetName());
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
            AddBoundActionToQueue(action.GetName());
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
            AddBoundActionToQueue(action.GetName());
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

bool InputHandler::IsInputActionBeingHeld(const std::string& actionName)
{
    const InputAction* action = GetInputAction(actionName);
    if (!action)
    {
        std::cerr << "InputHandler::IsInputActionBeingHeld::Failed to find Input action with the name " << actionName << std::endl;
        return false;
    }

    for (const auto& key : action->GetAssignedKeys() | std::views::keys)
    {
        if (m_heldKeys.contains(key)) return true;
    }

    for (const auto& button : action->GetAssignedMouseButtons() | std::views::keys)
    {
        if (m_heldKeys.contains(button)) return true;
    }

    for (const auto& button : action->GetAssignedGamepadButtons() | std::views::keys)
    {
        if (m_heldKeys.contains(button)) return true;
    }

    return false;
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
