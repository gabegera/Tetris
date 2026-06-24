#pragma once

#include <vector>
#include <string>

#include "SDL3/SDL.h"

enum class InputType : Uint8
{
    Pressed = 0,
    Released = 1,
    Held = 2
};

class InputAction
{
public:
    explicit InputAction(const std::string& inName);

private:
    inline static Uint16 m_idCounter = 0;

protected:
    const Uint16 m_id;
    const std::string m_name;
    std::vector<std::pair<SDL_Keycode, InputType>> m_assignedKeys;
    std::vector<std::pair<Uint8, InputType>> m_assignedMouseButtons;
    std::vector<std::pair<Uint8, InputType>> m_assignedGamepadButtons;
    float m_holdDuration = 0.5; // Number of seconds the input has to be held to trigger the action.
    float m_repeatDelay = 0.0f; // Number of seconds before the action can be repeated.
    float m_repeatDelayTracker = 0.0f;

public:
    auto operator<=>(const InputAction& other) const;

    void AssignKey(SDL_Keycode key, InputType inputType);
    void AssignMouseButton(Uint8 button, InputType inputType);
    void AssignGamepadButton(Uint8 button, InputType inputType);

    // In Seconds
    void SetHoldDuration(float inDuration);

    // In Seconds
    void SetRepeatDelay(float inTime);

    // In Seconds.
    void AddToRepeatDelayTracker(float inTime);

    // In Seconds.
    void ClearRepeatDelayTracker();

    Uint8 GetID() const;

    const std::string& GetName() const;

    const std::vector<std::pair<SDL_Keycode, InputType>>& GetAssignedKeys() const;
    const std::vector<std::pair<Uint8, InputType>>& GetAssignedMouseButtons() const;
    const std::vector<std::pair<Uint8, InputType>>& GetAssignedGamepadButtons() const;

    // In Seconds
    float GetHoldDuration() const;

    // In Seconds
    float GetRepeatDelay() const;

    bool WasRepeatDelayPassed() const;

    bool IsKeyAssigned(SDL_Keycode inKey) const;
    bool IsMouseButtonAssigned(Uint8 inButton) const;
    bool IsGamepadButtonAssigned(Uint8 inButton) const;

    bool IsInputTypeAssignedToKey(SDL_Keycode inKey, InputType inType) const;
    bool IsInputTypeAssignedToMouseButton(Uint8 inButton, InputType inType) const;
    bool IsInputTypeAssignedToGamepadButton(Uint8 inButton, InputType inType) const;
};
