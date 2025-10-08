#include "Input.h"

void RayEngine::Input::Handle(const Vector2& screenSize){
    m_ScreenSize = screenSize;
}

bool RayEngine::Input::GetKey(KeyCode keyCode, InputState state) const{
    switch (state){
        case InputState::Pressed:
            return IsKeyPressed((int) keyCode);
        break;
        case InputState::Relased:
            return IsKeyReleased((int) keyCode);
        break;
        case InputState::Held:
            return IsKeyDown((int) keyCode);
        break;
    }
}

bool RayEngine::Input::GetMouseButton(MouseButton mouseButton, InputState state) const{
    switch (state){
        case InputState::Pressed:
            return IsMouseButtonPressed((int) mouseButton);
        break;
        case InputState::Relased:
            return IsMouseButtonReleased((int) mouseButton);
        break;
        case InputState::Held:
            return IsMouseButtonDown((int) mouseButton);
        break;
    }
}

Vector2 RayEngine::Input::GetCursorPosition() const{
    const Vector2 mouseScreenPosition = GetMousePosition();
    Vector2 result;
    result.x = mouseScreenPosition.x / m_ScreenSize.x;
    result.y = mouseScreenPosition.y / m_ScreenSize.y;
    return result;
}

Vector2 RayEngine::Input::GetCursorWorldPosition(const Camera2D &camera) const{
    const Vector2 mouseScreenPosition = GetMousePosition();
    return GetScreenToWorld2D(mouseScreenPosition, camera);
}