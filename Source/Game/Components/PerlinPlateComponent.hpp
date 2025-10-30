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
                controlState = 0;
                maxValue = 3;
                int shaderLoc = GetShaderLocation(perlin, "resolution");
                Vector2 resolution = {static_cast<float>(img.width), static_cast<float>(img.height)};
                frequency = 10;
                amplitude = 1.0;
                SetShaderValue(perlin, shaderLoc, &resolution, SHADER_UNIFORM_VEC2);
                SetShaderValue(perlin, shaderLoc, &frequency, SHADER_UNIFORM_FLOAT);
            }
            Component *Clone() const override{
                return new PerlinPlateComponent(color);
            }
            const std::string &GetID() const override {return ID;}
            void OnUpdate(const RayEngine::UpdateContext &context) override{

                if(context.Input->GetKey(RayEngine::KeyCode::Up, RayEngine::InputState::Pressed)){
                    controlState==0?:controlState--;
                }
                else if(context.Input->GetKey(RayEngine::KeyCode::Down, RayEngine::InputState::Pressed)){
                    controlState++;
                    controlState%=maxValue;
                }
                switch(controlState){
                    case 0:   
                        if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held)){
                            frequency +=0.1;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held)){
                            frequency -=0.1;
                        }
                        SetShaderValue(perlin, GetShaderLocation(perlin, "frequency"), &frequency, SHADER_UNIFORM_FLOAT);
                    break;
                    case 1:
                        if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held)){
                            amplitude += 0.01;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held)){
                            amplitude -= 0.01;
                        }
                        SetShaderValue(perlin, GetShaderLocation(perlin, "amplitude"), &amplitude, SHADER_UNIFORM_FLOAT);
                    break;
                    case 2:
                        if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held))
                            seedCoord.x += 0.025;
                        else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held))
                            seedCoord.x -= 0.025;
                        if(context.Input->GetKey(RayEngine::KeyCode::W, RayEngine::InputState::Held))
                            seedCoord.y -= 0.025;
                        else if(context.Input->GetKey(RayEngine::KeyCode::S, RayEngine::InputState::Held))
                            seedCoord.y += 0.025;
                        SetShaderValue(perlin, GetShaderLocation(perlin, "seedCoord"), &seedCoord, SHADER_UNIFORM_VEC2);
                    break;
                }
            }
            void OnRender(const RayEngine::RenderContext &context) const override{
                RayEngine::Entity *owner = GetOwner();
                if(owner == nullptr){
                    std::cout << "\nOwner is nullptr\n\n";
                }
                float rotation = owner->GetRotation();
                Vector2 position = owner->GetPosition();
                Vector2 scale = owner->GetScale();
                const float textureWidth = (float)SpriteTexture.width;
                const float textureHeight = (float)SpriteTexture.height;
                const Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
                const Vector2 spriteSize = {1.0f * scale.x, 1.0f * scale.y};
                const Rectangle destRect = {position.x, position.y, spriteSize.x, spriteSize.y};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};

                BeginShaderMode(perlin);
                DrawTexturePro(SpriteTexture, sourceRect, destRect, origin, rotation, color);
                EndShaderMode();

            }
            void OnRenderUI(const RayEngine::RenderUiContext &context)const override{
                std::string freqText = "Frequency: " + std::to_string(frequency);
                DrawText(freqText.c_str(), 10, 10, 15, BLACK);
                std::string amplText = "Amplitude: " + std::to_string(amplitude);
                DrawText(amplText.c_str(), 10, 30, 15, BLACK);
                std::string seedText = "Seed: {" + std::to_string(seedCoord.x) + "," + std::to_string(seedCoord.y) + "}";
                DrawText(seedText.c_str(), 10, 50, 15, BLACK);
            }
            void OnDestroy() override {
                UnloadShader(perlin);
            }
        private:
            int controlState;
            int maxValue;
            Shader perlin;
            Texture2D SpriteTexture;
            Color color = WHITE;
            float frequency;
            float amplitude;
            Vector2 seedCoord;
    };
    const std::string PerlinPlateComponent::ID = "PerlinPlateComonent";
}