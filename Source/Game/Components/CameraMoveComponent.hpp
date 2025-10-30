#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"


namespace GTP{
    class CameraMoveComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            CameraMoveComponent(){}
            Component *Clone() const override{
                return new CameraMoveComponent();
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{
                if(context.Input->GetKey(RayEngine::KeyCode::W, RayEngine::InputState::Held)){
                    context.Camera->target.y -= 10.0f;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::S, RayEngine::InputState::Held)){
                    context.Camera->target.y += 10.0f;
                }
                if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held)){
                    context.Camera->target.x += 10.0f;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held)){
                    context.Camera->target.x -= 10.0f;
                }
                if(context.Input->GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Pressed)){
                    Camera2D camera = *context.Camera;
                    mouseCoord = context.Input->GetCursorWorldPosition(camera);
                }
                if(context.Input->GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Held)){
                    Camera2D camera = *context.Camera;
                    Vector2 tempCoord = context.Input->GetCursorWorldPosition(camera);
                    context.Camera->target.x += (mouseCoord.x - tempCoord.x);
                    context.Camera->target.y += (mouseCoord.y - tempCoord.y);
                }

                if(GetMouseWheelMove() > 0){
                    context.Camera->zoom += 5.0f * context.DeltaTime;
                }
                else if(GetMouseWheelMove() < 0){
                    context.Camera->zoom -= 5.0f * context.DeltaTime;
                }
            }
        private:
            Vector2 mouseCoord;
    };
    const std::string CameraMoveComponent::ID = "CameraMoveComponent";
}