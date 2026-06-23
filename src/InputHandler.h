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

    std::vector<InputAction> m_actions;
    std::unordered_map<std::string, std::function<void()>> m_actionBindings;

    InputAction* CreateInputAction(const std::string& inName);

    void TriggerInputAction(const std::string& actionName) const;
    void TriggerHeldInputs();

public:
    void Update(float deltaTime);

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

    void BindFunctionToAction(const std::string& actionName, const std::function<void()>& function);

    InputAction* GetInputAction(const std::string& name);
};
