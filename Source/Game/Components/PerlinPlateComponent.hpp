#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "../Component.h"
#include "../Entity.h"



namespace GTP{
    class PerlinPlateComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            PerlinPlateComponent(const Color &color = WHITE) : color(color){
                Image img = GenImageColor(1024, 1024, GREEN);
                SpriteTexture = LoadTextureFromImage(img);
                perlin = LoadShader(
                    "../../Data/Shaders/perlin.vs",
                    "../../Data/Shaders/perlin.fs"
                );
                int shaderLoc = GetShaderLocation(perlin, "resolution");
                Vector2 resolution = {static_cast<float>(img.width), static_cast<float>(img.height)};
                frequency = 10000;
                SetShaderValue(perlin, shaderLoc, &resolution, SHADER_UNIFORM_VEC2);
            }
            Component *Clone() const override{
                return new PerlinPlateComponent(color);
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{

                if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Pressed)){
                    frequency += 1000;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Pressed)){
                    frequency -=1000;
                }
                SetShaderValue(perlin, GetShaderLocation(perlin, "frequency"), &frequency, SHADER_UNIFORM_FLOAT);
            }
            void OnRender(const RayEngine::RenderContext &context) const override{
                RayEngine::Entity *owner = GetOwner();
                if(owner == nullptr){
                    std::cout << "\nOwner is nullptr\n\n";
                }
                float rotation = owner->GetRotation();
                Vector2 position = owner->GetPosition();
                Vector2 frequency = owner->GetScale();
                const float textureWidth = (float)SpriteTexture.width;
                const float textureHeight = (float)SpriteTexture.height;
                const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
                const Vector2 spriteSize = {1.0f * frequency.x, 1.0f * frequency.y};
                const Rectangle destRect = {position.x, position.y, spriteSize.x, spriteSize.y};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};

                BeginShaderMode(perlin);
                DrawTexturePro(SpriteTexture, sourceRect, destRect, origin, rotation, color);
                EndShaderMode();
            }
            void OnDestroy() override {
                UnloadShader(perlin);
            }
        private:
            Shader perlin;
            Texture2D SpriteTexture;
            Color color = WHITE;
            float frequency;
            float amplitude;
            vec2 seedCoord;
    };
    const std::string PerlinPlateComponent::ID = "PerlinPlateComonent";
}