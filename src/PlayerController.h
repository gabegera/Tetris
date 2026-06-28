#pragma once

class InputAction;
class InputHandler;
class Application;

class PlayerController
{
public:
    explicit PlayerController(Application& app);
    ~PlayerController();

protected:
    Application& m_app;

    InputAction* m_exitAction = nullptr;
    InputAction* m_moveShapeRightAction = nullptr;
    InputAction* m_moveShapeLeftAction = nullptr;
    InputAction* m_moveShapeDownAction = nullptr;
    InputAction* m_dropShapeAction = nullptr;
    InputAction* m_rotateClockwiseAction = nullptr;
    InputAction* m_rotateCounterClockwiseAction = nullptr;
    InputAction* m_holdShapeAction = nullptr;
    InputAction* m_selectNextUIElementAction = nullptr;
    InputAction* m_selectPreviousUIElementAction = nullptr;
    InputAction* m_triggerUIElementAction = nullptr;
    InputAction* m_clickUIElementAction = nullptr;

public:
    void Exit() const;

    void MoveShapeRight() const;

    void MoveShapeLeft() const;

    void MoveShapeDown() const;

    void DropShape() const;

    void RotateClockwise() const;

    void RotateCounterClockwise() const;

    void HoldShape() const;

    void SelectNextUIElement() const;

    void SelectPreviousUIElement() const;

    void TriggerUIElement() const;

    void ClickUIElement() const;

    InputHandler* GetInputHandler() const;
};
