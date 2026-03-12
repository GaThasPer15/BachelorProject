#pragma once 

#include <iostream>
#include <ostream>
#include <raylib.h>
#include "rlgl.h"
#include "../Component.h"
#include "../Entity.h"
#include "../../Shaders/MetalCompute.h"
#include "../../Graphic/perlin.h"
#include "../../MathLib/MathLib.h"


namespace GTP{
    class PerlinPlateComponent : public RayEngine::Component{

        public:
            static const std::string ID;
            PerlinPlateComponent(const Color &color = WHITE) : color(color){
                Image img = GenImageColor(1024, 1024, GREEN);
                SpriteTexture = LoadTextureFromImage(img);
                perlin = LoadShader(
                    "../Data/Shaders/perlin.vs",
                    "../Data/Shaders/perlin.fs"
                );
                controlState = 0;
                maxValue = 6;
                int shaderLoc = GetShaderLocation(perlin, "resolution");
                Vector2 resolution = {static_cast<float>(img.width), static_cast<float>(img.height)};
                SetShaderValue(perlin, shaderLoc, &resolution, SHADER_UNIFORM_VEC2);
                SetShaderValue(perlin, GetShaderLocation(perlin, "frequency"), &data.frequency, SHADER_UNIFORM_FLOAT);
                SetShaderValue(perlin, GetShaderLocation(perlin, "amplitude"), &data.amplitude, SHADER_UNIFORM_FLOAT);
                
                perlinPlate = LoadTextureFromImage(GenImageColor(1024, 1024, RED));
                texSize.width = texSize.height = 1024;
                bytes.reserve(texSize.width*texSize.height*4);

                perlinTexGenerator = new Perlin::TextureGenerator(texSize);
                perlinTexGenerator->Generate(perlinPlate, data);
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
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.frequency +=0.1;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.frequency -=0.1;
                        }
                        SetShaderValue(perlin, GetShaderLocation(perlin, "frequency"), &data.frequency, SHADER_UNIFORM_FLOAT);
                    break;
                    case 1:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.amplitude += 0.01;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.amplitude -= 0.01;
                        }
                        SetShaderValue(perlin, GetShaderLocation(perlin, "amplitude"), &data.amplitude, SHADER_UNIFORM_FLOAT);
                    break;
                    case 2:
                        if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held))
                            data.seed.x += 0.025;
                        else if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held))
                            data.seed.x -= 0.025;
                        if(context.Input->GetKey(RayEngine::KeyCode::W, RayEngine::InputState::Held))
                            data.seed.y -= 0.025;
                        else if(context.Input->GetKey(RayEngine::KeyCode::S, RayEngine::InputState::Held))
                            data.seed.y += 0.025;
                        SetShaderValue(perlin, GetShaderLocation(perlin, "seedCoord"), &data.seed, SHADER_UNIFORM_VEC2);
                    break;
                    case 3:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                            data.octave += 1;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                            data.octave-= 1;
                        }
                    break;
                    case 4:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.lacunarity += 0.01;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.lacunarity -= 0.01;
                        }
                    break;
                    case 5:
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Held)){
                            data.persistence += 0.01;
                        }
                        else if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Held)){
                            data.persistence -= 0.01;
                        }
                    break;
                }
                perlinTexGenerator->Generate(perlinPlate, data);
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
                const Rectangle destRect = {position.x-350.0f, position.y, spriteSize.x, spriteSize.y};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};

                BeginShaderMode(perlin);
                DrawTexturePro(SpriteTexture, sourceRect, destRect, origin, rotation, color);
                EndShaderMode();
                
                const Rectangle plateSourceRect = {0.0f, 0.0f, 1024, 1024};
                const Rectangle plateDestRect = {position.x+350.0f, position.y, spriteSize.x, spriteSize.y};
                DrawTexturePro(perlinPlate, plateSourceRect, plateDestRect, origin, rotation, color);

            }
            void OnRenderUI(const RayEngine::RenderUiContext &context)const override{
                std::string text = "Frequency: " + std::to_string(data.frequency);
                DrawText(text.c_str(), 10, 10, 15, BLACK);
                text = "Amplitude: " + std::to_string(data.amplitude);
                DrawText(text.c_str(), 10, 30, 15, BLACK);
                text = "Seed: {" + std::to_string(data.seed.x) + "," + std::to_string(data.seed.y) + "}";
                DrawText(text.c_str(), 10, 50, 15, BLACK);
                text = "Octave: " + std::to_string(data.octave);
                DrawText(text.c_str(), 10, 70, 15, BLACK);
                text = "Lacunatity: " + std::to_string(data.lacunarity);
                DrawText(text.c_str(), 10, 90, 15, BLACK);
                text = "Persistence: " + std::to_string(data.persistence);
                DrawText(text.c_str(), 10, 110, 15, BLACK);
            }
            void OnDestroy() override {
                delete perlinTexGenerator;
                UnloadShader(perlin);
            }
        private:
            Perlin::TextureGenerator *perlinTexGenerator;

            ::Shader perlinTextureGenShader;
            Texture2D perlinPlate;
            Perlin::PerlinData data;

            int controlState;
            int maxValue;
            ::Shader perlin;
            Texture2D SpriteTexture;
            Color color = WHITE;


            Graphic::TextureSize texSize;
            std::vector<unsigned char> bytes;
    };


    const std::string PerlinPlateComponent::ID = "PerlinPlateComonent";
}