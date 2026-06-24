#pragma once

#include <unordered_map>
#include <functional>

#include "InputAction.h"

class Application;

class InputHandler
{
public:
    explicit InputHandler(Application& app);
    ~InputHandler();

protected:
    Application& m_app;

    std::unordered_map<SDL_Keycode, float> m_heldKeys;
    std::unordered_map<Uint8, float> m_heldMouseButtons;
    std::unordered_map<Uint8, float> m_heldGamepadButtons;

    std::unordered_map<Uint16, InputAction> m_actions;
    std::unordered_map<Uint16, std::function<void()>> m_actionBindings;
    std::vector<Uint16> m_actionQueue;

    void UpdateHeldInputs(float deltaTime);

    void AddBoundActionToQueue(Uint16 actionID);

    void ExecuteQueuedActions();

public:
    void Update(float deltaTime);

    InputAction* CreateInputAction(const std::string& inName);

    void KeyUp(SDL_Keycode key);
    void KeyDown(SDL_Keycode key);

    void MouseButtonUp(Uint8 button);
    void MouseButtonDown(Uint8 button);
    void MouseMoved();

    void GamepadButtonUp(Uint8 button);
    void GamepadButtonDown(Uint8 button);

    bool IsKeyBeingHeld(SDL_Keycode key) const;
    bool IsMouseButtonBeingHeld(Uint8 button) const;
    bool IsGamepadButtonBeingHeld(Uint8 button) const;

    bool IsInputActionBeingHeld(Uint16 id);

    void BindFunctionToAction(Uint16 actionID, const std::function<void()>& function);

    const InputAction* GetInputActionByID(Uint16 inID) const;
    InputAction* GetInputActionByID(Uint16 inID);
    const InputAction* GetInputActionByName(const std::string& name) const;
    InputAction* GetInputActionByName(const std::string& name);
};
