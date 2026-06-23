#include "InputAction.h"

#include <ranges>

InputAction::InputAction(const std::string& inName) : m_name(inName)
{

}

auto InputAction::operator<=>(const InputAction& other) const
{
    return m_name <=> other.m_name;
}

void InputAction::AssignKey(const SDL_Keycode key, const InputType inputType)
{
    m_assignedKeys.emplace_back(std::pair(key, inputType));
}

void InputAction::AssignMouseButton(const Uint8 button, const InputType inputType)
{
   m_assignedMouseButtons.emplace_back(std::pair(button, inputType));
}

void InputAction::AssignGamepadButton(const Uint8 button, const InputType inputType)
{
    m_assignedGamepadButtons.emplace_back(std::pair(button, inputType));
}

void InputAction::SetHoldDuration(const float inDuration)
{
    m_holdDuration = inDuration;
}

void InputAction::SetRepeatDelay(const float inTime)
{
    m_repeatDelay = inTime;
}

void InputAction::AddToRepeatDelayTracker(const float inTime)
{
    m_repeatDelayTracker += inTime;
}

void InputAction::ClearRepeatDelayTracker()
{
    m_repeatDelayTracker = 0.0f;
}

const std::string& InputAction::GetName() const
{
    return m_name;
}

float InputAction::GetHoldDuration() const
{
    return m_holdDuration;
}

float InputAction::GetRepeatDelay() const
{
    return m_repeatDelay;
}

bool InputAction::WasRepeatDelayPassed() const
{
    return m_repeatDelayTracker >= m_repeatDelay;
}

bool InputAction::IsKeyAssigned(const SDL_Keycode inKey) const
{
    for (const auto key : m_assignedKeys | std::views::keys)
    {
        if (key == inKey) return true;
    }

    return false;
}

bool InputAction::IsMouseButtonAssigned(const Uint8 inButton) const
{
    for (const auto button : m_assignedMouseButtons | std::views::keys)
    {
        if (button == inButton) return true;
    }

    return false;
}

bool InputAction::IsGamepadButtonAssigned(const Uint8 inButton) const
{
    for (const auto button : m_assignedGamepadButtons | std::views::keys)
    {
        if (button == inButton) return true;
    }

    return false;
}

bool InputAction::IsInputTypeAssignedToKey(const SDL_Keycode inKey, const InputType inType) const
{
    for (const auto [key, inputType] : m_assignedKeys)
    {
        if (key == inKey && inType == inputType) return true;
    }

    return false;
}

bool InputAction::IsInputTypeAssignedToMouseButton(const Uint8 inButton, const InputType inType) const
{
    for (const auto [button, inputType] : m_assignedMouseButtons)
    {
        if (button == inButton && inType == inputType) return true;
    }

    return false;
}

bool InputAction::IsInputTypeAssignedToGamepadButton(const Uint8 inButton, const InputType inType) const
{
    for (const auto [button, inputType] : m_assignedGamepadButtons)
    {
        if (button == inButton && inType == inputType) return true;
    }

    return false;
}
