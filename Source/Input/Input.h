#pragma once
#include "InputCodes.h"

namespace RayEngine{
    enum class InputState{
        Pressed, Held, Relased
    };

    class Input{
        public:
            Input() = default;
            void Handle(const Vector2 &screenSize);
            bool GetKey(KeyCode keyCode, InputState state) const;
            bool GetMouseButton(MouseButton mouseButton, InputState state) const;
            Vector2 GetCursorPosition() const;
            Vector2 GetCursorWorldPosition(const Camera2D &camera) const;
        private:
            Vector2 m_ScreenSize;
    };
}