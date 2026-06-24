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

void InputHandler::AddBoundActionToQueue(const Uint16 actionID)
{
    m_actionQueue.emplace_back(actionID);
}

void InputHandler::ExecuteQueuedActions()
{
    for (int i = 0; i < m_actionQueue.size(); i++)
    {
        try
        {
            m_actionBindings.at(m_actionQueue.back())();
        }
        catch (const std::out_of_range& error)
        {
            std::cerr << "InputHandler::ExecuteQueuedActions::Unable to find action binding with the ID: " << m_actionQueue.back() << std::endl;
        }
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
        for (InputAction& action : m_actions | std::views::values)
        {
            if (!action.IsInputTypeAssignedToKey(key, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetID());
                action.ClearRepeatDelayTracker();
            }
        }
    }

    // Mouse Button Inputs
    for (auto [button, heldDuration] : m_heldMouseButtons)
    {
        if (heldDuration <= 0) continue;

        for (InputAction& action : m_actions | std::views::values)
        {
            if (!action.IsInputTypeAssignedToMouseButton(button, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetID());
                action.ClearRepeatDelayTracker();
            }
        }
    }

    // Gamepad Button Inputs
    for (auto [button, heldDuration] : m_heldGamepadButtons)
    {
        if (heldDuration <= 0) continue;

        for (InputAction& action : m_actions | std::views::values)
        {
            if (!action.IsInputTypeAssignedToGamepadButton(button, InputType::Held)) continue;

            action.AddToRepeatDelayTracker(deltaTime);

            if (heldDuration >= action.GetHoldDuration() && action.WasRepeatDelayPassed())
            {
                AddBoundActionToQueue(action.GetID());
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
    if (GetInputActionByName(inName))
    {
        std::cerr << "InputHandler::CreateInputAction::Action with the name " << inName << " already exists." << std::endl;
        return nullptr;
    }

    InputAction newAction(inName);
    auto [iterator, wasSuccessful] = m_actions.try_emplace(newAction.GetID(), newAction);
    if (!wasSuccessful)
    {
        std::cerr << "InputHandler::CreateInputAction::Failed to create action with the name " << inName << std::endl;
        return nullptr;
    }

    return &iterator->second;
}

void InputHandler::KeyUp(const SDL_Keycode key)
{
    const auto iterator = m_heldKeys.find(key);
    if (iterator == m_heldKeys.end()) return;

    m_heldKeys.erase(key);

    for (InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToKey(key, InputType::Released))
        {
            AddBoundActionToQueue(action.GetID());
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

    for (const InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToKey(key, InputType::Pressed))
        {
            AddBoundActionToQueue(action.GetID());
        }
    }
}

void InputHandler::MouseButtonUp(const Uint8 button)
{
    const auto iterator = m_heldMouseButtons.find(button);
    if (iterator == m_heldMouseButtons.end()) return;

    m_heldMouseButtons.erase(button);

    for (InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToMouseButton(button, InputType::Released))
        {
            AddBoundActionToQueue(action.GetID());
        }

        action.ClearRepeatDelayTracker();
    }
}

void InputHandler::MouseButtonDown(const Uint8 button)
{
    if (IsMouseButtonBeingHeld(button)) return;

    if (!m_heldMouseButtons.contains(button))
    {
        m_heldMouseButtons.try_emplace(button, 0.0f);
    }

    for (const InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToMouseButton(button, InputType::Pressed))
        {
            AddBoundActionToQueue(action.GetID());
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

    for (InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToGamepadButton(button, InputType::Released))
        {
            AddBoundActionToQueue(action.GetID());
        }

        action.ClearRepeatDelayTracker();
    }
}

void InputHandler::GamepadButtonDown(const Uint8 button)
{
    if (IsGamepadButtonBeingHeld(button)) return;

    if (!m_heldGamepadButtons.contains(button))
    {
        m_heldGamepadButtons.try_emplace(button, 0.0f);
    }

    for (const InputAction& action : m_actions | std::views::values)
    {
        if (action.IsInputTypeAssignedToGamepadButton(button, InputType::Pressed))
        {
            AddBoundActionToQueue(action.GetID());
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

bool InputHandler::IsInputActionBeingHeld(const Uint16 id)
{
    const InputAction* action = GetInputActionByID(id);
    if (!action)
    {
        std::cerr << "InputHandler::IsInputActionBeingHeld::Failed to find Input action with the ID: " << id << std::endl;
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

void InputHandler::BindFunctionToAction(const Uint16 actionID, const std::function<void()>& function)
{
    const InputAction* foundAction = GetInputActionByID(actionID);
    if (!foundAction)
    {
        std::cerr << "InputHandler::BindFunctionToAction::Unable to find Input Action with the ID: " << actionID << std::endl;
        return;
    }

    m_actionBindings.try_emplace(actionID, function);
}

const InputAction* InputHandler::GetInputActionByID(const Uint16 inID) const
{
    try
    {
        return &m_actions.at(inID);
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "InputHandler::GetInputActionByID::Unable to find action with the ID: " << inID << std::endl;
        return nullptr;
    }
}

InputAction* InputHandler::GetInputActionByID(const Uint16 inID)
{
    try
    {
        return &m_actions.at(inID);
    }
    catch (const std::out_of_range& error)
    {
        std::cerr << "InputHandler::GetInputActionByID::Unable to find action with the ID: " << inID << std::endl;
        return nullptr;
    }
}

const InputAction* InputHandler::GetInputActionByName(const std::string& name) const
{
    for (const InputAction& action : m_actions | std::views::values)
    {
        if (action.GetName() == name)
        {
            return &action;
        }
    }

    // std::cerr << "InputHandler::GetInputActionByName::Unable to find action with the name " << name << std::endl;
    return nullptr;
}

InputAction* InputHandler::GetInputActionByName(const std::string& name)
{
    for (InputAction& action : m_actions | std::views::values)
    {
        if (action.GetName() == name)
        {
            return &action;
        }
    }

    // std::cerr << "InputHandler::GetInputActionByName::Unable to find action with the name " << name << std::endl;
    return nullptr;
}
