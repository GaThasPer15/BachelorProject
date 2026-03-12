#pragma once 
#include <iostream>
#include <ostream>
#include <raylib.h>
#include "rlgl.h"
#include "../Component.h"
#include "../Entity.h"
#include "../../Shaders/MetalCompute.h"
#include "../../MathLib/MathLib.h"
#include "../../Graphic/VoronoiDiagram.h"

namespace GPT{
    class VoronoiComponent : public RayEngine::Component{
        public:
            static const std::string ID;
            VoronoiComponent(){
                Image img = GenImageColor(1024, 1024, BLANK);
                plate = LoadTextureFromImage(img);
                UnloadImage(img);
                data = Voronoi::VoronoiData();
                data.size = 20;
                seed = 1;
                maxValue = 2;
                controlState = 0;
                texSize = {1024, 1024};
                
                GenerateVoronoiData();
                generator = new Voronoi::Generator(texSize);
                generator->Generate(plate, data);
                updatedFlag = false;
            }
            Component *Clone() const override{
                return new VoronoiComponent();
            }
            const std::string &GetID() const override {return ID;}
            void OnRender(const RayEngine::RenderContext &context) const override{
                RayEngine::Entity *owner = GetOwner();
                if(owner == nullptr){
                    std::cout << "\nOwner is nullptr\n\n";
                }
                float rotation = owner->GetRotation();
                Vector2 position = owner->GetPosition();
                Vector2 scale = owner->GetScale();
                const Vector2 spriteSize = {1.0f * scale.x, 1.0f * scale.y};
                const Rectangle destRect = {0, 0, spriteSize.x, spriteSize.y};
                const Vector2 origin = {spriteSize.x * 0.5f, spriteSize.y * 0.5f};
                const Rectangle sourceRect = {0.0f, 0.0f, 1024, 1024};
                DrawTexturePro(plate, sourceRect, destRect, origin, rotation, WHITE);
            }
            void OnRenderUI(const RayEngine::RenderUiContext &context) const override{
                std::string text = "Points amount: " + std::to_string(data.size);
                DrawText(text.c_str(), 10, 10, 15, BLACK);
                text = "Seed: " + std::to_string(seed);
                DrawText(text.c_str(), 10, 30, 15, BLACK);
            }
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
                        if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                            if(data.size != 1){
                                data.size--;
                                updatedFlag = true;
                            }
                        }
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                            data.size++;
                            updatedFlag = true;
                        }
                        if(context.Input->GetKey(RayEngine::KeyCode::R, RayEngine::InputState::Pressed)){
                            data.size = rand() % 1000;
                            updatedFlag = true;
                        }
                    break;
                    case 1:
                        if(context.Input->GetKey(RayEngine::KeyCode::Left, RayEngine::InputState::Pressed)){
                            if(seed != 0){
                                seed--;
                                updatedFlag = true;
                            }
                        }
                        if(context.Input->GetKey(RayEngine::KeyCode::Right, RayEngine::InputState::Pressed)){
                            seed++;
                            updatedFlag = true;
                        }   
                        if(context.Input->GetKey(RayEngine::KeyCode::R, RayEngine::InputState::Pressed)){
                            seed = rand() % 100000;
                            updatedFlag = true;
                        }
                    break;
                    }
                    if(updatedFlag){
                        GenerateVoronoiData();
                        generator->Generate(plate, data);
                        updatedFlag = false;
                    }
            }
            void OnDestroy() override{
                delete data.colors;
                delete data.points;
                UnloadTexture(plate);
            }
        private:
        Texture2D plate;
        Voronoi::VoronoiData data;
        Graphic::TextureSize texSize;
        Voronoi::Generator *generator;

        bool updatedFlag;
        unsigned int seed;

        int controlState;
        int maxValue;

        void GenerateVoronoiData(){
            GTPMath::Random::HashRNGGenerator *numGen = new GTPMath::Random::HashRNGGenerator(seed);
                
                if(data.points != nullptr) delete data.points;
                data.points = new Vector2 [data.size];
                if(data.colors != nullptr) delete data.colors;
                data.colors = new Color [data.size];
                for(int i=0; i<data.size; i++){
                     data.points[i] = numGen->Vector2(0.0f, 0.0f);
                     data.colors[i] = Color{
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        static_cast<unsigned char>(numGen->Int(static_cast<int32_t>(data.points[i].x), static_cast<int32_t>(data.points[i].y))%256),
                        255
                     };
                }
                delete numGen;
        }
    };
    const std::string VoronoiComponent::ID = "VoronoiComponent";
}